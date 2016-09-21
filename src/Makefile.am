# Note: plugindir is set in configure

plugin_LTLIBRARIES = liblumeneraplugin.la

# Path to installation of the lumenera SDK 
LU_CFLAGS = -I/usr/include
LU_LIBS = -llucamapi -L/usr/lib

# sources used to compile this plug-in
liblumeneraplugin_la_SOURCES = gstlumenerasrc.c gstlumenerasrc.h gstplugin.c

# compiler and linker flags used to compile this plugin, set in configure.ac
liblumeneraplugin_la_CFLAGS = $(GST_CFLAGS) $(LU_CFLAGS)
liblumeneraplugin_la_LIBADD = $(GST_LIBS) $(LU_LIBS) -lgstvideo-1.0
liblumeneraplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
liblumeneraplugin_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstlumenerasrc.h
