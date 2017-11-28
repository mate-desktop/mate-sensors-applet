## Process this file with automake to produce Makefile.in
AM_CPPFLAGS = -DMATELOCALEDIR=\""$(datadir)/locale/"\" \
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
lib_LTLIBRARIES = libmate-sensors-applet-plugin.la

libmate_sensors_applet_plugin_la_SOURCES = sensors-applet-plugin.c
libmate_sensors_applet_plugin_la_LDFLAGS = -export-dynamic -rpath /usr/lib/

libmate_sensors_applet_plugin_la_LIBADD = $(top_builddir)/sensors-applet/libmsa-enum-types.la $(top_builddir)/sensors-applet/libmsa-sensor.la

