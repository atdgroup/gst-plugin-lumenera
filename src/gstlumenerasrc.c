/* GStreamer lumenera Plugin
 * Copyright (C) 2014 Gray Cancer Institute
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 * 
 * Author: P Barber
 *
 */
/**
 * SECTION:element-gstlumenera_src
 *
 * The lumenerasrc element is a source for a USB 3 camera supported by the IDS lumenera SDK.
 * A live source, operating in push mode.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v lumenerasrc ! autovideosink
 * ]|
 * Shows video from the default camera source.
 * </refsect2>
 */

// Which functions of the base class to override. Create must alloc and fill the buffer. Fill just needs to fill it
//#define OVERRIDE_FILL  !!! NOT IMPLEMENTED !!!
#define OVERRIDE_CREATE

#include <unistd.h> // for usleep
#include <string.h> // for memcpy

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>
#include <gst/video/video.h>

#include "gstlumenerasrc.h"

GST_DEBUG_CATEGORY_STATIC (gst_lumenera_src_debug);
#define GST_CAT_DEFAULT gst_lumenera_src_debug

/* prototypes */
static void gst_lumenera_src_set_property (GObject * object,
		guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_lumenera_src_get_property (GObject * object,
		guint property_id, GValue * value, GParamSpec * pspec);
static void gst_lumenera_src_dispose (GObject * object);
static void gst_lumenera_src_finalize (GObject * object);

static gboolean gst_lumenera_src_start (GstBaseSrc * src);
static gboolean gst_lumenera_src_stop (GstBaseSrc * src);
static GstCaps *gst_lumenera_src_get_caps (GstBaseSrc * src, GstCaps * filter);
static gboolean gst_lumenera_src_set_caps (GstBaseSrc * src, GstCaps * caps);

#ifdef OVERRIDE_CREATE
	static GstFlowReturn gst_lumenera_src_create (GstPushSrc * src, GstBuffer ** buf);
#endif
#ifdef OVERRIDE_FILL
	static GstFlowReturn gst_lumenera_src_fill (GstPushSrc * src, GstBuffer * buf);
#endif

//static GstCaps *gst_lumenera_src_create_caps (GstLumeneraSrc * src);
static void gst_lumenera_src_reset (GstLumeneraSrc * src);
enum
{
	PROP_0,
	PROP_CAMERAPRESENT,
	PROP_EXPOSURE,
	PROP_GAIN,
//	PROP_BLACKLEVEL,   // Cannot currently control the black level with the Linux SDK
	PROP_RGAIN,
	PROP_GGAIN,
	PROP_BGAIN,
	PROP_HORIZ_FLIP,
	PROP_VERT_FLIP,
	PROP_WHITEBALANCE,
	PROP_MAXFRAMERATE
};


#define	LU_UPDATE_LOCAL  FALSE
#define	LU_UPDATE_CAMERA TRUE

#define DEFAULT_PROP_EXPOSURE           20.0
#define DEFAULT_PROP_GAIN               1
//#define DEFAULT_PROP_BLACKLEVEL         128
#define DEFAULT_PROP_RGAIN              1.109374    
#define DEFAULT_PROP_GGAIN              1.0625     
#define DEFAULT_PROP_BGAIN              1.921875
#define DEFAULT_PROP_HORIZ_FLIP         0
#define DEFAULT_PROP_VERT_FLIP          0
#define DEFAULT_PROP_WHITEBALANCE       GST_WB_DISABLED
#define DEFAULT_PROP_MAXFRAMERATE       25

#define DEFAULT_LU_VIDEO_FORMAT GST_VIDEO_FORMAT_RGB
// Put matching type text in the pad template below

// pad template
static GstStaticPadTemplate gst_lumenera_src_template =
		GST_STATIC_PAD_TEMPLATE ("src",
				GST_PAD_SRC,
				GST_PAD_ALWAYS,
				GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE
						("{ RGB }"))
		);

// error check, use in functions where 'src' is declared and initialised
#define LUEXECANDCHECK(function)\
{\
	BOOL Ret = function;\
	if (!Ret){\
		ULONG Err=LucamGetLastError();\
		GST_ERROR_OBJECT(src, "Lucam call failed with: %d (see lucamerr.h)", Err);\
	}\
}

// I removed this line because it caused a glib error when dynamically changing the pipeline to start recording.
// But no "lumenera call failed ..." error is seen, with or without it!
// g_free(pcErr);\    // This free was in the LUEXECANDCHECK define, after the GST_ERROR_OBJECT line.

#define TYPE_WHITEBALANCE (whitebalance_get_type ())
static GType
whitebalance_get_type (void)
{
  static GType whitebalance_type = 0;

  if (!whitebalance_type) {
    static GEnumValue wb_types[] = {
	  { GST_WB_DISABLED, "Auto white balance disabled.",    "disabled" },
	  { GST_WB_ONESHOT,  "One shot white balance.", "oneshot"  },
	  { GST_WB_AUTO, "Auto white balance.", "auto" },
      { 0, NULL, NULL },
    };

    whitebalance_type =
	g_enum_register_static ("WhiteBalanceType", wb_types);
  }

  return whitebalance_type;
}

