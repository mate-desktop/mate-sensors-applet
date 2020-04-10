# Dummy plugin
plugindir = $(libdir)/mate-sensors-applet/plugins

AM_CPPFLAGS = -DMATELOCALEDIR=\""$(datadir)/locale/"\" \
	-DG_LOG_DOMAIN=\""Sensors Applet"\" \
	-DPIXMAPS_DIR=\""$(datadir)/pixmaps/$(PACKAGE)/"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
        -DSYSCONFDIR=\""$(sysconfdir)"\" \
        -DPREFIX=\""$(prefix)"\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(WARN_CFLAGS)


plugin_LTLIBRARIES = libdummy.la

libdummy_la_SOURCES =	\
	dummy-plugin.h	\
	dummy-plugin.c

libdummy_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) $(GLIB_LIBS)

libdummy_la_LIBADD = $(top_builddir)/lib/libmate-sensors-applet-plugin.la
