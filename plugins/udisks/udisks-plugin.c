/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2009 Pramod Dematagoda <pmd.lotr.gandalf@gmail.com>
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

#include <stdio.h>
#include <atasmart.h>
#include <glib.h>
#include <dbus/dbus-glib.h>
#include "udisks-plugin.h"

#define UDISKS_BUS_NAME              "org.freedesktop.UDisks"
#define UDISKS_DEVICE_INTERFACE_NAME "org.freedesktop.UDisks.Device"
#define UDISKS_INTERFACE_NAME        "org.freedesktop.UDisks"
#define UDISKS_PROPERTIES_INTERFACE  "org.freedesktop.DBus.Properties"
#define UDISKS_OBJECT_PATH           "/org/freedesktop/UDisks"


/*
 * Info about a single sensor
 */
typedef struct _DevInfo{
	gchar *path;
	gboolean changed;
	gdouble temp;
	DBusGProxy *sensor_proxy;
} DevInfo;

const gchar *plugin_name = "udisks";

GHashTable *devices = NULL;

/* This is a global variable for convenience */
DBusGConnection *connection;


/* This is the handler for the Changed() signal emitted by UDisks. */
static void udisks_changed_signal_cb(DBusGProxy *sensor_proxy) {
	const gchar *path = dbus_g_proxy_get_path(sensor_proxy);
	DevInfo *info;

	info = g_hash_table_lookup(devices, path);
	if (info)
	{
		info->changed = TRUE;
	}
}

static void udisks_plugin_get_sensors(GList **sensors) {
	DBusGProxy *proxy, *sensor_proxy;
	GError *error = NULL;
	GPtrArray *paths;
	guint i;
	DevInfo *info;

	g_type_init();

	/* This connection will be used for everything, including the obtaining
	 * of sensor data
	 */
	connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
	if (connection == NULL)
	{
		g_debug("Failed to open connection to DBUS: %s",
							error->message);
		g_error_free(error);
		return;
	}

	/* This is the proxy which is only used once during the enumeration of
	 * the device object paths
	 */
	proxy = dbus_g_proxy_new_for_name(connection,
					  UDISKS_BUS_NAME,
					  UDISKS_OBJECT_PATH,
					  UDISKS_INTERFACE_NAME);

	/* The object paths of the disks are enumerated and placed in an array
	 * of object paths
	 */
	if (!dbus_g_proxy_call(proxy, "EnumerateDevices", &error,
			       G_TYPE_INVALID,
			       dbus_g_type_get_collection("GPtrArray",
							  DBUS_TYPE_G_OBJECT_PATH),
			       &paths,
			       G_TYPE_INVALID))
	{
		g_debug("Failed to enumerate disk devices: %s",
			   error->message);
		g_error_free(error);
		g_object_unref(proxy);
		dbus_g_connection_unref (connection);
		return;
	}

	for (i = 0; i < paths->len; i++) {
		/* This proxy is used to get the required data in order to build
		 * up the list of sensors
		 */
		GValue model = {0, }, id = {0, }, smart_available = {0, };
		gchar *path = (gchar *)g_ptr_array_index(paths, i);

		sensor_proxy = dbus_g_proxy_new_for_name(connection,
							 UDISKS_BUS_NAME,
							 path,
							 UDISKS_PROPERTIES_INTERFACE);

		if (dbus_g_proxy_call(sensor_proxy, "Get", &error,
				      G_TYPE_STRING,
				      UDISKS_BUS_NAME,
				      G_TYPE_STRING,
				      "DriveAtaSmartIsAvailable",
				      G_TYPE_INVALID,
				      G_TYPE_VALUE, &smart_available, G_TYPE_INVALID)) {
			gchar *id_str, *model_str;
			if (!g_value_get_boolean(&smart_available)) {
				g_debug("Drive at path '%s' does not support Smart monitoring... ignoring",
					path);
				g_object_unref(sensor_proxy);
				g_free (path);
				continue;
			}

			dbus_g_proxy_call(sensor_proxy, "Get", NULL,
					  G_TYPE_STRING, UDISKS_BUS_NAME,
					  G_TYPE_STRING, "DriveModel",
					  G_TYPE_INVALID,
					  G_TYPE_VALUE, &model,
					  G_TYPE_INVALID);

			dbus_g_proxy_call(sensor_proxy, "Get", NULL,
					  G_TYPE_STRING, UDISKS_BUS_NAME,
					  G_TYPE_STRING, "DeviceFile",
					  G_TYPE_INVALID,
					  G_TYPE_VALUE, &id,
					  G_TYPE_INVALID);

			g_object_unref(sensor_proxy);

			sensor_proxy = dbus_g_proxy_new_for_name(connection,
								 UDISKS_BUS_NAME,
								 path,
								 UDISKS_DEVICE_INTERFACE_NAME);

			/* Use the Changed() signal emitted from UDisks to
			 * get the temperature without always polling
			 */
			dbus_g_proxy_add_signal(sensor_proxy, "Changed",
						G_TYPE_INVALID);

			dbus_g_proxy_connect_signal(sensor_proxy, "Changed",
						    G_CALLBACK(udisks_changed_signal_cb),
						    path, NULL);

			info = g_malloc(sizeof(DevInfo));
			if (devices == NULL)
			{
				devices = g_hash_table_new(g_str_hash,
							   g_str_equal);
			}
			info->path = g_strdup(path);
			info->sensor_proxy = sensor_proxy;
			/* Set the device status changed as TRUE because we need
			 * to get the initial temperature reading
			 */
			info->changed = TRUE;
			g_hash_table_insert(devices, info->path, info);

			/* Write the sensor data */
			id_str = g_value_get_string(&id);
			model_str = g_value_get_string(&model);
			sensors_applet_plugin_add_sensor(sensors,
							 path,
							 id_str,
							 model_str,
							 TEMP_SENSOR,
							 FALSE,
							 HDD_ICON,
							 DEFAULT_GRAPH_COLOR);
			g_free(id_str);
			g_free(model_str);
		} else {
			g_debug ("Cannot obtain data for device: %s\n"
				    "Error: %s\n",
				    path,
				    error->message);
			g_error_free (error);
			error = NULL;
			g_object_unref(sensor_proxy);
		}
		g_free(path);
	}
	g_ptr_array_free(paths, TRUE);
	g_object_unref(proxy);
	if (devices == NULL)
		dbus_g_connection_unref (connection);
}

