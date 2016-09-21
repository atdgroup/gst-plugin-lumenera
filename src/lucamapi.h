#ifndef LUMENERA_LUCAMAPI_H
#define LUMENERA_LUCAMAPI_H

/*****************************************************************************
*
* Lumenera Corporation Lucam Camera API
*
* Copyright (c) 2002-2015 Lumenera Corporation. All rights reserved.
*
*****************************************************************************/

#include "lucamTypes.h"

//
// Pixel Formats 
//
#define LUCAM_PF_8                           0
#define LUCAM_PF_16                          1
#define LUCAM_PF_24                          2
#if defined(_WIN32)
#define LUCAM_PF_YUV422                      3
#define LUCAM_PF_COUNT                       4
#define LUCAM_PF_FILTER                      5
#endif
#define LUCAM_PF_32                          6
#define LUCAM_PF_48                          7

//
// Properties
//
#define LUCAM_PROP_BRIGHTNESS                0
#define LUCAM_PROP_CONTRAST                  1
#define LUCAM_PROP_HUE                       2
#define LUCAM_PROP_SATURATION                3
#if defined(_WIN32)
#define LUCAM_PROP_SHARPNESS                 4
#endif
#define LUCAM_PROP_GAMMA                     5

#if defined(_WIN32)
#define LUCAM_PROP_PAN                       16
#define LUCAM_PROP_TILT                      17
#define LUCAM_PROP_ROLL                      18
#define LUCAM_PROP_ZOOM                      19
#endif
#define LUCAM_PROP_EXPOSURE                  20
#define LUCAM_PROP_IRIS                      21
#define LUCAM_PROP_FOCUS                     22

#define LUCAM_PROP_GAIN                      40
#define LUCAM_PROP_GAIN_RED                  41
#define LUCAM_PROP_GAIN_BLUE                 42
#define LUCAM_PROP_GAIN_GREEN1               43
#define LUCAM_PROP_GAIN_GREEN2               44
#if defined(_WIN32)
#define LUCAM_PROP_GAIN_MAGENTA              41
#define LUCAM_PROP_GAIN_CYAN                 42
#define LUCAM_PROP_GAIN_YELLOW1              43
#define LUCAM_PROP_GAIN_YELLOW2              44
#endif

#define LUCAM_PROP_DEMOSAICING_METHOD        64
#define LUCAM_PROP_CORRECTION_MATRIX         65
#define LUCAM_PROP_FLIPPING                  66

#define LUCAM_PROP_DIGITAL_WHITEBALANCE_U    69 // From -100 to 100
#define LUCAM_PROP_DIGITAL_WHITEBALANCE_V    70 // From -100 to 100
#define LUCAM_PROP_DIGITAL_GAIN              71 // From 0 to 2, 1 means a gain of 1.0
#define LUCAM_PROP_DIGITAL_GAIN_RED          72 // From 0 to 2.5, 1 means a gain of 1.0. Relates to GAIN_Y and WHITEBALANCE
#define LUCAM_PROP_DIGITAL_GAIN_GREEN        73 // From 0 to 2.5, 1 means a gain of 1.0. Relates to GAIN_Y and WHITEBALANCE
#define LUCAM_PROP_DIGITAL_GAIN_BLUE         74 // From 0 to 2.5, 1 means a gain of 1.0. Relates to GAIN_Y and WHITEBALANCE

#define LUCAM_PROP_COLOR_FORMAT              80 // Read-only
#define LUCAM_PROP_MAX_WIDTH                 81 // Read-only
#define LUCAM_PROP_MAX_HEIGHT                82 // Read-only
#define LUCAM_PROP_UNIT_WIDTH                83 // Read-only
#define LUCAM_PROP_UNIT_HEIGHT               84 // Read-only

#define LUCAM_PROP_ABS_FOCUS                 85 // Requires the auto lens to be initialized
#if defined(_WIN32)
#define LUCAM_PROP_BLACK_LEVEL               86
#endif

#define LUCAM_PROP_KNEE1_EXPOSURE            96
#define LUCAM_PROP_STILL_KNEE1_EXPOSURE      96
#define LUCAM_PROP_KNEE2_EXPOSURE            97
#define LUCAM_PROP_STILL_KNEE2_EXPOSURE      97
#define LUCAM_PROP_STILL_KNEE3_EXPOSURE      98
#define LUCAM_PROP_VIDEO_KNEE                99
#define LUCAM_PROP_KNEE1_LEVEL               99