static void
gst_lumenera_set_camera_exposure (GstLumeneraSrc * src, gboolean send)
{  // How should the pipeline be told/respond to a change in frame rate - seems to be ok with a push source
	LONG flags;

	// With the Lumenera always choose the max frame rate
	src->framerate = src->maxframerate;
	src->duration = 1000000000.0/src->framerate;  // frame duration in ns
	if (send && src->hCam){
		GST_DEBUG_OBJECT(src, "Request frame rate to %.1f, duration %u us, and exposure to %.1f ms", src->framerate, (unsigned int)GST_TIME_AS_USECONDS(src->duration), src->exposure);
//		GST_DEBUG_OBJECT(src, "LucamSetFormat");
		LucamSetFormat(src->hCam, &(src->frameFormat), src->framerate); // set a suitable frame rate for the exposure, if too fast for usb camera will slow it down, get the actual frame rate back
//		GST_DEBUG_OBJECT(src, "LucamSetProperty");
		LucamSetProperty(src->hCam, LUCAM_PROP_EXPOSURE, src->exposure, LUCAM_PROP_FLAG_USE);
		// Get the exposure value actually set back from the camera
//		GST_DEBUG_OBJECT(src, "LucamGetProperty");
		LucamGetProperty(src->hCam, LUCAM_PROP_EXPOSURE, &(src->exposure), &flags);
//		LucamGetFormat(src->hCam, &(src->frameFormat), &(src->framerate));
		// The camera always returns the nominal framerate, we must calculate it
		src->framerate = 1000.0/(src->exposure);
		src->framerate = MIN(src->framerate, src->maxframerate);
		// Update the duration to the actual value
		src->duration = 1000000000.0/src->framerate;  // frame duration in ns
		GST_DEBUG_OBJECT(src, "Set frame rate to %.1f, duration %u us, and exposure to %.1f ms", src->framerate, (unsigned int)GST_TIME_AS_USECONDS(src->duration), src->exposure);
	}
}

static void
gst_lumenera_set_camera_whitebalance (GstLumeneraSrc * src)
{
	LONG flags;
	float gain_green2;

	switch (src->whitebalance){
//		double dblAutoWb;
	case GST_WB_AUTO:   // the following code is from the lumenera demo program (tabProcessing.cpp)
//		dblAutoWb = 0.0;
//		is_SetAutoParameter (src->hCam, IS_SET_AUTO_WB_ONCE, &dblAutoWb, NULL);
//		dblAutoWb = 1.0;
//		is_SetAutoParameter (src->hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblAutoWb, NULL);
		break;
	case GST_WB_ONESHOT:
		LucamOneShotAutoWhiteBalance(src->hCam, 0, 0, src->imageFormat.Width, src->imageFormat.Height);
		usleep(500*1000);
		LucamDigitalWhiteBalance(src->hCam, 0, 0, src->imageFormat.Width, src->imageFormat.Height);
		usleep(500*1000);
//		// TODO somehow, after the WB finished signal is received, return state to GST_WB_DISABLED. Seems OK without this.
		LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_RED, &(src->rgain), &flags);
		LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN1, &(src->ggain), &flags);
		LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN2, &gain_green2, &flags);
		LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_BLUE, &(src->bgain), &flags);
		GST_DEBUG_OBJECT(src, "White balance set: R %f G1 %f G2 %f B %f", src->rgain, src->ggain, gain_green2, src->bgain);
		break;
	case GST_WB_DISABLED:
	default:
//		dblAutoWb = 0.0;
//		is_SetAutoParameter (src->hCam, IS_SET_AUTO_WB_ONCE, &dblAutoWb, NULL);
//		is_SetAutoParameter (src->hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblAutoWb, NULL);
		break;
	}
}

/* class initialisation */

G_DEFINE_TYPE (GstLumeneraSrc, gst_lumenera_src, GST_TYPE_PUSH_SRC);

static void
gst_lumenera_src_class_init (GstLumeneraSrcClass * klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GstElementClass *gstelement_class = GST_ELEMENT_CLASS (klass);
	GstBaseSrcClass *gstbasesrc_class = GST_BASE_SRC_CLASS (klass);
	GstPushSrcClass *gstpushsrc_class = GST_PUSH_SRC_CLASS (klass);

	GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, "lumenerasrc", 0,
			"lumenera Camera source");

	gobject_class->set_property = gst_lumenera_src_set_property;
	gobject_class->get_property = gst_lumenera_src_get_property;
	gobject_class->dispose = gst_lumenera_src_dispose;
	gobject_class->finalize = gst_lumenera_src_finalize;

	gst_element_class_add_pad_template (gstelement_class,
			gst_static_pad_template_get (&gst_lumenera_src_template));

	gst_element_class_set_static_metadata (gstelement_class,
			"lumenera Video Source", "Source/Video",
			"lumenera Camera video source", "Paul R. Barber <paul.barber@oncology.ox.ac.uk>");

	gstbasesrc_class->start = GST_DEBUG_FUNCPTR (gst_lumenera_src_start);
	gstbasesrc_class->stop = GST_DEBUG_FUNCPTR (gst_lumenera_src_stop);
	gstbasesrc_class->get_caps = GST_DEBUG_FUNCPTR (gst_lumenera_src_get_caps);
	gstbasesrc_class->set_caps = GST_DEBUG_FUNCPTR (gst_lumenera_src_set_caps);

