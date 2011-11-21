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

#ifndef SENSORS_APPLET_GCONF_H
#define SENSORS_APPLET_GCONF_H

#include "sensors-applet.h"
#include <panel-applet-gconf.h>

#define FONT_SIZE "font-size" /* hidden gconf option specifying font
                               * size in points */

#define HIDE_UNITS "hide-units" /* hidden gconf option specifying not to
                                 * show sensor units */

#define IS_SETUP "setup"  /* have we actually set up this instance of
			   * the applet (gets set to true after
			   * closing prefences dialog) */

#define DISPLAY_MODE "display_mode" /* display icons or text labels?*/

#define LAYOUT_MODE "layout_mode" /* value beside or below label */
#define TEMPERATURE_SCALE "temperature_scale" /* Kelvin,
                                                 Celsius or
                                                 Fahrenheit */
#define DISPLAY_NOTIFICATIONS "display_notifications" /* whether to
                                                       * display
                                                       * notifications */
#define TIMEOUT "timeout_delay" /* delay (in ms) between refreshes */
#define GRAPH_SIZE "graph_size" /* the size of the graph in pixels -
                                 * either width if horizontal, or
                                 * height if vertical */
#define PATHS "paths" /* full paths to filenames */
#define IDS "ids" /* a list of the sensor device ids */
#define INTERFACES "interfaces" /* a list of the sensor device
				 * interface for each sensor */
#define LABELS "labels"  /* user defined labels for each sensor */
#define ENABLES "sensor_enables" /* list of booleans corresponding to
				  * the filenames of whether a sensor
				  * is enabled or not */
#define LOW_VALUES "low_values" /* stored as ints (1000 * double
				     * value) for accuracy, since can
				     * only do ints easily */
#define HIGH_VALUES "high_values" /* stored as ints (1000 * double
				     * value) for accuracy, since can
				     * only do ints easily */
#define ALARM_ENABLES "alarm_enables" /* list of whether each sensor
				       * has its alarm enabled */
#define LOW_ALARM_COMMANDS "low_alarm_commands" /* list of commands to execute
                                                 * when each alarm is
                                                 * activated */
#define HIGH_ALARM_COMMANDS "high_alarm_commands" /* list of commands to execute
                                                 * when each alarm is
                                                 * activated */

#define ALARM_TIMEOUTS "alarm_timeouts" /* list of how often each
					   alarm should be sounded (in
					   seconds) */

#define SENSOR_TYPES "sensor_types" /* used to identify a sensor in a
				       list */

#define MULTIPLIERS "multipliers"
#define OFFSETS "offsets"
#define ICON_TYPES "icon_types"
#define GRAPH_COLORS "graph_colors"

#define SENSORS_APPLET_VERSION "sensors_applet_version" /* version of
                                                         * config
                                                         * data */

gboolean sensors_applet_gconf_save_sensors(SensorsApplet *sensors_applet);
gboolean sensors_applet_gconf_setup_sensors(SensorsApplet *sensors_applet);
void sensors_applet_gconf_setup(SensorsApplet *sensors_applet);

#endif /* SENSORS_APPLET_GCONF_H*/