#define LUCAM_PROP_THRESHOLD                 101
#define LUCAM_PROP_AUTO_EXP_TARGET           103
#define LUCAM_PROP_TIMESTAMPS                105
#define LUCAM_PROP_SNAPSHOT_CLOCK_SPEED      106 // 0 is the fastest
#if defined(_WIN32)
#define LUCAM_PROP_AUTO_EXP_MAXIMUM          107
#endif
#define LUCAM_PROP_TEMPERATURE               108
#define LUCAM_PROP_TRIGGER                   110
#define LUCAM_PROP_TRIGGER_PIN               LUCAM_PROP_TRIGGER // Alias
#if defined(_WIN32)
#define LUCAM_PROP_FRAME_GATE                112
#endif
#define LUCAM_PROP_EXPOSURE_INTERVAL         113
#if defined(_WIN32)
#define LUCAM_PROP_PWM                       114
#define LUCAM_PROP_MEMORY                    115 // Read-only. Represents number of frames in memory.
#endif
#define LUCAM_PROP_STILL_STROBE_DURATION     116
#if defined(_WIN32)
#define LUCAM_PROP_FAN                       118
#define LUCAM_PROP_SYNC_MODE                 119
#endif
#define LUCAM_PROP_SNAPSHOT_COUNT            120
#if defined(_WIN32)
#define LUCAM_PROP_LSC_X                     121
#define LUCAM_PROP_LSC_Y                     122
#define LUCAM_PROP_AUTO_IRIS_MAX             123
#define LUCAM_PROP_LENS_STABILIZATION        124
#define LUCAM_PROP_VIDEO_TRIGGER             125
#endif
#define LUCAM_PROP_VIDEO_CLOCK_SPEED         126 // 0 is the fastest. Check for read-only flag.
#define LUCAM_PROP_KNEE2_LEVEL               163
#if defined(_WIN32)
#define LUCAM_PROP_THRESHOLD_LOW             165
#define LUCAM_PROP_THRESHOLD_HIGH            166
#define LUCAM_PROP_TEMPERATURE2              167
#define LUCAM_PROP_LIGHT_FREQUENCY           168
#define LUCAM_PROP_LUMINANCE                 169
#define LUCAM_PROP_AUTO_GAIN_MAXIMUM         170
#endif
#define LUCAM_PROP_STROBE_PIN                172
#if defined(_WIN32)
#define LUCAM_PROP_TRIGGER_MODE              173 // See TRIGGER_MODE_* below
#define LUCAM_PROP_FOCAL_LENGTH              174
#define LUCAM_PROP_IRIS_LATENCY              175
#endif
#define LUCAM_PROP_TAP_CONFIGURATION         176
#define LUCAM_PROP_STILL_TAP_CONFIGURATION   177
#if defined(_WIN32)
#define LUCAM_PROP_MAX_FRAME_RATE            184
#endif
#define LUCAM_PROP_JPEG_QUALITY              256

// Property Flags 
#define LUCAM_PROP_FLAG_USE                             0x80000000
#define LUCAM_PROP_FLAG_AUTO                            0x40000000
#define LUCAM_PROP_FLAG_STROBE_FROM_START_OF_EXPOSURE   0x20000000
#define LUCAM_PROP_FLAG_POLARITY                        0x10000000
#define LUCAM_PROP_FLAG_ALTERNATE                       0x00080000 
#define LUCAM_PROP_FLAG_BUSY                            0x00040000
#define LUCAM_PROP_FLAG_UNKNOWN_MAXIMUM                 0x00020000
#define LUCAM_PROP_FLAG_UNKNOWN_MINIMUM                 0x00010000

// Property-specific property flags
#define LUCAM_PROP_FLAG_MEMORY_READBACK                 0x08000000 // For LUCAM_PROP_MEMORY
#define LUCAM_PROP_FLAG_USE_FOR_SNAPSHOTS               0x04000000 // For LUCAM_PROP_IRIS
#define LUCAM_PROP_FLAG_MASTER                          0x40000000 // For LUCAM_PROP_SYNC_MODE
#define LUCAM_PROP_FLAG_LITTLE_ENDIAN                   0x80000000 // For LUCAM_PROP_COLOR_FORMAT
#define LUCAM_PROP_FLAG_READONLY                        0x00010000 // For flags param of GetPropertyRange
#define LUCAM_PROP_FLAG_BACKLASH_COMPENSATION           0x20000000 // LUCAM_PROP_IRIS and LUCAM_PROP_FOCUS

#if defined(_WIN32)
// Property flags for VIDEO_TRIGGER (also uses LUCAM_PROP_FLAG_USE)
#define LUCAM_PROP_FLAG_HW_ENABLE                       0x40000000
#define LUCAM_PROP_FLAG_SW_TRIGGER                      0x00200000 // Self-cleared
#endif

// These flags can be used with  LUCAM_PROP_GAMMA, LUCAM_PROP_BRIGHTNESS, and LUCAM_PROP_CONTRAST
// They are available on specific cameras only.
#define LUCAM_PROP_FLAG_RED                  0x00000001
#define LUCAM_PROP_FLAG_GREEN1               0x00000002
#define LUCAM_PROP_FLAG_GREEN2               0x00000004
#define LUCAM_PROP_FLAG_BLUE                 0x00000008

// 
#define LUCAM_PROP_STILL_EXPOSURE            50
#define LUCAM_PROP_STILL_GAIN                51
#define LUCAM_PROP_STILL_GAIN_RED            52
#define LUCAM_PROP_STILL_GAIN_GREEN1         53
#define LUCAM_PROP_STILL_GAIN_GREEN2         54
#define LUCAM_PROP_STILL_GAIN_BLUE           55
#if defined(_WIN32)
#define LUCAM_PROP_STILL_GAIN_MAGENTA        52
#define LUCAM_PROP_STILL_GAIN_YELLOW1        53
#define LUCAM_PROP_STILL_GAIN_YELLOW2        54
#define LUCAM_PROP_STILL_GAIN_CYAN           55
#endif
#define LUCAM_PROP_STILL_STROBE_DELAY        56
#define LUCAM_PROP_STILL_EXPOSURE_DELAY      100

// For the LUCAM_PROP_COLOR_FORMAT property
#define LUCAM_CF_MONO                         0
#define LUCAM_CF_BAYER_RGGB                   8
#define LUCAM_CF_BAYER_GRBG                   9
#define LUCAM_CF_BAYER_GBRG                  10
#define LUCAM_CF_BAYER_BGGR                  11
#if defined(_WIN32)
#define LUCAM_CF_BAYER_CYYM                  16
#define LUCAM_CF_BAYER_YCMY                  17
#define LUCAM_CF_BAYER_YMCY                  18
#define LUCAM_CF_BAYER_MYYC                  19
#endif

// For the LUCAM_PROP_FLIPPING property
#define LUCAM_PROP_FLIPPING_NONE             0
#define LUCAM_PROP_FLIPPING_X                1
#define LUCAM_PROP_FLIPPING_Y                2
#define LUCAM_PROP_FLIPPING_XY               3

