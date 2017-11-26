## Process this file with automake to produce Makefile.in

uidir = $(datadir)/mate-sensors-applet/ui

AM_CPPFLAGS = -DMATELOCALEDIR=\""$(datadir)/locale/"\" \
	-DG_LOG_DOMAIN=\""Sensors Applet"\" \
	-DPIXMAPS_DIR=\""$(datadir)/pixmaps/$(PACKAGE)/"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-DUIDIR=\""$(pkgdatadir)/ui"\" \
	-DSENSORS_APPLET_PLUGIN_DIR=\""$(libdir)/$(PACKAGE)/plugins/"\" \
        -DSYSCONFDIR=\""$(sysconfdir)"\" \
        -DPREFIX=\""$(prefix)"\" \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) $(GTK_CFLAGS) $(MATE_CFLAGS) $(CAIRO_CFLAGS) $(LIBNOTIFY_CFLAGS)


LIBS = $(GLIB_LIBS) $(GTK_LIBS) $(MATE_LIBS) $(CAIRO_LIBS) $(LIBNOTIFY_LIBS)

if LIBNOTIFY
libnotify_SRC = active-sensor-libnotify.c \
		active-sensor-libnotify.h
else
libnotify_SRC =
endif

libexec_PROGRAMS = mate-sensors-applet

# enum generation
# A list of headers to inspect
enum_headers = sensors-applet-sensor.h

# command in bash: glib-mkenums --template "enum-types.h.in" test.h > enum-types.h
msa-enum-types.h: $(enum_headers) msa-enum-types.h.in
				$(AM_V_GEN)$(GLIB_MKENUMS) --template "msa-enum-types.h.in" --identifier-prefix "MSA" $(enum_headers) > $@

msa-enum-types.c: $(enum_headers) msa-enum-types.c.in msa-enum-types.h
				$(AM_V_GEN)$(GLIB_MKENUMS) --template "msa-enum-types.c.in" --identifier-prefix "MSA" $(enum_headers) > $@

# generate them first to prevent make errors if they are included in the other files
mate_sensors_applet_SOURCES = msa-enum-types.h msa-enum-types.c

# clean generated files with make clean
CLEANFILES = msa-enum-types.h msa-enum-types.c

# do not install these on the system:
EXTRA_DIST = msa-enum-types.h.in msa-enum-types.c.in

#  end enum generation

mate_sensors_applet_SOURCES += main.c \
		msa-sensor.h \
		msa-sensor.c \
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
		$(libnotify_SRC)

mate_sensors_applet_LDADD = -ldl

# install headers for plugins to use
INST_H_FILES = sensors-applet-plugin.h sensors-applet-sensor.h

headerdir = $(prefix)/include/mate-sensors-applet

header_DATA = $(INST_H_FILES)

# since it is separate
EXTRA_DIST += sensors-applet-plugin.h
