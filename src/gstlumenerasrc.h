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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GST_LU_SRC_H_
#define _GST_LU_SRC_H_

#include  "lucamapi.h"

#include <gst/base/gstpushsrc.h>

G_BEGIN_DECLS

#define GST_TYPE_LU_SRC   (gst_lumenera_src_get_type())
#define GST_LU_SRC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_LU_SRC,GstLumeneraSrc))
#define GST_LU_SRC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_LU_SRC,GstLumeneraSrcClass))
#define GST_IS_LU_SRC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_LU_SRC))
#define GST_IS_LU_SRC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_LU_SRC))

typedef struct _GstLumeneraSrc GstLumeneraSrc;
typedef struct _GstLumeneraSrcClass GstLumeneraSrcClass;

typedef enum
{
	GST_WB_DISABLED,
	GST_WB_ONESHOT,
	GST_WB_AUTO
} WhiteBalanceType;

struct _GstLumeneraSrc
{
  GstPushSrc base_lumenera_src;

  // device
  HANDLE hCam;  // device handle
  gboolean cameraPresent;
  LUCAM_IMAGE_FORMAT imageFormat;  // device sensor information
  LUCAM_FRAME_FORMAT frameFormat;
  LUCAM_CONVERSION_PARAMS conversionParams;
  LONG callbackID;  //
  volatile gboolean rgbImageOwnerIsProducer;

  int lMemId;  // ID of the allocated memory
  int nWidth;
  int nHeight;
  int nBitsPerPixel;
  int nBytesPerPixel;
  int nPitch;   // Stride in bytes between lines
  int nImageSize;  // Image size in bytes

  unsigned char *rgbImage;

  gint gst_stride;  // Stride/pitch for the GStreamer buffer

  // gst properties
  gfloat exposure;
  gfloat framerate;
  gfloat maxframerate;
  gfloat gain;   // will be 0-100%
  gfloat cam_min_gain, cam_max_gain;  //  min and max settable values for the camera
//  gint blacklevel;
  gfloat rgain;
  gfloat ggain;
  gfloat bgain;
  gint vflip;
  gint hflip;
  WhiteBalanceType whitebalance;

  // stream
  gboolean acq_started;
  gint n_frames;
  gint total_timeouts;
  GstClockTime duration;
  GstClockTime last_frame_time;
};

struct _GstLumeneraSrcClass
{
  GstPushSrcClass base_lumenera_src_class;
};

GType gst_lumenera_src_get_type (void);

G_END_DECLS

#endif
