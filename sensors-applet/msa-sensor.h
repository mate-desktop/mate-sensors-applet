/*
 * Copyright (C) 2017 CPPSP Info <info@cppsp.de>
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

#ifndef MSA_SENSOR_H
#define MSA_SENSOR_H

#include "msa-enum-types.h"

/* already linked */
/*#include <glib-object.h>*/

G_BEGIN_DECLS

/*
 * Type declaration.
 */
#define MSA_TYPE_SENSOR msa_sensor_get_type ()
G_DECLARE_FINAL_TYPE (MSASensor, msa_sensor, MSA, SENSOR, GObject)

/*
 * Method definitions.
 */
MSASensor *msa_sensor_new (void);

G_END_DECLS

#endif /* MSA_SENSOR_H */
