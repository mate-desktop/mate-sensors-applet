# PMU-SYS plugin
plugindir = $(libdir)/sensors-applet/plugins

INCLUDES = -DGNOMELOCALEDIR=\""$(datadir)/locale/"\" \
	-DG_LOG_DOMAIN=\""Sensors Applet"\" \
	-DPIXMAPS_DIR=\""$(datadir)/pixmaps/$(PACKAGE)/"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
        -DSYSCONFDIR=\""$(sysconfdir)"\" \
        -DPREFIX=\""$(prefix)"\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

plugin_LTLIBRARIES = libpmu-sys.la

libpmu_sys_la_SOURCES = \
	pmu-sys-plugin.h	\
	pmu-sys-plugin.c

libpmu_sys_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) $(GLIB_LIBS)
libpmu_sys_la_LIBADD = $(top_builddir)/lib/libsensors-applet-plugin.la
