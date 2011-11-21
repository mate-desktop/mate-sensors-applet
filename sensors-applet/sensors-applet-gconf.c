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

#include "sensors-applet.h"
#include "sensors-applet-gconf.h"

#define DEFAULT_TIMEOUT 2000
#define DEFAULT_GRAPH_SIZE 42

static const gchar * const compatible_versions[] = {
        PACKAGE_VERSION, /* always list current version */
        "2.2.6",
        "2.2.5",
	"2.2.4",
        "2.2.3",
	"2.2.2",
};

#define NUM_COMPATIBLE_VERSIONS G_N_ELEMENTS(compatible_versions)

typedef enum {
        SENSORS_APPLET_GCONF_ERROR = 0,
        SENSORS_APPLET_VERSION_ERROR,
} SensorsAppletGConfError;

static const gchar * const error_titles[] = {
        N_("An error occurred loading the stored sensors data"),
        N_("Incompatible sensors configuration found")
};

static const gchar * const error_messages[] = {
        N_("An error has occurred when loading the stored sensors data. "
           "The default values will be used to recover from this error."),
        
        N_("Unfortunately the previous configuration for GNOME Sensors Applet "
           "is not compatible with this version. The existing sensors data "
           "will be overwritten with the default values for this new version.")
};

/* function to be called if an error occurs
   when loading values from gconf */
static void sensors_applet_gconf_error_occurred(SensorsAppletGConfError error) {
	GtkWidget *dialog;
        gchar *markup;

        g_debug("Error occurred: %s", error_titles[error]);
        markup = g_markup_printf_escaped("<span size=\"large\" weight=\"bold\">%s</span>\n\n%s", _(error_titles[error]), _(error_messages[error]));

	dialog = gtk_message_dialog_new_with_markup(NULL, /* no parent window */
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_WARNING,
                                                    GTK_BUTTONS_OK,
                                                    "%s", markup);

        g_free(markup);

        /* runs dialog as modal and doesn't return until user clicks
         * button */
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));
}


static void sensors_applet_gconf_set_defaults(SensorsApplet *sensors_applet) {
	panel_applet_gconf_set_int(sensors_applet->applet, DISPLAY_MODE, DISPLAY_ICON_WITH_VALUE, NULL);
	panel_applet_gconf_set_int(sensors_applet->applet, LAYOUT_MODE, VALUE_BESIDE_LABEL, NULL);
	panel_applet_gconf_set_int(sensors_applet->applet, TEMPERATURE_SCALE, CELSIUS, NULL);
	panel_applet_gconf_set_int(sensors_applet->applet, TIMEOUT, DEFAULT_TIMEOUT, NULL);
	panel_applet_gconf_set_int(sensors_applet->applet, GRAPH_SIZE, DEFAULT_GRAPH_SIZE, NULL);
#ifdef HAVE_LIBNOTIFY
	panel_applet_gconf_set_bool(sensors_applet->applet, DISPLAY_NOTIFICATIONS, TRUE, NULL);
#endif        
	panel_applet_gconf_set_bool(sensors_applet->applet, IS_SETUP, FALSE, NULL);

}

/**
 * Returns TRUE is old_version is one of the compatible versions
 */
static gboolean sensors_applet_gconf_is_compatible(const gchar *old_version) {
        guint i;
        for (i = 0; i < NUM_COMPATIBLE_VERSIONS; i++) {
                if (g_ascii_strcasecmp(old_version, compatible_versions[i]) == 0) {
                        return TRUE;
                }
        }
        return FALSE;
}


void sensors_applet_gconf_setup(SensorsApplet *sensors_applet) {
        gboolean setup = FALSE;
        gchar *old_version;
        GError *error = NULL;

        /* need to convert old num_samples value to new GRAPH_SIZE
         * parameter */
        gint num_samples;
        if ((num_samples = panel_applet_gconf_get_int(sensors_applet->applet,
                                                      "num_samples",
                                                      NULL))) {
                g_debug("Convering old num_samples value %d into graph_size", num_samples);
                panel_applet_gconf_set_int(sensors_applet->applet,
                                           GRAPH_SIZE,
                                           (num_samples + GRAPH_FRAME_EXTRA_WIDTH),
                                           NULL);
                /* reset num_samples to zero */
                panel_applet_gconf_set_int(sensors_applet->applet,
                                           "num_samples",
                                           0,
                                           NULL);
                
        }                                      

        /* convert old alarm_commands to high and low if exist */
        GSList *alarm_commands;
        if ((alarm_commands = panel_applet_gconf_get_list(sensors_applet->applet,
                                                          "alarm_commands",
                                                          GCONF_VALUE_STRING,
                                                          NULL))) {
                
                g_debug("Converting old alarm commands to new high and low commands");
                
                panel_applet_gconf_set_list(sensors_applet->applet,
                                            LOW_ALARM_COMMANDS,
                                            GCONF_VALUE_STRING,
                                            alarm_commands,
                                            NULL);
                panel_applet_gconf_set_list(sensors_applet->applet,
                                            HIGH_ALARM_COMMANDS,
                                            GCONF_VALUE_STRING,
                                            alarm_commands,
                                            NULL);
                /* reset old list to null */
                panel_applet_gconf_set_list(sensors_applet->applet,
                                            "alarm_commands",
                                            GCONF_VALUE_STRING,
                                            NULL,
                                            NULL);
                g_slist_foreach(alarm_commands, (GFunc)g_free, NULL);
                g_slist_free(alarm_commands);
                
        }

        setup = panel_applet_gconf_get_bool(sensors_applet->applet, 
                                            IS_SETUP, &error);
        if (error) {
                g_debug("Previous configuration not found: %s, setting up manually", error->message);
                g_error_free(error);
                error = NULL;
                setup = FALSE;
        }


	if (setup) {
                /* see if setup version matches */
                old_version = panel_applet_gconf_get_string(sensors_applet->applet,
                                                            SENSORS_APPLET_VERSION,
                                                            &error);
                /* if versions don't match or there is no saved
                 * version string then need to overwrite old config */
                if (error) {
                        g_debug("Error getting old version string: %s", error->message);
                        g_error_free(error);
                        error = NULL;
                        old_version = NULL;
                }
                
                if (old_version) {
                        if (sensors_applet_gconf_is_compatible(old_version)) {
                                /* previously setup and versions match so use
                                 * old values */
                                g_debug("GConf data is compatible. Trying to set up sensors from gconf data");
                                if (sensors_applet_gconf_setup_sensors(sensors_applet)) {
                                        g_debug("done setting up from gconf");
                                } else {
                                        g_debug("Setting gconf defaults only");
                                        sensors_applet_gconf_set_defaults(sensors_applet);
                                }
                                g_free(old_version);

                                return;

                                        
                        }
                        g_free(old_version);

                }
                sensors_applet_notify(sensors_applet, GCONF_READ_ERROR);
                
                        
                sensors_applet_gconf_error_occurred(SENSORS_APPLET_VERSION_ERROR);
        }
        
        /* use defaults */
        g_debug("Setting gconf defaults only");
        sensors_applet_gconf_set_defaults(sensors_applet);
}

enum {
        PATHS_INDEX = 0,
        IDS_INDEX,
        LABELS_INDEX,
        INTERFACES_INDEX,
        SENSOR_TYPES_INDEX, 
        ENABLES_INDEX, 
        LOW_VALUES_INDEX, 
        HIGH_VALUES_INDEX, 
        ALARM_ENABLES_INDEX, 
        LOW_ALARM_COMMANDS_INDEX, 
        HIGH_ALARM_COMMANDS_INDEX,
        ALARM_TIMEOUTS_INDEX, 
        MULTIPLIERS_INDEX,
        OFFSETS_INDEX,
        ICON_TYPES_INDEX, 
        GRAPH_COLORS_INDEX,
        NUM_KEYS
};

const gchar * const keys[NUM_KEYS] = {
        PATHS,
        IDS,
        LABELS,
        INTERFACES,
        SENSOR_TYPES, 
        ENABLES, 
        LOW_VALUES, 
        HIGH_VALUES, 
        ALARM_ENABLES, 
        LOW_ALARM_COMMANDS,
        HIGH_ALARM_COMMANDS,
        ALARM_TIMEOUTS, 
        MULTIPLIERS,
        OFFSETS,
        ICON_TYPES, 
        GRAPH_COLORS,
};

/* MUST CORRESPOND TO ABOVE KEYS */
const GConfValueType key_types[NUM_KEYS] = {
        GCONF_VALUE_STRING, /* PATHS */
        GCONF_VALUE_STRING, /* IDS, */
        GCONF_VALUE_STRING, /* LABELS */
        GCONF_VALUE_STRING, /* INTERFACES, */
        GCONF_VALUE_INT, /* SENSOR_TYPES, */
        GCONF_VALUE_BOOL, /* ENABLES, */
        GCONF_VALUE_INT, /* LOW_VALUES, */
        GCONF_VALUE_INT, /* HIGH_VALUES, */
        GCONF_VALUE_BOOL, /* ALARM_ENABLES, */
        GCONF_VALUE_STRING, /* LOW_ALARM_COMMANDS, */
        GCONF_VALUE_STRING, /* HIGH_ALARM_COMMANDS, */
        GCONF_VALUE_INT, /* ALARM_TIMEOUTS, */
        GCONF_VALUE_INT, /* MULTIPLIERS, */
        GCONF_VALUE_INT, /* OFFSETS, */
        GCONF_VALUE_INT, /* ICON_TYPES, */
        GCONF_VALUE_STRING /* GRAPH_COLORS, */
};

void sensors_applet_gconf_set_current_to_lists(GSList *current[],
                                               GSList *lists[],
                                               int len) {
        for (len--; len >= 0; len--) {
                current[len] = lists[len];
        }
}

int sensors_applet_gconf_current_not_null(GSList *current[],
                                          int len) {
        for (len--; len >= 0; len--) {
                if (NULL == current[len]) {
                        return FALSE;
                }
        }
        return TRUE;
}
void sensors_applet_gconf_current_get_next(GSList *current[],
                                           int len) {
        for (len--; len >= 0; len--) {
                current[len] = g_slist_next(current[len]);
        }
}

void sensors_applet_gconf_free_lists(GSList *lists[],
                                     int len) {
        for (len--; len >= 0; len--) {
                if (key_types[len] == GCONF_VALUE_STRING) {
                        g_slist_foreach(lists[len], (GFunc)g_free, NULL);
                }
                g_slist_free(lists[len]);
        }

}

/* gets called if are already setup so we don't have to manually go
   through and find sensors etc again */
gboolean sensors_applet_gconf_setup_sensors(SensorsApplet *sensors_applet) {
	/* everything gets stored except alarm timeout indexes, which
	   we set to -1, and visible which we set to false for all
	   parent nodes and true for all child nodes */
        int i;
        GSList *lists[NUM_KEYS] = {NULL}; 

	GSList *current[NUM_KEYS] = {NULL}; 

	GError *error = NULL;

        for (i = 0; i < NUM_KEYS; i++) {
                lists[i] = panel_applet_gconf_get_list(sensors_applet->applet, 
                                                       keys[i], 
                                                       key_types[i], 
                                                       &error);
                if (error || NULL == lists[i]) {
                        sensors_applet_notify(sensors_applet, GCONF_READ_ERROR);
                
                        sensors_applet_gconf_error_occurred(SENSORS_APPLET_GCONF_ERROR);
                        if (error) {
                                g_error_free(error);
                        }
                        return FALSE;
                }
        }

	for (sensors_applet_gconf_set_current_to_lists(current,
                                                       lists,
                                                       NUM_KEYS);
             sensors_applet_gconf_current_not_null(current,
                                                   NUM_KEYS);
             sensors_applet_gconf_current_get_next(current,
                                                   NUM_KEYS)) {


		g_debug("trying to add sensor from gconf data: %s\n", (gchar *)(current[IDS_INDEX]->data));
                /* need to ensure correct order */
		sensors_applet_add_sensor(sensors_applet,
                                          (gchar *)(current[PATHS_INDEX]->data),
                                          (gchar *)(current[IDS_INDEX]->data), 
                                          (gchar *)(current[LABELS_INDEX]->data), 
                                          (gchar *)(current[INTERFACES_INDEX]->data),
                                          GPOINTER_TO_UINT(current[SENSOR_TYPES_INDEX]->data),
                                          GPOINTER_TO_INT(current[ENABLES_INDEX]->data),
                                          (gdouble)(GPOINTER_TO_INT(current[LOW_VALUES_INDEX]->data) / 1000.0),
                                          (gdouble)(GPOINTER_TO_INT(current[HIGH_VALUES_INDEX]->data) / 1000.0),
                                          GPOINTER_TO_INT(current[ALARM_ENABLES_INDEX]->data),
                                          (gchar *)(current[LOW_ALARM_COMMANDS_INDEX]->data),
                                          (gchar *)(current[HIGH_ALARM_COMMANDS_INDEX]->data),
                                          GPOINTER_TO_INT(current[ALARM_TIMEOUTS_INDEX]->data),
                                          (gdouble)(GPOINTER_TO_INT(current[MULTIPLIERS_INDEX]->data) / 1000.0),
                                          (gdouble)(GPOINTER_TO_INT(current[OFFSETS_INDEX]->data) / 1000.0),
                                          (SensorType)GPOINTER_TO_UINT(current[ICON_TYPES_INDEX]->data),
                                          (gchar *)(current[GRAPH_COLORS_INDEX]->data)
                                          
                        );
                
	}
        sensors_applet_gconf_free_lists(lists,
                                        NUM_KEYS);

	return TRUE;
}


gboolean sensors_applet_gconf_save_sensors(SensorsApplet *sensors_applet) {
	/* write everything to gconf except VISIBLE and
	   ALARM_TIMEOUT_INDEX */
	/* for stepping through GtkTreeStore data structure */
	GtkTreeIter interfaces_iter, sensors_iter;
	gboolean not_end_of_interfaces = TRUE, not_end_of_sensors = TRUE;

        /* make sure all are initialized to null - since list of
         * intializers is horter than number of element, rest get set
         * to 0 (ie NULL) */
        GSList *lists[NUM_KEYS] = {NULL};
        int i;
        gchar *current_path, *current_id, *current_label, *current_interface,
                *current_low_alarm_command, *current_high_alarm_command, 
                *current_graph_color;
        gboolean current_enable, current_alarm_enable;
	gdouble current_low_value, current_high_value, current_multiplier, 
                current_offset;
	guint current_alarm_timeout, current_sensor_type, 
                current_icon_type;
	
	GError *error = NULL;

	/* now step through the GtkTreeStore sensors to
	   find which sensors are enabled */
	for (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(sensors_applet->sensors), &interfaces_iter); not_end_of_interfaces; not_end_of_interfaces = gtk_tree_model_iter_next(GTK_TREE_MODEL(sensors_applet->sensors), &interfaces_iter)) {
		// store a gconf key for this interface
		gtk_tree_model_get(GTK_TREE_MODEL(sensors_applet->sensors), 
				   &interfaces_iter,
				   ID_COLUMN, &current_id,
				   -1);

		panel_applet_gconf_set_bool(sensors_applet->applet, current_id, TRUE, NULL);
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

			/* prepend values as this is faster then just
			   reverse list when finished */
                        lists[PATHS_INDEX] = g_slist_prepend(lists[PATHS_INDEX], current_path);
			lists[IDS_INDEX] = g_slist_prepend(lists[IDS_INDEX], current_id);
			lists[LABELS_INDEX] = g_slist_prepend(lists[LABELS_INDEX], current_label);
			lists[INTERFACES_INDEX] = g_slist_prepend(lists[INTERFACES_INDEX], current_interface);
			lists[ENABLES_INDEX] = g_slist_prepend(lists[ENABLES_INDEX], GINT_TO_POINTER(current_enable));
			lists[LOW_VALUES_INDEX] = g_slist_prepend(lists[LOW_VALUES_INDEX], GINT_TO_POINTER((gint)(current_low_value * 1000)));
			lists[HIGH_VALUES_INDEX] = g_slist_prepend(lists[HIGH_VALUES_INDEX], GINT_TO_POINTER((gint)(current_high_value * 1000)));
			lists[ALARM_ENABLES_INDEX] = g_slist_prepend(lists[ALARM_ENABLES_INDEX], GINT_TO_POINTER(current_alarm_enable));
			lists[LOW_ALARM_COMMANDS_INDEX] = g_slist_prepend(lists[LOW_ALARM_COMMANDS_INDEX], current_low_alarm_command);
			lists[HIGH_ALARM_COMMANDS_INDEX] = g_slist_prepend(lists[HIGH_ALARM_COMMANDS_INDEX], current_high_alarm_command);
			lists[ALARM_TIMEOUTS_INDEX] = g_slist_prepend(lists[ALARM_TIMEOUTS_INDEX], GINT_TO_POINTER(current_alarm_timeout));
			lists[SENSOR_TYPES_INDEX] = g_slist_prepend(lists[SENSOR_TYPES_INDEX], GUINT_TO_POINTER(current_sensor_type));
			lists[MULTIPLIERS_INDEX] = g_slist_prepend(lists[MULTIPLIERS_INDEX], GINT_TO_POINTER((gint)(current_multiplier * 1000)));
			lists[OFFSETS_INDEX] = g_slist_prepend(lists[OFFSETS_INDEX], GINT_TO_POINTER((gint)(current_offset * 1000)));
			lists[ICON_TYPES_INDEX] = g_slist_prepend(lists[ICON_TYPES_INDEX], GUINT_TO_POINTER(current_icon_type));
			lists[GRAPH_COLORS_INDEX] = g_slist_prepend(lists[GRAPH_COLORS_INDEX], current_graph_color);
		}
	}

	/* keep lists in original order */
        for (i = 0; i < NUM_KEYS; i++) {
                if (lists[i] != NULL) {
                        lists[i] = g_slist_reverse(lists[i]);
                        
                        panel_applet_gconf_set_list(sensors_applet->applet, 
                                                    keys[i], 
                                                    key_types[i],
                                                    lists[i], &error);
                        if (error) {
                                sensors_applet_notify(sensors_applet, GCONF_WRITE_ERROR);
                
                                g_error_free(error);
                                return FALSE;
                        }
                } else {
                        g_debug("list %s is NULL", keys[i]);
                }
                        
        }

	sensors_applet_gconf_free_lists(lists,
                                        NUM_KEYS);

        /* store current version to identify config data */
        panel_applet_gconf_set_string(sensors_applet->applet,
                                      SENSORS_APPLET_VERSION,
                                      PACKAGE_VERSION, &error);

	return TRUE;
}

