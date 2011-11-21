/*
 * Copyright (C) 2005-2009 Alex Murray <murray.alex@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <panel-applet.h>
#include <string.h>
#include "sensors-applet.h"

static gboolean sensors_applet_fill(PanelApplet *applet, 
                                    const gchar *iid, 
                                    gpointer data) {
	SensorsApplet *sensors_applet;
	gboolean retval = FALSE;
        if (strcmp(iid, "OAFIID:SensorsApplet") == 0) {
		sensors_applet = g_new0(SensorsApplet, 1);
		sensors_applet->applet = applet;
		sensors_applet_init(sensors_applet);
		retval = TRUE;
	}
	return retval;
}

PANEL_APPLET_BONOBO_FACTORY ("OAFIID:SensorsApplet_Factory", 
			     PANEL_TYPE_APPLET, 
			     PACKAGE, 
			     PACKAGE_VERSION, 
			     sensors_applet_fill, 
			     NULL);