// Tap configurations for LUCAM_PROP_TAP_CONFIGURATION and LUCAM_PROP_STILL_TAP_CONFIGURATION
#define TAP_CONFIGURATION_1X1                0
#define TAP_CONFIGURATION_2X1                1
#define TAP_CONFIGURATION_1X2                2
#define TAP_CONFIGURATION_2X2                4
#define TAP_CONFIGURATION_SINGLE             TAP_CONFIGURATION_1X1
#define TAP_CONFIGURATION_DUAL               TAP_CONFIGURATION_2X1
#define TAP_CONFIGURATION_QUAD               TAP_CONFIGURATION_2X2

#if defined(_WIN32)
// For the LUCAM_PROP_TRIGGER_MODE property.
#define TRIGGER_MODE_NORMAL                  0
#define TRIGGER_MODE_BULB                    1
#endif

//
// Version Information: Camera and Host
// For use with QueryVersion and EnumCameras
//
typedef struct LUCAM_VERSION {
    ULONG firmware;     // Camera firmware version.      Not available with LucamEnumCameras
    ULONG fpga;         // Camera FPGA version.          Not available with LucamEnumCameras
    ULONG api;          // API version (lucamapi.dll, lucamapi.so.*)
    ULONG driver;       // Device driver version.        Not available with LucamEnumCameras
    ULONG serialnumber; // Unique serial number of a camera.
    ULONG cameraid;     // Also known as camera model id. 
} LUCAM_VERSION;


// 
// Frame Format.
//
typedef struct LUCAM_FRAME_FORMAT {
    ULONG xOffset;         // X coordinate on imager of top left corner of subwindow, in pixels
    ULONG yOffset;         // Y coordinate on imager of top left corner of subwindow, in pixels
    ULONG width;           // Width  of subwindow, in pixels
    ULONG height;          // Height of subwindow, in pixls
    ULONG pixelFormat;     // Pixel format LUCAM_PF
    union {
        USHORT subSampleX; // Sub-sample ratio in x direction, in pixels
        USHORT binningX;   // Binning    ratio in x direction, in pixels
    };
    USHORT flagsX;         // LUCAM_FRAME_FORMAT_FLAGS_*
    union {
        USHORT subSampleY; // Sub-sample ratio in y direction, in pixels
        USHORT binningY;   // Binning    ratio in y direction, in pixels
    };
    USHORT flagsY;         // LUCAM_FRAME_FORMAT_FLAGS_*
} LUCAM_FRAME_FORMAT;

#define LUCAM_FRAME_FORMAT_FLAGS_BINNING     0x0001

//
// Snapshot Settings
// See TakeSnapshot, EnableFastFrames, and EnableSynchronousSnapshots.
//
typedef struct LUCAM_SNAPSHOT {
    float exposure;            // Exposure in milliseconds
    float gain;                // Overall gain as a multiplicative factor
    union {
        struct {
            float gainRed;     // Gain for Red pixels as multiplicative factor
            float gainBlue;    // Gain for Blue pixels as multiplicative factor
            float gainGrn1;    // Gain for Green pixels on Red rows as multiplicative factor
            float gainGrn2;    // Gain for Green pixels on Blue rows as multiplicative factor
        };
        struct {  
            float gainMag;     // Gain for Magenta pixels as multiplicative factor
            float gainCyan;    // Gain for Cyan pixels as multiplicative factor
            float gainYel1;    // Gain for Yellow pixels on Magenta rows as multiplicative factor
            float gainYel2;    // Gain for Yellow pixels on Cyan rows as multiplicative factor
        };
    };
    union {
        BOOL  useStrobe;       // For backward compatibility
        ULONG strobeFlags;     // Use LUCAM_PROP_FLAG_USE and/or LUCAM_PROP_FLAG_STROBE_FROM_START_OF_EXPOSURE
    };
    float strobeDelay;         // Time interval from when exposure starts to time the flash is fired in milliseconds
    BOOL  useHwTrigger;        // Wait for hardware trigger
    float timeout;             // Maximum time to wait for hardware trigger prior to returning from function in milliseconds
    LUCAM_FRAME_FORMAT format; // Frame format for data
    ULONG shutterType;
    float exposureDelay;
    union {
        BOOL  bufferlastframe; // Set to TRUE if you want TakeFastFrame to return an already received frame.
        ULONG ulReserved1;
    };
    ULONG ulReserved2;         // Must be set to 0
    float flReserved1;         // Must be set to 0
    float flReserved2;         // Must be set to 0
} LUCAM_SNAPSHOT;


// Video Modes 
#define STOP_STREAMING  0
#define START_STREAMING 1
#define START_DISPLAY   2
#define PAUSE_STREAM    3
#define START_RGBSTREAM 6

#if defined(_WIN32)
// Streaming AVI Modes 
#define STOP_AVI  0
#define START_AVI 1
#define PAUSE_AVI 2

// AVI types 
#define AVI_RAW_LUMENERA    0
#define AVI_STANDARD_24     1
#define AVI_STANDARD_32     2
#define AVI_XVID_24         3
#define AVI_STANDARD_8      4 // For monochrome only
#endif

// For use with LucamConvertFrame* 
typedef struct LUCAM_CONVERSION {
    ULONG DemosaicMethod;     // LUCAM_DM_*
    ULONG CorrectionMatrix;   // LUCAM_CM_*
} LUCAM_CONVERSION;

// Demosaicing Methods
#define LUCAM_DM_NONE                   0
#define LUCAM_DM_FAST                   1
#define LUCAM_DM_HIGH_QUALITY           2
#define LUCAM_DM_HIGHER_QUALITY         3
#define LUCAM_DM_SIMPLE                 8

// Color Correction/Creation Matrices
#define LUCAM_CM_NONE                   0
#define LUCAM_CM_FLUORESCENT            1
#define LUCAM_CM_DAYLIGHT               2
#define LUCAM_CM_INCANDESCENT           3
#define LUCAM_CM_XENON_FLASH            4
#define LUCAM_CM_HALOGEN                5
#if defined(_WIN32)
#define LUCAM_CM_LED                    6
#endif
#define LUCAM_CM_IDENTITY               14
#define LUCAM_CM_CUSTOM                 15


