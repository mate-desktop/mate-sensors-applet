/*
 * Copyright (C) 2005-2009 Alex Murray <murray.alex@gmail.com>
 *               2013 Stefano Karapetsas <stefano@karapetsas.com>
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <glib.h>
#include "sensors-applet.h"
#include "sensors-applet-settings.h"

gchar* sensors_applet_settings_get_unique_id (const gchar *interface, const gchar *id, const gchar *path) {
    gchar *unique_id;
    gchar *unique_id_hash;
    GChecksum *checksum;
    guint8 digest[16];
    gsize digest_len = sizeof (digest);

    unique_id = g_strdup_printf ("%s/%s/%s", interface, id, path);

    checksum = g_checksum_new (G_CHECKSUM_MD5);
    g_checksum_update (checksum, (const guchar *) unique_id, strlen (unique_id));
    g_checksum_get_digest (checksum, digest, &digest_len);
    g_assert (digest_len == 16);

    unique_id_hash = g_strdup (g_checksum_get_string (checksum));

    g_checksum_free (checksum);
    g_free (unique_id);

    return unique_id_hash;
}

gboolean sensors_applet_settings_save_sensors (SensorsApplet *sensors_applet) {
    /* write everything to GSettings except VISIBLE and
       ALARM_TIMEOUT_INDEX */
    /* for stepping through GtkTreeStore data structure */
    GtkTreeIter interfaces_iter, sensors_iter;
    gboolean not_end_of_interfaces = TRUE, not_end_of_sensors = TRUE;
    gchar *applet_path;

    int i;
    gchar *current_path,
          *current_id,
          *current_label,
          *current_interface,
          *current_low_alarm_command,
          *current_high_alarm_command,
          *current_graph_color;
    gboolean current_enable,
             current_alarm_enable;
    gdouble current_low_value,
            current_high_value,
            current_multiplier,
            current_offset;
    guint current_alarm_timeout,
          current_sensor_type,
          current_icon_type;

    applet_path = mate_panel_applet_get_preferences_path (sensors_applet->applet);

    /* now step through the GtkTreeStore sensors to
       find which sensors are enabled */
    for (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(sensors_applet->sensors), &interfaces_iter); not_end_of_interfaces; not_end_of_interfaces = gtk_tree_model_iter_next(GTK_TREE_MODEL(sensors_applet->sensors), &interfaces_iter)) {
        gtk_tree_model_get(GTK_TREE_MODEL(sensors_applet->sensors),
                           &interfaces_iter,
                           ID_COLUMN, &current_id,
                           -1);

        //mate_panel_applet_mateconf_set_bool(sensors_applet->applet, current_id, TRUE, NULL);
        g_free(current_id);

        /* reset sensors sentinel */
        not_end_of_sensors = TRUE;
        
        for (gtk_tree_model_iter_children(GTK_TREE_MODEL(sensors_applet->sensors), &sensors_iter, &interfaces_iter); not_end_of_sensors; not_end_of_sensors = gtk_tree_model_iter_next(GTK_TREE_MODEL(sensors_applet->sensors), &sensors_iter)) {
            gtk_tree_model_get(GTK_TREE_MODEL(sensors_applet->sensors),
                               &sensors_iter,
                               PATH_COLUMN, &current_path,
                               ID_COLUMN, &current_id,
                               LABEL_COLUMN, &current_label,
                               INTERFACE_COLUMN, &current_interface,
                               SENSOR_TYPE_COLUMN, &current_sensor_type,
                               ENABLE_COLUMN, &current_enable,
                               LOW_VALUE_COLUMN, &current_low_value,
                               HIGH_VALUE_COLUMN, &current_high_value,
                               ALARM_ENABLE_COLUMN, &current_alarm_enable,
                               LOW_ALARM_COMMAND_COLUMN, &current_low_alarm_command,
                               HIGH_ALARM_COMMAND_COLUMN, &current_high_alarm_command,
                               ALARM_TIMEOUT_COLUMN, &current_alarm_timeout,
                               MULTIPLIER_COLUMN, &current_multiplier,
                               OFFSET_COLUMN, &current_offset,
                               ICON_TYPE_COLUMN, &current_icon_type,
                               GRAPH_COLOR_COLUMN, &current_graph_color,
                               -1);

            gchar *path = g_strdup_printf ("%s%s/",
                                           applet_path,
                                           sensors_applet_settings_get_unique_id (current_interface,
                                                                                  current_id,
                                                                                  current_path));

            GSettings *settings;
            settings = g_settings_new_with_path ("org.mate.sensors-applet.sensor", path);
            g_free (path);

            g_settings_delay (settings);
            g_settings_set_string (settings, PATH, current_path);
            g_settings_set_string (settings, ID, current_id);
            g_settings_set_string (settings, LABEL, current_label);
            g_settings_set_string (settings, INTERFACE, current_interface);
            g_settings_set_int (settings, SENSOR_TYPE, current_sensor_type);
            g_settings_set_boolean (settings, ENABLED, current_enable);
            g_settings_set_double (settings, LOW_VALUE, current_low_value);
            g_settings_set_double (settings, HIGH_VALUE, current_high_value);
            g_settings_set_boolean (settings, ALARM_ENABLED, current_alarm_enable);
            g_settings_set_string (settings, LOW_ALARM_COMMAND, current_low_alarm_command);
            g_settings_set_string (settings, HIGH_ALARM_COMMAND, current_high_alarm_command);
            g_settings_set_int (settings, ALARM_TIMEOUT, current_alarm_timeout);
            g_settings_set_double (settings, MULTIPLIER, current_multiplier);
            g_settings_set_double (settings, OFFSET, current_offset);
            g_settings_set_int (settings, ICON_TYPE, current_icon_type);
            g_settings_set_string (settings, GRAPH_COLOR, current_graph_color);
            g_settings_apply (settings);
            g_object_unref (settings);

        }
    }

    g_free (applet_path);

    return TRUE;
}

