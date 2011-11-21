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
	$(GLIB_CFLAGS) $(GTK_CFLAGS) $(GNOME_CFLAGS) $(CAIRO_CFLAGS) $(LIBNOTIFY_CFLAGS)


LIBS = $(GLIB_LIBS) $(GTK_LIBS) $(GNOME_LIBS) $(CAIRO_LIBS) $(LIBNOTIFY_LIBS)

if LIBNOTIFY
libnotify_SRC = active-sensor-libnotify.c \
		active-sensor-libnotify.h
else
libnotify_SRC =
endif

libexec_PROGRAMS = sensors-applet
sensors_applet_SOURCES = main.c \
		about-dialog.c \
		about-dialog.h \
		active-sensor.c \
		active-sensor.h \
		prefs-dialog.c \
		prefs-dialog.h \
		sensor-config-dialog.c \
		sensor-config-dialog.h \
		sensors-applet.c \
		sensors-applet.h \
		sensors-applet-sensor.h \
		sensors-applet-plugins.c \
		sensors-applet-plugins.h \
		sensors-applet-gconf.c \
		sensors-applet-gconf.h \
		$(libnotify_SRC) 

# install headers for plugins to use
INST_H_FILES = sensors-applet-plugin.h sensors-applet-sensor.h

headerdir = $(prefix)/include/sensors-applet

header_DATA = $(INST_H_FILES)

# since it is separate
EXTRA_DIST = sensors-applet-plugin.h