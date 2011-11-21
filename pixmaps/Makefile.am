pixmapsdir = $(datadir)/pixmaps/sensors-applet
pixmaps_DATA = \
	very-low-temp-icon.png \
	low-temp-icon.png \
	normal-temp-icon.png \
	high-temp-icon.png \
	very-high-temp-icon.png

deviceiconsdir = $(datadir)/icons/hicolor/22x22/devices
deviceicons_DATA = \
	sensors-applet-battery.png \
	sensors-applet-case.png \
	sensors-applet-chip.png \
	sensors-applet-cpu.png \
	sensors-applet-gpu.png \
	sensors-applet-fan.png \
	sensors-applet-drive-harddisk.png \
	sensors-applet-memory.png

appicondir = $(datadir)/icons/hicolor/48x48/apps
appicon_DATA = sensors-applet.png

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook: update_icon_cache
uninstall-hook: update_icon_cache
update_icon_cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated.  After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi

EXTRA_DIST = \
	$(pixmaps_DATA) \
	$(deviceicons_DATA) \
	$(appicon_DATA)