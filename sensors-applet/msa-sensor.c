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

#include "msa-sensor.h"

#include <syslog.h>

/* class struct */
struct _MSASensor {
  GObject parent_instance;

    /* Other members, including private data. */
    /* msa_sensor_set_property () frees strings before setting them */

    gchar *path;
    gchar *id;
    gchar *label;
    SensorType type; 
    gboolean enable;
    gdouble low_value;
    gdouble high_value;
    gdouble multiplier;
    gdouble offset;
    IconType icon;
    gchar *graph_color;
};

/* convenience macro for type implementations */
G_DEFINE_TYPE (MSASensor, msa_sensor, G_TYPE_OBJECT)

/* MSASensor properties */
enum {
    /* must start with 1 */
    PROP_PATH = 1,
    PROP_ID,
    PROP_LABEL,
    PROP_SENSOR_TYPE,
    PROP_ENABLE,
    PROP_LOW_VALUE,
    PROP_HIGH_VALUE,
    PROP_MULTIPLIER,
    PROP_OFFSET,
    PROP_ICON_TYPE,
    PROP_GRAPH_COLOR,

    /* enum size */
    N_PROPERTIES
};

/* properties specification */
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };


/* must be implemented as there is no default version */
static void msa_sensor_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {

    MSASensor *self = MSA_SENSOR (object);

    switch (property_id)
    {
        case PROP_PATH:
            g_value_set_string (value, self->path);
            break;

        case PROP_ID:
            g_value_set_string (value, self->id);
            break;

        case PROP_LABEL:
            g_value_set_string (value, self->label);
            break;

        case PROP_SENSOR_TYPE:
            g_value_set_enum (value, self->type);
            break;

        case PROP_ENABLE:
            g_value_set_boolean (value, self->enable);
            break;

        case PROP_LOW_VALUE:
            g_value_set_double (value, self->low_value);
            break;

        case PROP_HIGH_VALUE:
            g_value_set_double (value, self->high_value);
            break;

        case PROP_MULTIPLIER:
            g_value_set_double (value, self->multiplier);
            break;

        case PROP_OFFSET:
            g_value_set_double (value, self->offset);
            break;

        case PROP_ICON_TYPE:
            g_value_set_enum (value, self->icon);
            break;

        case PROP_GRAPH_COLOR:
            g_value_set_string (value, self->graph_color);
            break;

        default:
            /* We don't have any other property... */
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }

}

/* must be implemented as there is no default version */
static void msa_sensor_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {

    MSASensor *self = MSA_SENSOR (object);

    switch (property_id)
    {
        case PROP_PATH:
            g_free (self->path);
            self->path = g_value_dup_string (value);
            break;

        case PROP_ID:
            g_free (self->id);
            self->id = g_value_dup_string (value);
            break;

        case PROP_LABEL:
            g_free (self->label);
            self->label = g_value_dup_string (value);
            break;

        case PROP_SENSOR_TYPE:
            self->type = g_value_get_enum (value);
            break;

        case PROP_ENABLE:
            self->enable = g_value_get_boolean (value);
            break;

        case PROP_LOW_VALUE:
            self->low_value = g_value_get_double (value);
            break;

        case PROP_HIGH_VALUE:
            self->high_value = g_value_get_double (value);
            break;

        case PROP_MULTIPLIER:
            self->multiplier = g_value_get_double (value);
            break;

        case PROP_OFFSET:
            self->offset = g_value_get_double (value);
            break;

        case PROP_ICON_TYPE:
            self->icon = g_value_get_enum (value);
            break;

        case PROP_GRAPH_COLOR:
            g_free (self->graph_color);
            self->graph_color = g_value_dup_string (value);
            break;

        default:
            /* We don't have any other property... */
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }

}

