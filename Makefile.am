SUBDIRS = sensors-applet lib plugins pixmaps po help

serverdir 	= $(libdir)/bonobo/servers
server_in_files = SensorsApplet.server.in
server_DATA 	= $(server_in_files:.server.in=.server)

$(server_in_files): $(server_in_files:.server.in=.server.in.in) Makefile
	sed -e "s|\@DATADIR\@|$(datadir)|" -e "s|\@LIBEXECDIR\@|$(libexecdir)|" -e "s|\@PACKAGE\@|$(PACKAGE)|" $< > $@

@INTLTOOL_SERVER_RULE@

uidir 		= $(datadir)/gnome-2.0/ui
ui_DATA 	= SensorsApplet.xml

CLEANFILES 	=  $(server_in_files) \
		   $(server_DATA)

EXTRA_DIST 	= $(ui_DATA) \
		  $(server_DATA:.server=.server.in.in) \
		  intltool-merge.in intltool-extract.in intltool-update.in \
		  gnome-doc-utils.make

DISTCLEANFILES	=	gnome-doc-utils.make \
		   	intltool-extract \
		   	intltool-merge \
	   	   	intltool-update

DISTCHECK_CONFIGURE_FLAGS	= --disable-scrollkeeper