/*
 * Copyright (C) 2009 Jaap Versteegh <j.r.versteegh@gmail.com>
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

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif /* HAVE_STDIO_H */

#ifdef HAVE_TIME_H
#include <time.h>
#endif /* HAVE_TIME_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <glib.h>
#include <glib/gi18n.h>
#include "aticonfig-plugin.h"

const gchar *plugin_name = "aticonfig";

#define GPU_CORE_TEMP "CoreTemp"
#define MAX_GPUS 4
#define SENSOR_ID_PREFIX "ATIGPU"

static gdouble gpu_temps[MAX_GPUS];
static int num_gpus = 0;

static int ati_get_temps(gdouble temps[], int max_temps)
{
  double temp;
  int read_count;
  int gpu_no = 0;
#ifdef HAVE_STDIO_H
  FILE *aticonfig = popen(ATICONFIG_EXE
      " --adapter=all --od-gettemperature", "r");
  if (aticonfig == NULL) {
    return 0;
  }
  while ((read_count = fscanf(aticonfig, "Temperature - %lf", &temp)) != EOF) {
    if (read_count < 1) {
      getc(aticonfig);
    } 
    else {
      temps[gpu_no] = (gdouble)temp;
      if (++gpu_no >= max_temps) 
	break;
    }
  } 
  pclose(aticonfig); 
#endif
  return gpu_no; 
}

static void ati_update_temps(void)
{
#ifdef HAVE_TIME_H
  static time_t last = 0;
  time_t now = time(NULL);
  /* Only update when more than two seconds have passed since last update */
  if (timediff(now, last) > 2) {
#endif
    num_gpus = ati_get_temps(&gpu_temps, MAX_GPUS);
#ifdef HAVE_TIME_H
    last = now;
  }
#endif
}


static GList *aticonfig_plugin_init(void) 
{
  GList *sensors = NULL;

  g_debug("Initializing aticonfig plugin\n"); 

  int sensor_count = ati_get_temps(&gpu_temps, MAX_GPUS);
 
  int i;
  for (i = 0; i < sensor_count; i++) {
    gchar *id = g_strdup_printf("%s%d%s", SENSOR_ID_PREFIX, i, GPU_CORE_TEMP);
    sensors_applet_plugin_add_sensor(&sensors,
	GPU_CORE_TEMP,
	id,
	_("GPU"),
	TEMP_SENSOR,
	TRUE,
	GPU_ICON,
	DEFAULT_GRAPH_COLOR);
    g_free(id);
  }
  return sensors;
}

static gdouble aticonfig_plugin_get_sensor_value(const gchar *path, 
                                                const gchar *id, 
                                                SensorType type,
                                                GError **error) 
{

  if (g_ascii_strcasecmp(path, GPU_CORE_TEMP) != 0 || type != TEMP_SENSOR) {
    g_set_error(error, SENSORS_APPLET_PLUGIN_ERROR,
          0, "Invalid sensor value request to aticonfig plugin");
    return 0;
  }
  ati_update_temps();

  int i = g_ascii_strtoll(id + strlen(SENSOR_ID_PREFIX), NULL, 10);
  if (i < 0 || i >= num_gpus) {
    g_set_error(error, SENSORS_APPLET_PLUGIN_ERROR,
          0, "Sensor index out of range in aticonfig plugin");
    return 0;
  }
  return gpu_temps[i];
}

const gchar *sensors_applet_plugin_name(void) 
{
  return plugin_name;
}

GList *sensors_applet_plugin_init(void) 
{
  return aticonfig_plugin_init();
}

gdouble sensors_applet_plugin_get_sensor_value(const gchar *path, 
                                                const gchar *id, 
                                                SensorType type,
                                                GError **error) 
{
  return aticonfig_plugin_get_sensor_value(path, id, type, error);
}
