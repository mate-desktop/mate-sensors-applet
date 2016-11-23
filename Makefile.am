SUBDIRS = sensors-applet lib plugins pixmaps po help

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

appletdir =  $(datadir)/mate-panel/applets
applet_in_files = org.mate.applets.SensorsApplet.mate-panel-applet.in
applet_DATA = $(applet_in_files:.mate-panel-applet.in=.mate-panel-applet)

$(applet_in_files): $(applet_in_files).in Makefile
	$(AM_V_GEN)sed \
		-e "s|\@LIBEXECDIR\@|$(libexecdir)|" \
		$< > $@

%.mate-panel-applet: %.mate-panel-applet.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

uidir 		= $(datadir)/mate-sensors-applet/ui
ui_DATA 	= SensorsApplet.xml

servicedir = $(datadir)/dbus-1/services
service_in_files = org.mate.panel.applet.SensorsAppletFactory.service.in
service_DATA = $(service_in_files:.service.in=.service)

org.mate.panel.applet.SensorsAppletFactory.service: $(service_in_files)
	$(AM_V_GEN)sed \
		-e "s|\@LIBEXECDIR\@|$(libexecdir)|" \
		$< > $@

@INTLTOOL_XML_NOMERGE_RULE@
gsettingsschema_in_files = \
	org.mate.sensors-applet.gschema.xml.in \
	org.mate.sensors-applet.sensor.gschema.xml.in

gsettings_SCHEMAS = $(gsettingsschema_in_files:.xml.in=.xml)
@GSETTINGS_RULES@

CLEANFILES = $(applet_DATA) \
	$(applet_DATA).in \
	$(service_DATA) \
	$(gsettings_SCHEMAS) \
	*.gschema.valid

EXTRA_DIST = $(ui_DATA) \
	$(applet_in_files).in \
	$(service_in_files) \
	$(gsettingsschema_in_files) \
	autogen.sh

DISTCLEANFILES	= intltool-extract \
		   	intltool-merge \
	   	   	intltool-update

# Build ChangeLog from GIT  history
ChangeLog:
	$(AM_V_GEN) if test -d $(top_srcdir)/.git; then \
		GIT_DIR="$(top_srcdir)/.git" git log --stat > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog

