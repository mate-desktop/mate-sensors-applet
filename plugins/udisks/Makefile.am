# UDisks plugin
plugindir = $(libdir)/sensors-applet/plugins

INCLUDES = -DGNOMELOCALEDIR=\""$(datadir)/locale/"\" \
	-DG_LOG_DOMAIN=\""Sensors Applet"\" \
	-DPIXMAPS_DIR=\""$(datadir)/pixmaps/$(PACKAGE)/"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
        -DSYSCONFDIR=\""$(sysconfdir)"\" \
        -DPREFIX=\""$(prefix)"\" \
	-I$(top_srcdir) \
	$(DBUSGLIB_CFLAGS)
	$(LIBATASMART_CFLAGS)



plugin_LTLIBRARIES = libudisks.la

libudisks_la_SOURCES = \
	udisks-plugin.h	\
	udisks-plugin.c

libudisks_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) $(DBUSGLIB_LIBS)	$(LIBATASMART_LIBS)

libudisks_la_LIBADD = $(top_builddir)/lib/libsensors-applet-plugin.la
