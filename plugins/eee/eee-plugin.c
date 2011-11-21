/*
 * Copyright (C) 2005-2006 Nickolay Surin <nickolay.surin@gmail.com>
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
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif /* HAVE_STDIO_H */

#include <glib.h>
#include <glib/gi18n.h>
#include "eee-plugin.h"

const gchar *plugin_name = "eee";

#define EEE_FAN_FILE "/proc/eee/fan_rpm"

enum {
	EEE_DEVICE_FILE_OPEN_ERROR,
	EEE_DEVICE_FILE_READ_ERROR
};

static void eee_plugin_add_sensor(GList **sensors, const gchar *path) {
	gchar *dirname;
	gchar *id;
	gchar *label;

	dirname = g_path_get_dirname(path);
	id = g_path_get_basename(dirname);
	g_free(dirname);

	sensors_applet_plugin_add_sensor(sensors,
					 path,
					 id,
					 _("FAN"),
					 FAN_SENSOR,
					 TRUE,
					 FAN_ICON,
					 DEFAULT_GRAPH_COLOR);
	g_free(id);
}

static void eee_plugin_test_sensor(GList **sensors,
				   const gchar *path) {
	gchar *filename;
	filename = g_path_get_basename(path);
	eee_plugin_add_sensor(sensors, path);
	g_free(filename);

}

static GList *eee_plugin_init(void) {
	GList *sensors = NULL;

	sensors_applet_plugin_find_sensors(&sensors, EEE_FAN_FILE,
					   eee_plugin_test_sensor);
	return sensors;
}


static gdouble eee_plugin_get_sensor_value(const gchar *path,
					   const gchar *id,
					   SensorType type,
					   GError **error) {

	FILE *fp;
	gfloat sensor_value = 0;
	gchar units[32];

	if (NULL == (fp = fopen(path, "r"))) {
		g_set_error(error, SENSORS_APPLET_PLUGIN_ERROR, EEE_DEVICE_FILE_OPEN_ERROR, "Error opening sensor device file %s", path);
		return sensor_value;
	}

	if (fscanf(fp, "%f", &sensor_value, units) < 1) {
		g_set_error(error, SENSORS_APPLET_PLUGIN_ERROR, EEE_DEVICE_FILE_READ_ERROR, "Error reading from sensor device file %s", path);
		fclose(fp);
		return sensor_value;
	}
	fclose(fp);

	return (gdouble)sensor_value;
}

const gchar *sensors_applet_plugin_name(void)
{
	return plugin_name;
}

GList *sensors_applet_plugin_init(void)
{
	return eee_plugin_init();
}

gdouble sensors_applet_plugin_get_sensor_value(const gchar *path,
					       const gchar *id,
					       SensorType type,
					       GError **error) {
	return eee_plugin_get_sensor_value(path, id, type, error);
}