#ifdef OVERRIDE_CREATE
	gstpushsrc_class->create = GST_DEBUG_FUNCPTR (gst_lumenera_src_create);
	GST_DEBUG ("Using gst_lumenera_src_create.");
#endif
#ifdef OVERRIDE_FILL
	gstpushsrc_class->fill   = GST_DEBUG_FUNCPTR (gst_lumenera_src_fill);
	GST_DEBUG ("Using gst_lumenera_src_fill.");
#endif

	// Install GObject properties
	// Camera Present property
	g_object_class_install_property (gobject_class, PROP_CAMERAPRESENT,
			g_param_spec_boolean ("devicepresent", "Camera Device Present", "Is the camera present and connected OK?",
					FALSE, G_PARAM_READABLE));
	// Exposure property
	g_object_class_install_property (gobject_class, PROP_EXPOSURE,
	  g_param_spec_double("exposure", "Exposure", "Camera sensor exposure time (ms).", 0.01, 2000, DEFAULT_PROP_EXPOSURE,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// Gain property
	g_object_class_install_property (gobject_class, PROP_GAIN,
	  g_param_spec_int("gain", "Gain", "Camera sensor master gain.", 0, 100, DEFAULT_PROP_GAIN,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// Black Level property
//	g_object_class_install_property (gobject_class, PROP_BLACKLEVEL,
//	  g_param_spec_int("blacklevel", "Black Level", "Camera sensor black level offset.", 0, 255, DEFAULT_PROP_BLACKLEVEL,
//          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// R gain property
	g_object_class_install_property (gobject_class, PROP_RGAIN,
			g_param_spec_float("rgain", "Red Gain", "Camera sensor red channel gain.", 1.0, 3.984375, DEFAULT_PROP_RGAIN,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// G gain property
	g_object_class_install_property (gobject_class, PROP_GGAIN,
			g_param_spec_float("ggain", "Green Gain", "Camera sensor green channel gain.", 1.0, 3.984375, DEFAULT_PROP_GGAIN,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// B gain property
	g_object_class_install_property (gobject_class, PROP_BGAIN,
			g_param_spec_float("bgain", "Blue Gain", "Camera sensor blue channel gain.", 1.0, 3.984375, DEFAULT_PROP_BGAIN,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// vflip property
	g_object_class_install_property (gobject_class, PROP_VERT_FLIP,
	  g_param_spec_int("vflip", "Vertical flip", "Image up-down flip.", 0, 1, DEFAULT_PROP_HORIZ_FLIP,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// hflip property
	g_object_class_install_property (gobject_class, PROP_HORIZ_FLIP,
	  g_param_spec_int("hflip", "Horizontal flip", "Image left-right flip.", 0, 1, DEFAULT_PROP_VERT_FLIP,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// White balance property
	g_object_class_install_property (gobject_class, PROP_WHITEBALANCE,
	  g_param_spec_enum("whitebalance", "White Balance", "White Balance mode. Disabled, One Shot or Auto. Not sure this works - FIXME!", TYPE_WHITEBALANCE, DEFAULT_PROP_WHITEBALANCE,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
	// Max Frame Rate property
	g_object_class_install_property (gobject_class, PROP_MAXFRAMERATE,
	  g_param_spec_double("maxframerate", "Maximum Frame Rate", "Camera sensor maximum allowed frame rate (fps)."
			  "The frame rate will be determined from the exposure time, up to this maximum value when short exposures are used", 10, 200, DEFAULT_PROP_MAXFRAMERATE,
          (GParamFlags) (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | GST_PARAM_MUTABLE_PLAYING)));
}

static void
gst_lumenera_src_init (GstLumeneraSrc * src)
{
	/* set source as live (no preroll) */
	gst_base_src_set_live (GST_BASE_SRC (src), TRUE);

	/* override default of BYTES to operate in time mode */
	gst_base_src_set_format (GST_BASE_SRC (src), GST_FORMAT_TIME);

	// Initialise properties
	src->exposure = DEFAULT_PROP_EXPOSURE;
	gst_lumenera_set_camera_exposure(src, LU_UPDATE_LOCAL);
	src->gain = DEFAULT_PROP_GAIN;
//	src->blacklevel = DEFAULT_PROP_BLACKLEVEL;
	src->rgain = DEFAULT_PROP_RGAIN;
	src->ggain = DEFAULT_PROP_GGAIN;
	src->bgain = DEFAULT_PROP_BGAIN;
	src->vflip = DEFAULT_PROP_VERT_FLIP;
	src->hflip = DEFAULT_PROP_HORIZ_FLIP;
	src->whitebalance = DEFAULT_PROP_WHITEBALANCE;
	src->maxframerate = DEFAULT_PROP_MAXFRAMERATE;

	gst_lumenera_src_reset (src);
}

static void
gst_lumenera_src_reset (GstLumeneraSrc * src)
{
	src->hCam=0;
	src->rgbImageOwnerIsProducer = FALSE;
	src->cameraPresent = FALSE;
	src->n_frames=0;
	src->total_timeouts = 0;
	src->last_frame_time = 0;
}

void
gst_lumenera_src_set_property (GObject * object, guint property_id,
		const GValue * value, GParamSpec * pspec)
{
	GstLumeneraSrc *src;

	src = GST_LU_SRC (object);

	switch (property_id) {
	case PROP_CAMERAPRESENT:
		src->cameraPresent = g_value_get_boolean (value);
		break;
	case PROP_EXPOSURE:
		src->exposure = g_value_get_double(value);
		gst_lumenera_set_camera_exposure(src, LU_UPDATE_CAMERA);
		break;
	case PROP_GAIN:
		src->gain = g_value_get_int (value);  // will be 0-100%
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_GAIN, src->cam_min_gain + src->gain/100.0*(src->cam_max_gain-src->cam_min_gain), LUCAM_PROP_FLAG_USE);
		break;
//	case PROP_BLACKLEVEL:
//		src->blacklevel = g_value_get_int (value);
//		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_BLACK_LEVEL, src->blacklevel, LUCAM_PROP_FLAG_USE);
		break;
	case PROP_RGAIN:
		src->rgain = g_value_get_float(value);
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_RED, src->rgain, LUCAM_PROP_FLAG_USE);
		break;
	case PROP_GGAIN:
		src->ggain = g_value_get_float(value);
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN1, src->ggain, LUCAM_PROP_FLAG_USE);
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN2, src->ggain, LUCAM_PROP_FLAG_USE);
		break;
	case PROP_BGAIN:
		src->bgain = g_value_get_float(value);
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_BLUE, src->bgain, LUCAM_PROP_FLAG_USE);
		break;
	case PROP_HORIZ_FLIP:
		src->hflip = g_value_get_int (value);
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_FLIPPING_X, src->hflip, LUCAM_PROP_FLAG_USE);
		break;
	case PROP_VERT_FLIP:
		src->vflip = g_value_get_int (value);
		if (src->hCam) LucamSetProperty(src->hCam, LUCAM_PROP_FLIPPING_Y, src->vflip, LUCAM_PROP_FLAG_USE);
		break;
	case PROP_WHITEBALANCE:
		src->whitebalance = g_value_get_enum (value);
		gst_lumenera_set_camera_whitebalance(src);
		break;
	case PROP_MAXFRAMERATE:
		src->maxframerate = g_value_get_double(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

void
gst_lumenera_src_get_property (GObject * object, guint property_id,
		GValue * value, GParamSpec * pspec)
{
	GstLumeneraSrc *src;
	LONG flags;

	g_return_if_fail (GST_IS_LU_SRC (object));
	src = GST_LU_SRC (object);

	switch (property_id) {
	case PROP_CAMERAPRESENT:
		g_value_set_boolean (value, src->cameraPresent);
		break;
	case PROP_EXPOSURE:
		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_EXPOSURE, &(src->exposure), &flags);
		g_value_set_double (value, src->exposure);
		break;
	case PROP_GAIN:
		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_GAIN, &(src->gain), &flags);
		src->gain = (src->gain - src->cam_min_gain) * 100.0/(src->cam_max_gain-src->cam_min_gain);
		g_value_set_int (value, src->gain);
		break;
//	case PROP_BLACKLEVEL:
//		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_BLACK_LEVEL, &(src->blacklevel), &flags);
//		g_value_set_int (value, src->blacklevel);
//		break;
	case PROP_RGAIN:
		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_RED, &(src->rgain), &flags);
		g_value_set_float (value, src->rgain);
		break;
	case PROP_GGAIN:
		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN1, &(src->ggain), &flags);
//		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN1, &(src->ggain), &flags);  // Use green1 - we always set the same for 1 and 2
		g_value_set_float (value, src->ggain);
		break;
	case PROP_BGAIN:
		if (src->hCam) LucamGetProperty(src->hCam, LUCAM_PROP_GAIN_BLUE, &(src->bgain), &flags);
		g_value_set_float (value, src->bgain);
		break;
	case PROP_HORIZ_FLIP:
		g_value_set_int (value, src->hflip);
		break;
	case PROP_VERT_FLIP:
		g_value_set_int (value, src->vflip);
		break;
	case PROP_WHITEBALANCE:
		g_value_set_enum (value, src->whitebalance);
		break;
	case PROP_MAXFRAMERATE:
		g_value_set_double (value, src->maxframerate);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

void
gst_lumenera_src_dispose (GObject * object)
{
	GstLumeneraSrc *src;

	g_return_if_fail (GST_IS_LU_SRC (object));
	src = GST_LU_SRC (object);

	GST_DEBUG_OBJECT (src, "dispose");

	// clean up as possible.  may be called multiple times

	G_OBJECT_CLASS (gst_lumenera_src_parent_class)->dispose (object);
}

void
gst_lumenera_src_finalize (GObject * object)
{
	GstLumeneraSrc *src;

	g_return_if_fail (GST_IS_LU_SRC (object));
	src = GST_LU_SRC (object);

	GST_DEBUG_OBJECT (src, "finalize");

	/* clean up object here */
	G_OBJECT_CLASS (gst_lumenera_src_parent_class)->finalize (object);
}

static gboolean
gst_lumenera_src_start (GstBaseSrc * bsrc)
{
	LONG flags;
	gboolean ret;
	ULONG entry_count, i;
	float *framerates;

	// Start will open the device but not start it, set_caps starts it, stop should stop and close it (as v4l2src)
	GstLumeneraSrc *src = GST_LU_SRC (bsrc);

	GST_DEBUG_OBJECT (src, "start");

	// Turn on automatic timestamping, if so we do not need to do it manually, BUT there is some evidence that automatic timestamping is laggy
//	gst_base_src_set_do_timestamp(bsrc, TRUE);

	// read lib version (for informational purposes only)
    LUCAM_VERSION versionInfo;
    GST_INFO_OBJECT (src, "lumenera Library Ver 0x%8.8X\n",    versionInfo.api);


	// open first usable device
	GST_DEBUG_OBJECT (src, "LucamCameraOpen");
	src->hCam = LucamCameraOpen(1);

	// display error when no camera has been found
	if(!src->hCam)
	{
		GST_ERROR_OBJECT (src, "No lumenera device found.");
		goto fail;
	}

	// NOTE:
	// from now on, the "hCam" handle can be used to access the camera board.
	// use is_ExitCamera to end the usage
	src->cameraPresent = TRUE;

	// Choose the number of taps, do this before anything else
	GST_DEBUG_OBJECT (src, "LucamSetProperty TAP_CONFIGURATION_DUAL");
	LucamSetProperty(src->hCam, LUCAM_PROP_TAP_CONFIGURATION, TAP_CONFIGURATION_DUAL, LUCAM_PROP_FLAG_USE);

	// Can now get the possible frame rates and choose one
	entry_count = LucamEnumAvailableFrameRates(src->hCam, 0, NULL);  // Call to get entry_count
	framerates = (float *)malloc(entry_count * sizeof(float));
	LucamEnumAvailableFrameRates(src->hCam, entry_count, framerates);   // Call to get framerates
	for (i=0; i<entry_count; i++){
		GST_DEBUG_OBJECT (src, "Possible framerate: %f", framerates[i]);
	}
	// Choose the last frame rate, which will be the highest one, for DUAL tap we expect 26.785578 fps
	src->maxframerate = framerates[entry_count-1];

	// Get and report the range of exposure values
	{
		float min, max, default_val;
		LONG flags;

		LucamPropertyRange(src->hCam, LUCAM_PROP_EXPOSURE, &min, &max, &default_val, &flags);
		GST_DEBUG_OBJECT (src, "Possible exposures: %f to %f, default %f (%d)", min, max, default_val, flags);

		LucamPropertyRange(src->hCam, LUCAM_PROP_GAIN, &min, &max, &default_val, &flags);
		GST_DEBUG_OBJECT (src, "Possible gains: %f to %f, default %f (%d)", min, max, default_val, flags);
		src->cam_min_gain = min;
		src->cam_max_gain = max;

		LucamPropertyRange(src->hCam, LUCAM_PROP_GAIN_RED, &min, &max, &default_val, &flags);
		GST_DEBUG_OBJECT (src, "Possible rgains: %f to %f, default %f (%d)", min, max, default_val, flags);

		LucamPropertyRange(src->hCam, LUCAM_PROP_GAIN_GREEN1, &min, &max, &default_val, &flags);
		GST_DEBUG_OBJECT (src, "Possible ggains: %f to %f, default %f (%d)", min, max, default_val, flags);

		LucamPropertyRange(src->hCam, LUCAM_PROP_GAIN_BLUE, &min, &max, &default_val, &flags);
		GST_DEBUG_OBJECT (src, "Possible bgains: %f to %f, default %f (%d)", min, max, default_val, flags);
	}

	// Try to set subsample or binning
//	LUEXECANDCHECK(LucamGetFormat(src->hCam, &(src->frameFormat), &(src->framerate)));
//	src->frameFormat.binningX = 2;
//	src->frameFormat.binningY = 2;
//	src->frameFormat.flagsX = LUCAM_FRAME_FORMAT_FLAGS_BINNING;
//	src->frameFormat.flagsY = LUCAM_FRAME_FORMAT_FLAGS_BINNING;
//	src->frameFormat.subSampleX = 2;
//	src->frameFormat.subSampleY = 2;
//	src->frameFormat.flagsX = 0;
//	src->frameFormat.flagsY = 0;
//	LUEXECANDCHECK(LucamSetFormat(src->hCam, &(src->frameFormat), src->framerate));

	// Get information about the camera sensor and image
	GST_DEBUG_OBJECT (src, "LucamGetVideoImageFormat");
	LUEXECANDCHECK(LucamGetVideoImageFormat (src->hCam, &(src->imageFormat)));
	GST_DEBUG_OBJECT (src, "imageFormat: w %d h%d ImageSize %d", src->imageFormat.Width, src->imageFormat.Height, src->imageFormat.ImageSize);
	LUEXECANDCHECK(LucamGetFormat(src->hCam, &(src->frameFormat), &(src->framerate)));
	GST_DEBUG_OBJECT (src, "frameFormat: w %d h %d subX %d subY %d binX %d binY %d",
			src->frameFormat.width, src->frameFormat.height,
			src->frameFormat.subSampleX, src->frameFormat.subSampleY,
			src->frameFormat.binningX, src->frameFormat.binningY);
	GST_DEBUG_OBJECT (src, "framerate: %f", src->framerate);


// Try to set a 16 bit format, this crashes with a seg fault when we try and START_STREAMING a few lines below
//	src->frameFormat.pixelFormat = LUCAM_PF_16;
//	LUEXECANDCHECK(LucamSetFormat(src->hCam, &(src->frameFormat), src->framerate));
//	LUEXECANDCHECK(LucamGetVideoImageFormat (src->hCam, &(src->imageFormat)));
//	LUEXECANDCHECK(LucamGetFormat(src->hCam, &(src->frameFormat), &(src->framerate)));

	// Output the frame format for info:
	switch(src->frameFormat.pixelFormat){
	case LUCAM_PF_8:
		GST_DEBUG_OBJECT (src, "Frame Format: LUCAM_PF_8");
		break;
	case LUCAM_PF_16:
		GST_DEBUG_OBJECT (src, "Frame Format: LUCAM_PF_16");
		break;
	case LUCAM_PF_24:
		GST_DEBUG_OBJECT (src, "Frame Format: LUCAM_PF_24");
		break;
	default:
		GST_DEBUG_OBJECT (src, "Frame Format: %d", src->frameFormat.pixelFormat);
		break;
	}

	// Check the pixel format
	src->nBitsPerPixel = 24;   // some sensible default
	switch(src->imageFormat.PixelFormat){
	case LUCAM_PF_8:
		// THIS IS THE CURRENT WORKING CASE !!!!!!!!!!!!!!!!!!!!!!!!
		GST_DEBUG_OBJECT (src, "Pixel Format: LUCAM_PF_8");
		src->nBitsPerPixel = 24;
		break;
	case LUCAM_PF_16:
		GST_DEBUG_OBJECT (src, "Pixel Format: LUCAM_PF_16");
		src->nBitsPerPixel = 48;
		break;
	case LUCAM_PF_24:
		GST_DEBUG_OBJECT (src, "Pixel Format: LUCAM_PF_24");
		src->nBitsPerPixel = 24;
		break;
	default:
		GST_DEBUG_OBJECT (src, "Pixel Format: %d", src->imageFormat.PixelFormat);
		src->nBitsPerPixel = 24;
		break;
	}

	// Setup the GPO frame pulse (default on GPO3)
	GST_DEBUG_OBJECT (src, "LucamGpoSelect 0");
	LucamGpoSelect(src->hCam, (BYTE)0x0);  // Set all GPIO to default
	ret = LucamGpioConfigure(src->hCam, (BYTE)0x255);   // Set GPIO to outputs
	GST_DEBUG_OBJECT (src, "LucamGpioConfigure returned: %d", ret);

	GST_DEBUG_OBJECT (src, "LucamStreamVideoControl START_STREAMING");
	LucamStreamVideoControl(src->hCam, START_STREAMING ,NULL);
	usleep(500 * 1000);

	LucamOneShotAutoWhiteBalance(src->hCam, 0, 0, src->imageFormat.Width, src->imageFormat.Height);
	usleep(500*1000);
	LucamDigitalWhiteBalance(src->hCam, 0, 0, src->imageFormat.Width, src->imageFormat.Height);
	usleep(500*1000);

	GST_DEBUG_OBJECT (src, "LucamStreamVideoControl STOP_STREAMING");
	LucamStreamVideoControl(src->hCam, STOP_STREAMING, NULL);

	// We will use the the full sensor
	src->nWidth = src->imageFormat.Width;
	src->nHeight = src->imageFormat.Height;
	src->nPitch = src->imageFormat.Width * 3;

	src->nBytesPerPixel = (src->nBitsPerPixel+1)/8;
	src->nImageSize = src->nWidth * src->nHeight * src->nBytesPerPixel;
	GST_DEBUG_OBJECT (src, "Image is %d x %d, pitch %d, bpp %d, Bpp %d", src->nWidth, src->nHeight, src->nPitch, src->nBitsPerPixel, src->nBytesPerPixel);

	gst_lumenera_set_camera_exposure(src, LU_UPDATE_CAMERA);
	LucamSetProperty(src->hCam, LUCAM_PROP_GAIN, src->gain, LUCAM_PROP_FLAG_USE);

	LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_RED, src->rgain, LUCAM_PROP_FLAG_USE);
	LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN1, src->ggain, LUCAM_PROP_FLAG_USE);
	LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_GREEN2, src->ggain, LUCAM_PROP_FLAG_USE);
	LucamSetProperty(src->hCam, LUCAM_PROP_GAIN_BLUE, src->bgain, LUCAM_PROP_FLAG_USE);
	GST_DEBUG_OBJECT (src, "Set Gains R %f G %f B %f", src->rgain, src->ggain, src->bgain);

	//	gst_lumenera_set_camera_binning(src); // Binning/subsample mode?
//	is_SetRopEffect(src->hCam, IS_SET_ROP_MIRROR_LEFTRIGHT, src->hflip, 0);
//	is_SetRopEffect(src->hCam, IS_SET_ROP_MIRROR_UPDOWN, src->vflip, 0);
//	gst_lumenera_set_camera_whitebalance(src);

    // Set params for Bayer conversion
	src->conversionParams.CorrectionMatrix = LUCAM_CM_NONE;
    src->conversionParams.DemosaicMethod = LUCAM_DM_FAST;
    src->conversionParams.UseColorGainsOverWb = TRUE;
    src->conversionParams.Size = sizeof(LUCAM_CONVERSION_PARAMS);
//    LucamGetProperty(src->hCam, LUCAM_PROP_DIGITAL_GAIN_BLUE,  &src->conversionParams.DigitalGainBlue, &flags);
//    LucamGetProperty(src->hCam, LUCAM_PROP_DIGITAL_GAIN_GREEN, &src->conversionParams.DigitalGainGreen, &flags);
//    LucamGetProperty(src->hCam, LUCAM_PROP_DIGITAL_GAIN_RED,   &src->conversionParams.DigitalGainRed, &flags);
    src->conversionParams.DigitalGainRed=1;
    src->conversionParams.DigitalGainGreen=1;
    src->conversionParams.DigitalGainBlue=1;
    src->conversionParams.FlipX = FALSE;
    src->conversionParams.FlipY = FALSE;
    //LucamGetProperty(src->hCam, LUCAM_PROP_HUE, &src->conversionParams.Hue, &flags);
    //LucamGetProperty(src->hCam, LUCAM_PROP_SATURATION, &src->conversionParams.Saturation, &flags);
    src->conversionParams.Hue=0;
    src->conversionParams.Saturation=1;

	return TRUE;

	fail:
	if (src->hCam) {
		LucamCameraClose(src->hCam);
		src->hCam = 0;
	}

	return FALSE;
}

static gboolean
gst_lumenera_src_stop (GstBaseSrc * bsrc)
{
	// Start will open the device but not start it, set_caps starts it, stop should stop and close it (as v4l2src)

	GstLumeneraSrc *src = GST_LU_SRC (bsrc);

	GST_DEBUG_OBJECT (src, "stop");
	LUEXECANDCHECK(LucamRemoveStreamingCallback(src->hCam, src->callbackID));
	GST_DEBUG_OBJECT (src, "LucamStreamVideoControl STOP_STREAMING");
	LUEXECANDCHECK(LucamStreamVideoControl(src->hCam, STOP_STREAMING, NULL));
	GST_DEBUG_OBJECT (src, "LucamCameraClose");
	LUEXECANDCHECK(LucamCameraClose(src->hCam));

	gst_lumenera_src_reset (src);

	return TRUE;
}

static GstCaps *
gst_lumenera_src_get_caps (GstBaseSrc * bsrc, GstCaps * filter)
{
	GstLumeneraSrc *src = GST_LU_SRC (bsrc);
	GstCaps *caps;

  if (src->hCam == 0) {
    caps = gst_pad_get_pad_template_caps (GST_BASE_SRC_PAD (src));
  } else {
    GstVideoInfo vinfo;

    // Create video info 
    gst_video_info_init (&vinfo);

    vinfo.width = src->nWidth;
    vinfo.height = src->nHeight;

   	vinfo.fps_n = 0;  vinfo.fps_d = 1;  // Frames per second fraction n/d, 0/1 indicates a frame rate may vary
    vinfo.interlace_mode = GST_VIDEO_INTERLACE_MODE_PROGRESSIVE;

    vinfo.finfo = gst_video_format_get_info (DEFAULT_LU_VIDEO_FORMAT);

    // cannot do this for variable frame rate
    //src->duration = gst_util_uint64_scale_int (GST_SECOND, vinfo.fps_d, vinfo.fps_n); // NB n and d are wrong way round to invert the fps into a duration.

    caps = gst_video_info_to_caps (&vinfo);

    // We can supply our max frame rate, but not sure how to do it or what effect it will have
    // 1st attempt to set max-framerate in the caps
//    GstStructure *structure = gst_caps_get_structure (caps, 0);
//    GValue *val=NULL;
//    g_value_set_double(val, src->maxframerate);
//    gst_structure_set_value (structure, "max-framerate", val);
  }

	GST_DEBUG_OBJECT (src, "The caps are %" GST_PTR_FORMAT, caps);

	if (filter) {
		GstCaps *tmp = gst_caps_intersect (caps, filter);
		gst_caps_unref (caps);
		caps = tmp;

		GST_DEBUG_OBJECT (src, "The caps after filtering are %" GST_PTR_FORMAT, caps);
	}

	return caps;
}

//
// Called when an image is received from the camera image stream
//
VOID imageCallback(VOID *pContext, BYTE *pData, ULONG dataLength)
{
	GstLumeneraSrc *src = (GstLumeneraSrc *)pContext;

	// Consumer of this object still needs the rgb image?
	// Drop this frame then
	if (!src->rgbImageOwnerIsProducer) {
	return;
	}

	//GST_DEBUG_OBJECT(src, "imageCallback called.");

	LucamConvertFrameToRgb24Ex(src->hCam, src->rgbImage, pData, &(src->imageFormat), &(src->conversionParams));
	//memset(src->rgbImage, 100, src->nHeight*src->nWidth*src->nBytesPerPixel);  // TEST line to see if LucamConvertFrameToRgb24Ex was taking a lot of time

	// Transfer ownership to consumer
	src->rgbImageOwnerIsProducer = FALSE;
}

static gboolean
gst_lumenera_src_set_caps (GstBaseSrc * bsrc, GstCaps * caps)
{
	// Start will open the device but not start it, set_caps starts it, stop should stop and close it (as v4l2src)

	GstLumeneraSrc *src = GST_LU_SRC (bsrc);
	GstVideoInfo vinfo;
	//GstStructure *s = gst_caps_get_structure (caps, 0);

	GST_DEBUG_OBJECT (src, "The caps being set are %" GST_PTR_FORMAT, caps);

	gst_video_info_from_caps (&vinfo, caps);

	if (GST_VIDEO_INFO_FORMAT (&vinfo) != GST_VIDEO_FORMAT_UNKNOWN) {
		g_assert (src->hCam != 0);
		//  src->vrm_stride = get_pitch (src->device);  // wait for image to arrive for this
		src->gst_stride = GST_VIDEO_INFO_COMP_STRIDE (&vinfo, 0);
		src->nHeight = vinfo.height;
	} else {
		goto unsupported_caps;
	}

	// TODO What should this be? Does not make any difference, does not help with mpeg2 mux container
//	gst_base_src_set_blocksize(bsrc, src->gst_stride * src->nHeight);
//	GST_DEBUG_OBJECT (src, "Buffer block size is %d bytes", gst_base_src_get_blocksize(bsrc));

	src->rgbImage = (unsigned char *)malloc(src->nWidth * src->nHeight * (src->nBytesPerPixel) * sizeof(unsigned char));

	// start freerun/continuous capture
    src->callbackID = LucamAddStreamingCallback(src->hCam, imageCallback,  src);
	GST_DEBUG_OBJECT (src, "LucamStreamVideoControl START_STREAMING");
    LUEXECANDCHECK(LucamStreamVideoControl(src->hCam, START_STREAMING, NULL));

	src->acq_started = TRUE;

	return TRUE;

	unsupported_caps:
	GST_ERROR_OBJECT (src, "Unsupported caps: %" GST_PTR_FORMAT, caps);
	return FALSE;
}

//  This can override the push class create fn, it is the same as fill above but it forces the creation of a buffer here to copy into.
#ifdef OVERRIDE_CREATE
static GstFlowReturn
gst_lumenera_src_create (GstPushSrc * psrc, GstBuffer ** buf)
{
	GstLumeneraSrc *src = GST_LU_SRC (psrc);
	GstMapInfo minfo;

	// lock next (raw) image for read access, convert it to the desired
	// format and unlock it again, so that grabbing can go on

	// Wait for the next image to be ready
	//INT nRet = is_WaitEvent(src->hCam, IS_SET_EVENT_FRAME_RECEIVED, 5000);

	// Release ownership to the producer to accept new image
	src->rgbImageOwnerIsProducer = TRUE;
	// Wait for the next image to be ready
//	GST_DEBUG_OBJECT(src, "Wait for image.");
	while (src->rgbImageOwnerIsProducer){
		usleep(500);
	}

//	if(G_LIKELY(nRet == IS_SUCCESS))
	{
		//  successfully returned an image
		// ----------------------------------------------------------

		guint i;

//		LucamGpioWrite(src->hCam, 0);

		// Copy image to buffer in the right way

		// Create a new buffer for the image
		*buf = gst_buffer_new_and_alloc (src->nHeight * src->gst_stride);

		gst_buffer_map (*buf, &minfo, GST_MAP_WRITE);

		// From the grabber source we get 1 progressive frame
		// We expect src->nPitch = src->gst_stride but use separate vars for safety
		//GST_DEBUG_OBJECT(src, "Copy image. %d %d", src->gst_stride, src->nPitch);
		for (i = 0; i < src->nHeight; i++) {
			memcpy (minfo.data + i * src->gst_stride,
					src->rgbImage + i * src->nPitch, src->nPitch);
		}

		gst_buffer_unmap (*buf, &minfo);

		// If we do not use gst_base_src_set_do_timestamp() we need to add timestamps manually
		src->last_frame_time += src->duration;   // Get the timestamp for this frame
		if(!gst_base_src_get_do_timestamp(GST_BASE_SRC(psrc))){
			GST_BUFFER_PTS(*buf) = src->last_frame_time;  // convert ms to ns
			GST_BUFFER_DTS(*buf) = src->last_frame_time;  // convert ms to ns
		}
		GST_BUFFER_DURATION(*buf) = src->duration;
//		GST_DEBUG_OBJECT(src, "pts, dts: %" GST_TIME_FORMAT ", duration: %d ms", GST_TIME_ARGS (src->last_frame_time), GST_TIME_AS_MSECONDS(src->duration));

		// count frames, and send EOS when required frame number is reached
		GST_BUFFER_OFFSET(*buf) = src->n_frames;  // from videotestsrc
		src->n_frames++;
		GST_BUFFER_OFFSET_END(*buf) = src->n_frames;  // from videotestsrc
		if (psrc->parent.num_buffers>0)  // If we were asked for a specific number of buffers, stop when complete
			if (G_UNLIKELY(src->n_frames >= psrc->parent.num_buffers))
				return GST_FLOW_EOS;

		// see, if we had to drop some frames due to data transfer stalls. if so,
		// output a message


//		LucamGpioWrite(src->hCam, 255);


	}

	return GST_FLOW_OK;
}
#endif // OVERRIDE_CREATE


