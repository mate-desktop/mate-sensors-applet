# IBM-ACPI plugin
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

plugin_LTLIBRARIES = libibm-acpi.la

libibm_acpi_la_SOURCES = \
	ibm-acpi-plugin.h	\
	ibm-acpi-plugin.c

libibm_acpi_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) $(GLIB_LIBS)
libibm_acpi_la_LIBADD = $(top_builddir)/lib/libsensors-applet-plugin.la