static gdouble udisks_plugin_get_sensor_value(const gchar *path,
					      const gchar *id,
					      SensorType type,
					      GError **error) {
	GValue smart_blob_val = { 0, };
	GArray *smart_blob;
	gdouble temperature;
	guint64 temperature_placer;
	DBusGProxy *sensor_proxy;
	guint count;
	DevInfo *info;
	SkDisk *sk_disk;

	info = (DevInfo *)g_hash_table_lookup(devices, path);
	if (info == NULL)
	{
		g_set_error(error, SENSORS_APPLET_PLUGIN_ERROR, 0,
			    "Error finding disk with path %s", path);
		return 0.0;
	}

	/* If the device has changed, we find the new temperature and return
	 * it
	 */
	if (info->changed)
	{
		GValue smart_time = { 0, };
		sensor_proxy = dbus_g_proxy_new_for_name(connection,
							 UDISKS_BUS_NAME,
							 path,
							 UDISKS_PROPERTIES_INTERFACE);
		if (!dbus_g_proxy_call(sensor_proxy, "Get", error,
				       G_TYPE_STRING, UDISKS_BUS_NAME,
				       G_TYPE_STRING, "DriveAtaSmartTimeCollected", G_TYPE_INVALID,
				       G_TYPE_VALUE, &smart_time,
				       G_TYPE_INVALID) ||
		    !g_value_get_uint64(&smart_time))
		{
			g_debug("Smart data has not been collected yet... returning 0.0 temp for now to avoid waking drive up unnecessarily");
			g_object_unref(sensor_proxy);
			return 0.0;
		}

		if (dbus_g_proxy_call(sensor_proxy, "Get", error,
				      G_TYPE_STRING, UDISKS_BUS_NAME,
				      G_TYPE_STRING, "DriveAtaSmartBlob", G_TYPE_INVALID,
				      G_TYPE_VALUE, &smart_blob_val,
				      G_TYPE_INVALID))
		{
			smart_blob = g_value_get_boxed(&smart_blob_val);

			sk_disk_open(NULL, &sk_disk);
			sk_disk_set_blob (sk_disk, smart_blob->data, smart_blob->len);
			/* Note: A gdouble cannot be passed in through a cast as it is likely that the
			 * temperature is placed in it purely through memory functions, hence a guint64
			 * is passed and the number is then placed in a gdouble manually 
			 */
			sk_disk_smart_get_temperature (sk_disk, &temperature_placer);
			temperature = temperature_placer;

			/* Temperature is in mK, so convert it to K first */
			temperature /= 1000;
			info->temp = temperature - 273.15;
			info->changed = FALSE;

			g_free (sk_disk);
			g_array_free(smart_blob, TRUE);
		}
		g_object_unref(sensor_proxy);
	}
	return info->temp;
}

static GList *udisks_plugin_init(void) {
	GList *sensors = NULL;

	udisks_plugin_get_sensors(&sensors);

	return sensors;
}

const gchar *sensors_applet_plugin_name(void)
{
	return plugin_name;
}

GList *sensors_applet_plugin_init(void)
{
	return udisks_plugin_init();
}

gdouble sensors_applet_plugin_get_sensor_value(const gchar *path,
					       const gchar *id,
					       SensorType type,
					       GError **error) {
	return udisks_plugin_get_sensor_value(path, id, type, error);
}
