## Process this file with automake to produce Makefile.in
INCLUDES = -DGNOMELOCALEDIR=\""$(datadir)/locale/"\" \
	-DG_LOG_DOMAIN=\""Sensors Applet"\" \
	-DPIXMAPS_DIR=\""$(datadir)/pixmaps/$(PACKAGE)/"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-DSENSORS_APPLET_PLUGIN_DIR=\""$(libdir)/$(PACKAGE)/plugins/"\" \
        -DSYSCONFDIR=\""$(sysconfdir)"\" \
        -DPREFIX=\""$(prefix)"\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

LIBS = $(GLIB_LIBS)

# also build the libsensorsplugin library
lib_LTLIBRARIES = libsensors-applet-plugin.la

libsensors_applet_plugin_la_SOURCES = sensors-applet-plugin.c 
libsensors_applet_plugin_la_LDFLAGS = -export-dynamic