## Process this file with automake to produce Makefile.in

uidir = $(datadir)/mate-sensors-applet/ui

INCLUDES = -DMATELOCALEDIR=\""$(datadir)/locale/"\" \
	-DG_LOG_DOMAIN=\""Sensors Applet"\" \
	-DPIXMAPS_DIR=\""$(datadir)/pixmaps/$(PACKAGE)/"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-DUIDIR=\""$(pkgdatadir)/ui"\" \
	-DSENSORS_APPLET_PLUGIN_DIR=\""$(libdir)/$(PACKAGE)/plugins/"\" \
        -DSYSCONFDIR=\""$(sysconfdir)"\" \
        -DPREFIX=\""$(prefix)"\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) $(GTK_CFLAGS) $(MATE_CFLAGS) $(CAIRO_CFLAGS) $(LIBMATENOTIFY_CFLAGS)


LIBS = $(GLIB_LIBS) $(GTK_LIBS) $(MATE_LIBS) $(CAIRO_LIBS) $(LIBMATENOTIFY_LIBS)

if LIBMATENOTIFY
libmatenotify_SRC = active-sensor-libmatenotify.c \
		active-sensor-libmatenotify.h
else
libmatenotify_SRC =
endif

libexec_PROGRAMS = mate-sensors-applet
mate_sensors_applet_SOURCES = main.c \
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
		sensors-applet-settings.c \
		sensors-applet-settings.h \
		$(libmatenotify_SRC) 

# install headers for plugins to use
INST_H_FILES = sensors-applet-plugin.h sensors-applet-sensor.h

headerdir = $(prefix)/include/mate-sensors-applet

header_DATA = $(INST_H_FILES)

# since it is separate
EXTRA_DIST = sensors-applet-plugin.h