/* used to clean up private data */
//static void msa_sensor_dispose (GObject *object) {
    /*MSASensorPrivate *priv = msa_sensor_get_instance_private (MSA_SENSOR (object));*/

    /* In dispose(), you are supposed to free all types referenced from this
    * object which might themselves hold a reference to self. Generally,
    * the most simple solution is to unref all members on which you own a 
    * reference.
    */

    /* dispose() might be called multiple times, so we must guard against
    * calling g_object_unref() on an invalid GObject by setting the member
    * NULL; g_clear_object() does this for us.
    */
    /*g_clear_object (&priv->input_stream);*/

    /* Always chain up to the parent class; there is no need to check if
    * the parent class implements the dispose() virtual function: it is
    * always guaranteed to do so
    */
//    G_OBJECT_CLASS (msa_sensor_parent_class)->dispose (object);
//}

static void msa_sensor_finalize (GObject *object) {
    /*MSASensorPrivate *priv = msa_sensor_get_instance_private (MSA_SENSOR (object));*/

    /*g_free (priv->filename);*/

    MSASensor *self = MSA_SENSOR (object);

    g_free (self->path);
    g_free (self->id);
    g_free (self->label);
    g_free (self->graph_color);


    /* Always chain up to the parent class; as with dispose(), finalize()
    * is guaranteed to exist on the parent's class virtual function table
    */
    G_OBJECT_CLASS (msa_sensor_parent_class)->finalize (object);
}


/* used by G_DEFINE_TYPE */
static void msa_sensor_class_init (MSASensorClass *klass) {

    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    /* name (const gchar *), nick (const gchar *), description (const gchar *), default value (const gchar *), flags (GParamFlags) */
    obj_properties[PROP_PATH] = g_param_spec_string ("path", "Path", "Path", NULL, G_PARAM_READWRITE);

    obj_properties[PROP_ID] = g_param_spec_string ("id", "ID", "ID", NULL, G_PARAM_READWRITE);

    obj_properties[PROP_LABEL] = g_param_spec_string ("label", "Label", "Label", NULL, G_PARAM_READWRITE);

    /* name, nick, description, enum type (GType), default value (gint), flags */
    obj_properties[PROP_SENSOR_TYPE] = g_param_spec_enum ("sensor-type", "Sensor type", "Sensor type", MSA_TYPE_SENSOR_TYPE, 0, G_PARAM_READWRITE);

    /* name, nick, description, default value (gboolean), flags */
    obj_properties[PROP_ENABLE] = g_param_spec_boolean ("enable", "Enable", "Enable", FALSE, G_PARAM_READWRITE);

    /* name, nick, description, min (gdouble), max (gdouble), default value (gdouble), flags */
    obj_properties[PROP_LOW_VALUE] = g_param_spec_double ("low-value", "Low value", "Low value", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE);

    obj_properties[PROP_HIGH_VALUE] = g_param_spec_double ("high-value", "High value", "High value", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE);

    obj_properties[PROP_MULTIPLIER] = g_param_spec_double ("multiplier", "Multiplier", "Multiplier", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE);

    obj_properties[PROP_OFFSET] = g_param_spec_double ("offset", "Offset", "Offset", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE);

    /* name, nick, description, enum type (GType), default value (gint), flags */
    /* 5 is a generic icon */
    obj_properties[PROP_ICON_TYPE] = g_param_spec_enum ("icon-type", "Icon type", "Icon type", MSA_TYPE_ICON_TYPE, 5, G_PARAM_READWRITE);

    obj_properties[PROP_GRAPH_COLOR] = g_param_spec_string ("graph-color", "Graph color", "Graph color", NULL, G_PARAM_READWRITE);


    /* set custom class methods */
    gobject_class->get_property = msa_sensor_get_property;
    gobject_class->set_property = msa_sensor_set_property;
//    gobject_class->dispose = msa_sensor_dispose;
    gobject_class->finalize = msa_sensor_finalize;

    g_object_class_install_properties (gobject_class, N_PROPERTIES, obj_properties);
}

/* used by G_DEFINE_TYPE */
static void msa_sensor_init (MSASensor *self) {

    /* initialize all public and private members to reasonable default values.
    * They are all automatically initialized to 0 to begin with. */
//    self->path = g_strdup ("testpath");
}

/* create new but empty sensor */
MSASensor *msa_sensor_new (void) {

  return g_object_new (MSA_TYPE_SENSOR, NULL);
}

