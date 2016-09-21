#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstlumenerasrc.h"

#define GST_CAT_DEFAULT gst_gstlumenera_debug
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);

static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, "lumenerasrc", 0,
      "debug category for Lumenera elements");

  if (!gst_element_register (plugin, "lumenerasrc", GST_RANK_NONE,
          GST_TYPE_LU_SRC)) {
    return FALSE;
  }

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    lumenera,
    "Lumenera camera source element.",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, "http://users.ox.ac.uk/~atdgroup")
