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

#ifndef SENSORS_APPLET_H
#define SENSORS_APPLET_H

#include <gtk/gtk.h>
#include <panel-applet.h>
#include "sensors-applet-sensor.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_LIBNOTIFY
#include <libnotify/notify.h>
#endif

typedef struct _ActiveSensor ActiveSensor;

#include "prefs-dialog.h"

#define GRAPH_FRAME_EXTRA_WIDTH 6
#define SENSORS_APPLET_ICON "sensors-applet"

static const gchar * const stock_icons[NUM_ICONS] = {
        "sensors-applet-cpu",
        "sensors-applet-drive-harddisk",
        "sensors-applet-battery",
        "sensors-applet-memory",
        "sensors-applet-gpu",
        "sensors-applet-chip",
        "sensors-applet-fan",
        "sensors-applet-case"
};        

#define DEFAULT_ICON_SIZE 22

#define UNITS_CELSIUS "\302\260C"
#define UNITS_FAHRENHEIT "\302\260F"
#define UNITS_KELVIN ""
#define UNITS_RPM _("RPM")
#define UNITS_VOLTAGE  _("V")
#define UNITS_CURRENT  _("A")


/* enumeration used to identify columns in the GtkTreeStore data
 * structure and to access specific gconf keys too.
 */
enum {
	PATH_COLUMN = 0,
	ID_COLUMN,
	LABEL_COLUMN,
	INTERFACE_COLUMN,
	SENSOR_TYPE_COLUMN,
	ENABLE_COLUMN,
	VISIBLE_COLUMN,
       	LOW_VALUE_COLUMN,
	HIGH_VALUE_COLUMN, 
	ALARM_ENABLE_COLUMN,
	LOW_ALARM_COMMAND_COLUMN,
        HIGH_ALARM_COMMAND_COLUMN,
	ALARM_TIMEOUT_COLUMN,
	MULTIPLIER_COLUMN,
	OFFSET_COLUMN,
	ICON_TYPE_COLUMN,
	ICON_PIXBUF_COLUMN,
        GRAPH_COLOR_COLUMN,
	N_COLUMNS
};


/* for display mode */
typedef enum {
	DISPLAY_LABEL_WITH_VALUE = 0,
        DISPLAY_ICON_WITH_VALUE, 
	DISPLAY_VALUE,
	DISPLAY_ICON,
	DISPLAY_GRAPH
} DisplayMode;

typedef enum {
        VALUE_BESIDE_LABEL = 0,
        VALUE_BELOW_LABEL
} LayoutMode;

typedef enum {
        KELVIN = 0,
        CELSIUS,
        FAHRENHEIT
} TemperatureScale;

/* types of Notifs - low and high alarm warnings and error conditions*/
typedef enum {
        LOW_ALARM = 0,
        HIGH_ALARM,
        SENSOR_INTERFACE_ERROR,
        GCONF_READ_ERROR,
        GCONF_WRITE_ERROR,
        NUM_NOTIFS
} NotifType;

/* only always two type of alarms - may have more notif types */
#define NUM_ALARMS 2

struct _SensorsApplet {
	/* the actual applet for this instance */
	PanelApplet* applet;
        gint size;

	GtkTreeStore *sensors;
	GtkTreeSelection *selection;

        GHashTable *required_plugins;
        GHashTable *plugins;
        
	guint timeout_id;
	/* preferences and about windows (if Gtk < 2.6)*/
	PrefsDialog *prefs_dialog;

	/* primary table to contain the panel dispay - we pack the
	 * list of labels and sensor values into this container */
	GtkWidget *table;
	GList *active_sensors;
#ifdef HAVE_LIBNOTIFY
        NotifyNotification *notification;
#endif // HAVE_LIBNOTIFY
};



/* non-static function prototypes */
void sensors_applet_init(SensorsApplet *sensors_applet);
void sensors_applet_sensor_enabled(SensorsApplet *sensors_applet,
                                   GtkTreePath *path);
void sensors_applet_sensor_disabled(SensorsApplet *sensors_applet,
                                    GtkTreePath *path);
gboolean sensors_applet_update_active_sensors(SensorsApplet *sensors_applet);
/**
 * to be called by things like prefs dialog to turn off a sensor alarm
 */
void sensors_applet_alarm_off(SensorsApplet *sensors_applet,
                              GtkTreePath *path,
                              NotifType notif_type);
void sensors_applet_all_alarms_off(SensorsApplet *sensors_applet,
                              GtkTreePath *path);
void sensors_applet_icon_changed(SensorsApplet *sensors_applet,
                                 GtkTreePath *path);
void sensors_applet_update_sensor(SensorsApplet *sensors_applet,
                                  GtkTreePath *path);

void sensors_applet_display_layout_changed(SensorsApplet *sensors_applet);
void sensors_applet_reorder_sensors(SensorsApplet *sensors_applet);
gdouble sensors_applet_convert_temperature(gdouble value, 
                                           TemperatureScale old, 
                                           TemperatureScale new);
void sensors_applet_notify(SensorsApplet *sensors_applet,
                           NotifType notif_type);
void sensors_applet_notify_end(ActiveSensor *active_sensor, NotifType notif_type);
void sensors_applet_notify_end_all(SensorsApplet *sensors_applet);
void sensors_applet_notify_active_sensor(ActiveSensor *active_sensor, NotifType notif_type);
GdkPixbuf *sensors_applet_load_icon(IconType icon_type);
void sensors_applet_graph_size_changed(SensorsApplet *sensors_applet);

typedef void SensorsInterfaceTestSensorFunc(SensorsApplet *sensors_applet,
                                            const gchar *path);
void sensors_applet_find_sensors(SensorsApplet *sensors_applet,
                                 const gchar *path,
                                 SensorsInterfaceTestSensorFunc test_sensor);

#endif /* SENSORS_APPLET_H */