// Electronic shutter types
#define LUCAM_SHUTTER_TYPE_GLOBAL         0
#define LUCAM_SHUTTER_TYPE_ROLLING        1


// "Extern" means the interface used by the camera to 
// talk to the host computer.
// See QueryExternInterface, and SelectExternInterface
#define LUCAM_EXTERN_INTERFACE_USB1       1
#define LUCAM_EXTERN_INTERFACE_USB2       2
#define LUCAM_EXTERN_INTERFACE_USB3       3
#define LUCAM_EXTERN_INTERFACE_GIGEVISION 4

//
// The RGB format for images produced by functions such as
// AddRgbPreviewCallback and ConvertFrameToRgb24
//
#define LUCAM_RGB_FORMAT_RGB              0    // Standard R,G,B
#define LUCAM_RGB_FORMAT_BMP              1    // B,G,R, row-inverted, in the format of a Windows BMP.

#if defined(_WIN32)
#define LUCAM_API_RGB24_FORMAT  LUCAM_RGB_FORMAT_BMP
#define LUCAM_API_RGB32_FORMAT  LUCAM_RGB_FORMAT_BMP
#define LUCAM_API_RGB48_FORMAT  LUCAM_RGB_FORMAT_BMP
#elif defined(__linux__)
#define LUCAM_API_RGB24_FORMAT  LUCAM_RGB_FORMAT_RGB
#define LUCAM_API_RGB32_FORMAT  LUCAM_RGB_FORMAT_RGB
#define LUCAM_API_RGB48_FORMAT  LUCAM_RGB_FORMAT_RGB
#else
#error "Unsupported platform"
#endif

//
// Lumenera API Functions
//

LUCAM_API LONG   LUCAM_EXPORT LucamNumCameras(void);
LUCAM_API LONG   LUCAM_EXPORT LucamEnumCameras(LUCAM_VERSION *pVersionsArray, ULONG arrayCount);

LUCAM_API HANDLE LUCAM_EXPORT LucamCameraOpen (ULONG index);
LUCAM_API BOOL   LUCAM_EXPORT LucamCameraClose(HANDLE hCamera);
LUCAM_API BOOL   LUCAM_EXPORT LucamCameraReset(HANDLE hCamera);

// Querying error information
LUCAM_API ULONG LUCAM_EXPORT LucamGetLastError(void);
LUCAM_API ULONG LUCAM_EXPORT LucamGetLastErrorForCamera(HANDLE hCamera);

LUCAM_API BOOL LUCAM_EXPORT LucamQueryVersion(HANDLE hCamera, LUCAM_VERSION *pVersion);
LUCAM_API BOOL LUCAM_EXPORT LucamQueryExternInterface(HANDLE hCamera, ULONG *pExternInterface);
LUCAM_API BOOL LUCAM_EXPORT LucamGetCameraId(HANDLE hCamera, ULONG *pId);
LUCAM_API BOOL LUCAM_EXPORT LucamGetHardwareRevision(HANDLE hCamera, ULONG *pRevision);

LUCAM_API BOOL LUCAM_EXPORT LucamGetProperty(HANDLE hCamera, ULONG propertyId, float *pValue, LONG *pFlags);
LUCAM_API BOOL LUCAM_EXPORT LucamSetProperty(HANDLE hCamera, ULONG propertyId, float   value, LONG   flags);
LUCAM_API BOOL LUCAM_EXPORT LucamPropertyRange(HANDLE hCamera, ULONG propertyId, float *pMin, float *pMax, float *pDefault, LONG *pFlags);

#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamDisplayPropertyPage   (HANDLE hCamera, HWND hParentWnd);
LUCAM_API BOOL LUCAM_EXPORT LucamDisplayVideoFormatPage(HANDLE hCamera, HWND hParentWnd);

LUCAM_API BOOL LUCAM_EXPORT LucamQueryDisplayFrameRate(HANDLE hCamera, float *pValue);

#ifdef __cplusplus
LUCAM_API BOOL LUCAM_EXPORT LucamCreateDisplayWindow(HANDLE hCamera, LPCSTR lpTitle = NULL, DWORD dwStyle = WS_OVERLAPPED|WS_MINIMIZEBOX|WS_CAPTION|WS_SYSMENU|WS_VISIBLE, int x = 0, int y = 0, int width = 0, int height = 0, HWND hParent = NULL, HMENU childId = NULL);
LUCAM_API BOOL LUCAM_EXPORT LucamAdjustDisplayWindow(HANDLE hCamera, LPCSTR lpTitle = NULL, int x = 0, int y = 0, int width = 0, int height = 0);
#else
LUCAM_API BOOL LUCAM_EXPORT LucamCreateDisplayWindow(HANDLE hCamera, LPCSTR lpTitle, DWORD dwStyle, int x, int y, int width, int height, HWND hParent, HMENU childId);
LUCAM_API BOOL LUCAM_EXPORT LucamAdjustDisplayWindow(HANDLE hCamera, LPCSTR lpTitle, int x, int y, int width, int height);
#endif
LUCAM_API BOOL LUCAM_EXPORT LucamDestroyDisplayWindow(HANDLE hCamera);

LUCAM_API BOOL LUCAM_EXPORT LucamReadRegister (HANDLE hCamera, LONG address, LONG numReg, LONG *pValue);
LUCAM_API BOOL LUCAM_EXPORT LucamWriteRegister(HANDLE hCamera, LONG address, LONG numReg, LONG *pValue);
#endif

