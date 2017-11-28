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
//G_DECLARE_FINAL_TYPE (MSASensor, msa_sensor, MSA, SENSOR, GObject)
// derivable to be able to add class functions
//G_DECLARE_DERIVABLE_TYPE (MSASensor, msa_sensor, MSA, SENSOR, GObject)
// can't use macro, as it defines ‘struct _MSASensor’, so do things manually
GType msa_sensor_get_type (void);
typedef struct _MSASensorClass MSASensorClass;
typedef struct _MSASensor MSASensor;
typedef struct _MSASensorPrivate MSASensorPrivate;
#define MSA_SENSOR(inst)            (G_TYPE_CHECK_INSTANCE_CAST ((inst), MSA_TYPE_SENSOR, MSASensor))
#define MSA_SENSOR_CLASS(class)     (G_TYPE_CHECK_CLASS_CAST ((class), MSA_TYPE_SENSOR, MSASensorClass))
#define MSA_IS_SENSOR(inst)         (G_TYPE_CHECK_INSTANCE_TYPE ((inst), MSA_TYPE_SENSOR))
#define MSA_IS_SENSOR_CLASS(class)  (G_TYPE_CHECK_CLASS_TYPE ((class), MSA_TYPE_SENSOR))
#define MSA_SENSOR_GET_CLASS(inst)  (G_TYPE_INSTANCE_GET_CLASS ((inst), MSA_TYPE_SENSOR, MSASensorClass))

/* class struct */
struct _MSASensorClass {
    GObjectClass parent_class;

    void (*printhello) (void);

    /* Padding to allow adding up to 10 new virtual functions without breaking ABI. */
    gpointer padding[10];
};

/* instance struct */
struct _MSASensor {
    GObject parent_instance;

    MSASensorPrivate *priv;
};

/*
 * Method definitions.
 */

static void msa_sensor_printhello (void);


MSASensor *msa_sensor_new (void);

G_END_DECLS

#endif /* MSA_SENSOR_H */