LUCAM_API BOOL LUCAM_EXPORT LucamSetFormat(HANDLE hCamera, LUCAM_FRAME_FORMAT *pFormat, float   frameRate);
LUCAM_API BOOL LUCAM_EXPORT LucamGetFormat(HANDLE hCamera, LUCAM_FRAME_FORMAT *pFormat, float *pFrameRate);

LUCAM_API ULONG LUCAM_EXPORT LucamEnumAvailableFrameRates(HANDLE hCamera, ULONG entryCount, float *pAvailableFrameRates);

LUCAM_API BOOL LUCAM_EXPORT LucamStreamVideoControl   (HANDLE hCamera, ULONG controlType, HWND hWnd);
#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamStreamVideoControlAVI(HANDLE hCamera, ULONG controlType, LPCWSTR pFileName, HWND hWnd);
#endif

LUCAM_API BOOL LUCAM_EXPORT LucamTakeVideo  (HANDLE hCamera, LONG numFrames, BYTE *pData);
LUCAM_API BOOL LUCAM_EXPORT LucamTakeVideoEx(HANDLE hCamera, BYTE *pData, ULONG *pLength, ULONG timeout);
LUCAM_API BOOL LUCAM_EXPORT LucamCancelTakeVideo(HANDLE hCamera);

//
// TakeSnapshot is a convenience function, wrapping the following sequence:
//   BOOL rc = LucamEnableFastFrames(hCamera, pSettings);
//   if (FALSE == rc) { return rc; }
//   rc = LucamTakeFastFrame(hCamera, pData);
//   LucamDisableFastFrames(hCamera);
//   return rc;
//
LUCAM_API BOOL LUCAM_EXPORT LucamTakeSnapshot(HANDLE hCamera, LUCAM_SNAPSHOT *pSettings, BYTE *pData);

LUCAM_API LUCAM_DEPRECATED BOOL LUCAM_EXPORT LucamSaveImage(  ULONG width, ULONG height, ULONG pixelFormat, BYTE *pData, const CHAR  *pFilename);
LUCAM_API BOOL LUCAM_EXPORT LucamSaveImageEx(HANDLE hCamera,  ULONG width, ULONG height, ULONG pixelFormat, BYTE *pData, const CHAR  *pFilename);
LUCAM_API LUCAM_DEPRECATED BOOL LUCAM_EXPORT LucamSaveImageW( ULONG width, ULONG height, ULONG pixelFormat, BYTE *pData, const WCHAR *pFilename);
LUCAM_API BOOL LUCAM_EXPORT LucamSaveImageWEx(HANDLE hCamera, ULONG width, ULONG height, ULONG pixelFormat, BYTE *pData, const WCHAR *pFilename);

LUCAM_API LONG LUCAM_EXPORT LucamAddStreamingCallback(HANDLE hCamera, VOID (LUCAM_EXPORT *VideoFilter)(VOID *pContext, BYTE *pData, ULONG dataLength), VOID *pCBContext);
LUCAM_API BOOL LUCAM_EXPORT LucamRemoveStreamingCallback(HANDLE hCamera, LONG callbackId);

LUCAM_API LONG LUCAM_EXPORT LucamAddRgbPreviewCallback(HANDLE hCamera, VOID (LUCAM_EXPORT *RgbVideoFilter)(VOID *pContext, BYTE *pData, ULONG dataLength, ULONG unused), VOID *pContext, ULONG rgbPixelFormat);
LUCAM_API BOOL LUCAM_EXPORT LucamRemoveRgbPreviewCallback(HANDLE hCamera, LONG callbackId);
LUCAM_API BOOL LUCAM_EXPORT LucamQueryRgbPreviewPixelFormat(HANDLE hCamera, ULONG *pRgbPixelFormat);

LUCAM_API LONG LUCAM_EXPORT LucamAddSnapshotCallback(HANDLE hCamera, VOID (LUCAM_EXPORT *SnapshotCallback)(VOID *pContext, BYTE *pData, ULONG dataLength), VOID *pCBContext);
LUCAM_API BOOL LUCAM_EXPORT LucamRemoveSnapshotCallback(HANDLE hCamera, LONG callbackId);

LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToRgb24      (HANDLE hCamera, BYTE   *pDest, BYTE   *pSrc, ULONG width, ULONG height, ULONG pixelFormat, LUCAM_CONVERSION *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToRgb32      (HANDLE hCamera, BYTE   *pDest, BYTE   *pSrc, ULONG width, ULONG height, ULONG pixelFormat, LUCAM_CONVERSION *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToRgb48      (HANDLE hCamera, USHORT *pDest, USHORT *pSrc, ULONG width, ULONG height, ULONG pixelFormat, LUCAM_CONVERSION *pParams);
#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToGreyscale8 (HANDLE hCamera, BYTE   *pDest, BYTE   *pSrc, ULONG width, ULONG height, ULONG pixelFormat, LUCAM_CONVERSION *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToGreyscale16(HANDLE hCamera, USHORT *pDest, USHORT *pSrc, ULONG width, ULONG height, ULONG pixelFormat, LUCAM_CONVERSION *pParams);
LUCAM_API VOID LUCAM_EXPORT LucamConvertBmp24ToRgb24(UCHAR *pFrame, ULONG width, ULONG height);

LUCAM_API BOOL LUCAM_EXPORT LucamConvertRawAVIToStdVideo(HANDLE hCamera, const WCHAR *pOutputFileName, const WCHAR *pInputFileName, ULONG outputType);

LUCAM_API HANDLE LUCAM_EXPORT LucamPreviewAVIOpen(const WCHAR *pFileName);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIClose(HANDLE hAVI);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIControl(HANDLE hAVI, ULONG previewControlType, HWND previewWindow);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIGetDuration(HANDLE hAVI, LONGLONG *pDurationMinutes, LONGLONG *pDurationSeconds, LONGLONG *pDurationMilliseconds, LONGLONG *pDurationMicroSeconds);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIGetFrameCount(HANDLE hAVI, LONGLONG *pFrameCount);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIGetFrameRate (HANDLE hAVI, float *pFrameRate);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVISetPositionFrame(HANDLE hAVI, LONGLONG  pPositionFrame);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIGetPositionFrame(HANDLE hAVI, LONGLONG *pPositionFrame);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVISetPositionTime (HANDLE hAVI, LONGLONG   positionMinutes, LONGLONG   positionSeconds, LONGLONG   positionMilliSeconds, LONGLONG   positionMicroSeconds);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIGetPositionTime (HANDLE hAVI, LONGLONG *pPositionMinutes, LONGLONG *pPositionSeconds, LONGLONG *pPositionMilliSeconds, LONGLONG *pPositionMicroSeconds);
LUCAM_API BOOL   LUCAM_EXPORT LucamPreviewAVIGetFormat(HANDLE hAVI, LONG *width, LONG *height, LONG *fileType, LONG *bitDepth);
#endif

LUCAM_API BOOL LUCAM_EXPORT LucamSetupCustomMatrix(HANDLE hCamera, float *pMatrix);
LUCAM_API BOOL LUCAM_EXPORT LucamGetCurrentMatrix (HANDLE hCamera, float *pMatrix);

LUCAM_API BOOL LUCAM_EXPORT LucamEnableFastFrames      (HANDLE hCamera, LUCAM_SNAPSHOT *pSettings);
LUCAM_API BOOL LUCAM_EXPORT LucamTakeFastFrame         (HANDLE hCamera, BYTE *pData);
LUCAM_API BOOL LUCAM_EXPORT LucamForceTakeFastFrame    (HANDLE hCamera, BYTE *pData);
LUCAM_API BOOL LUCAM_EXPORT LucamTakeFastFrameNoTrigger(HANDLE hCamera, BYTE *pData);
LUCAM_API BOOL LUCAM_EXPORT LucamSetTriggerMode        (HANDLE hCamera, BOOL useHwTrigger);
LUCAM_API BOOL LUCAM_EXPORT LucamTriggerFastFrame      (HANDLE hCamera);
LUCAM_API BOOL LUCAM_EXPORT LucamCancelTakeFastFrame   (HANDLE hCamera);
LUCAM_API BOOL LUCAM_EXPORT LucamDisableFastFrames     (HANDLE hCamera);

#if defined(_WIN32)
LUCAM_API HANDLE LUCAM_EXPORT LucamEnableSynchronousSnapshots (ULONG numberOfCameras, HANDLE *phCameras, LUCAM_SNAPSHOT **ppSettings);
LUCAM_API BOOL   LUCAM_EXPORT LucamTakeSynchronousSnapshots   (HANDLE syncSnapsHandle, BYTE **ppBuffers);
LUCAM_API BOOL   LUCAM_EXPORT LucamDisableSynchronousSnapshots(HANDLE syncSnapsHandle);
#endif

LUCAM_API BOOL LUCAM_EXPORT LucamGpioRead     (HANDLE hCamera, BYTE *pGpoValues, BYTE *pGpiValues);
LUCAM_API BOOL LUCAM_EXPORT LucamGpioWrite    (HANDLE hCamera, BYTE   gpoValues);
LUCAM_API BOOL LUCAM_EXPORT LucamGpoSelect    (HANDLE hCamera, BYTE gpoEnable);    // Selects between GPO output or alternate function
LUCAM_API BOOL LUCAM_EXPORT LucamGpioConfigure(HANDLE hCamera, BYTE enableOutput); // Enables output drive on a pin.

LUCAM_API BOOL LUCAM_EXPORT LucamOneShotAutoExposure      (HANDLE hCamera, UCHAR target, ULONG startX, ULONG startY, ULONG width, ULONG height);
#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamOneShotAutoExposureEx    (HANDLE hCamera, UCHAR target, ULONG startX, ULONG startY, ULONG width, ULONG height, float lightingPeriod /* ms, should be 8.333 in North America*/);
#endif
LUCAM_API BOOL LUCAM_EXPORT LucamOneShotAutoGain          (HANDLE hCamera, UCHAR target, ULONG startX, ULONG startY, ULONG width, ULONG height);
LUCAM_API BOOL LUCAM_EXPORT LucamOneShotAutoWhiteBalance  (HANDLE hCamera,               ULONG startX, ULONG startY, ULONG width, ULONG height);
#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamOneShotAutoWhiteBalanceEx(HANDLE hCamera, float redOverGreen, float blueOverGreen,
                                                                                         ULONG startX, ULONG startY, ULONG width, ULONG height);
#endif

LUCAM_API BOOL LUCAM_EXPORT LucamDigitalWhiteBalance  (HANDLE hCamera, ULONG startX, ULONG startY, ULONG width, ULONG height);
#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamDigitalWhiteBalanceEx(HANDLE hCamera, float redOverGreen, float blueOverGreen, ULONG startX, ULONG startY, ULONG width, ULONG height);
LUCAM_API BOOL LUCAM_EXPORT LucamAdjustWhiteBalanceFromSnapshot(HANDLE hCamera, LUCAM_SNAPSHOT *pSettings, BYTE *pData, float redOverGreen, float blueOverGreen, ULONG startX, ULONG startY, ULONG width, ULONG height);
LUCAM_API BOOL LUCAM_EXPORT LucamOneShotAutoIris(HANDLE hCamera, UCHAR target, ULONG startX, ULONG startY, ULONG width, ULONG height);
LUCAM_API BOOL LUCAM_EXPORT LucamContinuousAutoExposureEnable(HANDLE hCamera, UCHAR target, ULONG startX, ULONG startY, ULONG width, ULONG height, float lightingPeriod /* ms, should be 8.333 in North America */);
LUCAM_API BOOL LUCAM_EXPORT LucamContinuousAutoExposureDisable(HANDLE hCamera);

LUCAM_API BOOL LUCAM_EXPORT LucamAutoFocusStart(HANDLE hCamera, ULONG startX, ULONG startY, ULONG width, ULONG height, float putZeroThere1, float putZeroThere2, float putZeroThere3, BOOL (LUCAM_EXPORT * ProgressCallback)(void *context, float percentageCompleted), void *contextForCallback);
LUCAM_API BOOL LUCAM_EXPORT LucamAutoFocusWait (HANDLE hCamera, DWORD timeout);
LUCAM_API BOOL LUCAM_EXPORT LucamAutoFocusStop (HANDLE hCamera);
LUCAM_API BOOL LUCAM_EXPORT LucamAutoFocusQueryProgress(HANDLE hCamera, float *pPercentageCompleted);
LUCAM_API BOOL LUCAM_EXPORT LucamInitAutoLens(HANDLE hCamera, BOOL force);

// Lookup table
LUCAM_API BOOL LUCAM_EXPORT LucamSetup8bitsLUT     (HANDLE hCamera, UCHAR *pLut, ULONG length);   // Length must be 0 or 256
LUCAM_API BOOL LUCAM_EXPORT LucamSetup8bitsColorLUT(HANDLE hCamera, UCHAR *pLut, ULONG length, BOOL applyOnRed, BOOL applyOnGreen1, BOOL applyOnGreen2 , BOOL applyOnBlue);   // Length must be 0 or 256

// RS-232
LUCAM_API int LUCAM_EXPORT LucamRs232Transmit(HANDLE hCamera, char *pData, int length);
LUCAM_API int LUCAM_EXPORT LucamRs232Receive (HANDLE hCamera, char *pData, int maxLength);
LUCAM_API BOOL LUCAM_EXPORT LucamAddRs232Callback(HANDLE hCamera, void (LUCAM_EXPORT * callback)(void *), void *context);
LUCAM_API VOID LUCAM_EXPORT LucamRemoveRs232Callback(HANDLE hCamera);
#endif

// In-camera persistent buffers
LUCAM_API BOOL LUCAM_EXPORT LucamPermanentBufferRead (HANDLE hCamera, UCHAR *pBuf, ULONG offset, ULONG length);
LUCAM_API BOOL LUCAM_EXPORT LucamPermanentBufferWrite(HANDLE hCamera, UCHAR *pBuf, ULONG offset, ULONG length);

LUCAM_API BOOL LUCAM_EXPORT LucamGetTruePixelDepth(HANDLE hCamera, ULONG *pCount);

LUCAM_API BOOL LUCAM_EXPORT LucamSetTimeout(HANDLE hCamera, BOOL still, float timeout);

//
// For the ConvertFrameTo*Ex functions 
//
typedef struct LUCAM_CONVERSION_PARAMS {
    ULONG Size;               // Must be set to sizeof this struct
    ULONG DemosaicMethod;     // LUCAM_DM_*
    ULONG CorrectionMatrix;   // LUCAM_CM_*
    BOOL  FlipX;
    BOOL  FlipY;
    float Hue;
    float Saturation;
    BOOL  UseColorGainsOverWb;
    union {
        struct {
            float DigitalGain;
            float DigitalWhiteBalanceU;
            float DigitalWhiteBalanceV;
        };
        struct {
            float DigitalGainRed;
            float DigitalGainGreen;
            float DigitalGainBlue;
        };
    };
} LUCAM_CONVERSION_PARAMS;
typedef LUCAM_CONVERSION_PARAMS* PLUCAM_CONVERSION_PARAMS;


typedef struct LUCAM_IMAGE_FORMAT {
    ULONG Size;         // Must be set to sizeof this struct
    ULONG Width;
    ULONG Height;
    ULONG PixelFormat;  // LUCAM_PF_*
    ULONG ImageSize;

    ULONG LucamReserved[8];

} LUCAM_IMAGE_FORMAT;
typedef LUCAM_IMAGE_FORMAT* PLUCAM_IMAGE_FORMAT;

LUCAM_API BOOL LUCAM_EXPORT LucamGetVideoImageFormat(HANDLE hCamera, LUCAM_IMAGE_FORMAT *pImageFormat);
LUCAM_API BOOL LUCAM_EXPORT LucamGetStillImageFormat(HANDLE hCamera, LUCAM_IMAGE_FORMAT *pImageFormat);

LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToRgb24Ex(      HANDLE hCamera, BYTE   *pDest, const BYTE   *pSrc, const LUCAM_IMAGE_FORMAT *pImageFormat, const LUCAM_CONVERSION_PARAMS *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToRgb32Ex(      HANDLE hCamera, BYTE   *pDest, const BYTE   *pSrc, const LUCAM_IMAGE_FORMAT *pImageFormat, const LUCAM_CONVERSION_PARAMS *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToRgb48Ex(      HANDLE hCamera, USHORT *pDest, const USHORT *pSrc, const LUCAM_IMAGE_FORMAT *pImageFormat, const LUCAM_CONVERSION_PARAMS *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToGreyscale8Ex( HANDLE hCamera, BYTE   *pDest, const BYTE   *pSrc,       LUCAM_IMAGE_FORMAT *pImageFormat,       LUCAM_CONVERSION_PARAMS *pParams);
LUCAM_API BOOL LUCAM_EXPORT LucamConvertFrameToGreyscale16Ex(HANDLE hCamera, USHORT *pDest, const USHORT *pSrc,       LUCAM_IMAGE_FORMAT *pImageFormat,       LUCAM_CONVERSION_PARAMS *pParams);

#if defined(_WIN32)
// For use with LucamGetMetadata
// Metadata that may be embedded within an image.
// Capabilities vary from model to model.
#define LUCAM_METADATA_FRAME_COUNTER         1
#define LUCAM_METADATA_TIMESTAMP             2

LUCAM_API BOOL LUCAM_EXPORT LucamGetTimestampFrequency(HANDLE hCamera, ULONGLONG* pTimestampTickFrequency);
LUCAM_API BOOL LUCAM_EXPORT LucamGetTimestamp(HANDLE hCamera, ULONGLONG* pTimestamp);
LUCAM_API BOOL LUCAM_EXPORT LucamSetTimestamp(HANDLE hCamera, ULONGLONG   timestamp);
LUCAM_API BOOL LUCAM_EXPORT LucamEnableTimestamp   (HANDLE hCamera, BOOL     enable);
LUCAM_API BOOL LUCAM_EXPORT LucamIsTimestampEnabled(HANDLE hCamera, BOOL* pIsEnabled);
LUCAM_API BOOL LUCAM_EXPORT LucamGetMetadata(HANDLE hCamera, BYTE* pImageBuffer, LUCAM_IMAGE_FORMAT* pFormat, ULONG metaDataIndex, ULONGLONG* pTimeStamp);

// Events for notification via callback.
// For use with LucamRegisterEventNotification
#define LUCAM_EVENT_START_OF_READOUT          2
#define LUCAM_EVENT_GPI1_CHANGED              4
#define LUCAM_EVENT_GPI2_CHANGED              5
#define LUCAM_EVENT_GPI3_CHANGED              6
#define LUCAM_EVENT_GPI4_CHANGED              7
#define LUCAM_EVENT_DEVICE_SURPRISE_REMOVAL  32

LUCAM_API PVOID LUCAM_EXPORT LucamRegisterEventNotification  (HANDLE hCamera, DWORD eventId, HANDLE hEvent);
LUCAM_API BOOL  LUCAM_EXPORT LucamUnregisterEventNotification(HANDLE hCamera, PVOID pEventInformation);
#endif

// On-host Tap Correction
LUCAM_API BOOL LUCAM_EXPORT LucamPerformDualTapCorrection (HANDLE hCamera, BYTE *pFrame, const LUCAM_IMAGE_FORMAT *pImageFormat);
LUCAM_API BOOL LUCAM_EXPORT LucamPerformMultiTapCorrection(HANDLE hCamera, BYTE *pFrame, const LUCAM_IMAGE_FORMAT *pImageFormat);

#if defined(_WIN32)
LUCAM_API BOOL LUCAM_EXPORT LucamPerformMonoGridCorrection(HANDLE hCamera, BYTE *pFrame, const LUCAM_IMAGE_FORMAT *pImageFormat);

LUCAM_API BOOL LUCAM_EXPORT LucamGetImageIntensity(HANDLE hCamera, BYTE *pFrame, LUCAM_IMAGE_FORMAT *pImageFormat , ULONG startX, ULONG startY, ULONG width, ULONG height, float *pIntensity, float *pRedIntensity, float *pGreen1Intensity, float *pGreen2Intensity, float *pBlueIntensity);

LUCAM_API BOOL LUCAM_EXPORT LucamAutoRoiGet(HANDLE hCamera, LONG *pStartX, LONG *pStartY, LONG *pWidth, LONG *pHeight);
LUCAM_API BOOL LUCAM_EXPORT LucamAutoRoiSet(HANDLE hCamera, LONG   startX, LONG   startY, LONG   width, LONG   height);
LUCAM_API BOOL LUCAM_EXPORT LucamDataLsbAlign(HANDLE hCamera, LUCAM_IMAGE_FORMAT *pLif, UCHAR *pData);

LUCAM_API BOOL LUCAM_EXPORT LucamEnableInterfacePowerSpecViolation   (HANDLE hCamera, BOOL     enable);
LUCAM_API BOOL LUCAM_EXPORT LucamIsInterfacePowerSpecViolationEnabled(HANDLE hCamera, BOOL* pIsEnabled);

// 
// Start of GigE/GEV-specific
//
LUCAM_API BOOL LUCAM_EXPORT LucamSelectExternInterface(ULONG externInterface); // The API defaults to USB

typedef struct LGCAM_IP_CONFIGURATION {
    ULONG IPAddress;
    ULONG SubnetMask;
    ULONG DefaultGateway;
} LGCAM_IP_CONFIGURATION;
typedef LGCAM_IP_CONFIGURATION* PLGCAM_IP_CONFIGURATION;

LUCAM_API BOOL LUCAM_EXPORT LgcamGetIPConfiguration (
    ULONG index, 
    UCHAR cameraMac[6], 
    LGCAM_IP_CONFIGURATION *pCameraConfiguration, 
    UCHAR hostMac[6], 
    LGCAM_IP_CONFIGURATION *pHostConfiguration);
LUCAM_API BOOL LUCAM_EXPORT LgcamSetIPConfiguration (
    ULONG index, 
    LGCAM_IP_CONFIGURATION *pCameraConfiguration);

// GigE/GEV specific properties
#define LUCAM_PROP_GEV_IPCONFIG_LLA                         512 /* Use LUCAM_PROP_FLAG_USE flag */
#define LUCAM_PROP_GEV_IPCONFIG_DHCP                        513 /* Use LUCAM_PROP_FLAG_USE flag */
#define LUCAM_PROP_GEV_IPCONFIG_PERSISTENT                  514 /* Use LUCAM_PROP_FLAG_USE flag */
#define LUCAM_PROP_GEV_IPCONFIG_PERSISTENT_IPADDRESS        515 /* Use a non-cast */
#define LUCAM_PROP_GEV_IPCONFIG_PERSISTENT_SUBNETMASK       516 /* Use a non-cast */
#define LUCAM_PROP_GEV_IPCONFIG_PERSISTENT_DEFAULTGATEWAY   517 /* Use a non-cast */
#define LUCAM_PROP_GEV_SCPD                                 518

//
// End of GigE/GEV-specific
//
#endif

#endif // LUMENERA_LUCAMAPI_H
