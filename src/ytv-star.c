/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-star.c - Draws a filled star
 * Copyright (C) 2008 Víctor Manuel Jáquez Leal <vjaquez@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * SECTION:ytv-star
 * @short_description: Draws a filled star
 *
 * The #YtvStar draws an area with a star filled according a rank from
 * 0 to 1
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-star.h>

#include <glib.h>
#include <gtk/gtk.h>

#include <math.h>

enum _YtvStarProperties
{
        PROP_0,
        PROP_PERCENTAGE
};

typedef struct _YtvPoint YtvPoint;

struct _YtvPoint
{
        gfloat x, y;
};

typedef struct _YtvStarPriv YtvStarPriv;

struct _YtvStarPriv
{
        gfloat percentage;
        GArray* star_points;
};

#define YTV_STAR_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_STAR, YtvStarPriv))

G_DEFINE_TYPE (YtvStar, ytv_star, GTK_TYPE_DRAWING_AREA)

static void
calculate_star_points (GArray* star_points)
{
        gint i;
        gfloat incr, rad;
        YtvPoint* point;

        if (star_points->len > 0)
        {
                return;
        }

        incr = -2 * G_PI / 5;

        for (i = 0; i < 6; i++)
        {
                rad = i * incr - G_PI_2;

                point = g_slice_new0 (YtvPoint);

                point->x = cos (rad);
                point->y = sin (rad);

                g_array_append_val (star_points, point);
                
                rad += incr / 2;

                point = g_slice_new0 (YtvPoint);

                point->x = cos (rad) / 2;
                point->y = sin (rad) / 2;

                g_array_append_val (star_points, point);
        }

        return;
}

static void
draw (GtkWidget* self, cairo_t* cr)
{
        YtvStarPriv* priv;
        gdouble width, height, side;
        YtvPoint* point;
        gint i, num_points;

        priv = YTV_STAR_GET_PRIVATE (self);

        if (priv->star_points->len == 0)
        {
                calculate_star_points (priv->star_points);
        }

        width = self->allocation.width;
        height = self->allocation.height;

        cairo_translate (cr, width / 2.0, height / 1.85);
        side = MIN (width, height);
        cairo_scale (cr, side / 2.0, side / 2.0);

        cairo_save (cr); /* stack-pen-size */

        cairo_set_source_rgb (cr, 1.0, 0.75, 0.25);
        cairo_set_line_width (cr, 0.1);
        cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);

        point = g_array_index (priv->star_points, YtvPoint*, 0);
        cairo_move_to (cr, point->x, point->y);

        for (i = 1; i < priv->star_points->len; i++)
        {
                point = g_array_index (priv->star_points, YtvPoint*, i);
                cairo_line_to (cr, point->x, point->y);
        }

        cairo_stroke_preserve (cr);
        cairo_clip (cr);

        if (priv->percentage != 1.0)
        {
                cairo_set_source_rgb (cr, 1, 1, 1);
                cairo_rectangle (cr, -1, -1, 2, 2);
                cairo_fill (cr);
                cairo_set_source_rgb (cr, 1.0, 0.75, 0.25);
                
                num_points = 1 + rint (priv->percentage / 0.1);

                if (num_points != 1)
                {
                        cairo_move_to (cr, 0, 0);

                        for (i = 0; i < num_points; i++)
                        {
                                point = g_array_index (priv->star_points,
                                                       YtvPoint*, i);
                                cairo_line_to (cr, point->x, point->y);
                        }
                }
        }
        else
        {
                cairo_set_source_rgb (cr, 1.0, 0.75, 0.25);
                cairo_rectangle (cr, -1, -1, 2, 2);
                cairo_fill (cr);
        }
        
        cairo_fill (cr);
        cairo_restore (cr);

        return;
}

static void
redraw_canvas (YtvStar* self)
{
        GtkWidget* widget;
        GdkRegion* region;

        widget = GTK_WIDGET (self);

        if (!widget->window)
                return;

        region = gdk_drawable_get_clip_region (widget->window);
        /* redraw the cairo canvas completly by exposing it */
        gdk_window_invalidate_region (widget->window, region, TRUE);
        gdk_window_process_updates (widget->window, TRUE);

        gdk_region_destroy (region);

        return;
}

void
set_percentage (YtvStar* self, gfloat percentage)
{
        YtvStarPriv* priv = YTV_STAR_GET_PRIVATE (self);

        priv->percentage = percentage;
        redraw_canvas (self);
        g_object_notify (G_OBJECT (self), "percentage");

        return;
}

gfloat
get_percentage (YtvStar* self)
{
        YtvStarPriv* priv = YTV_STAR_GET_PRIVATE (self);

        return priv->percentage;
}

static gboolean
ytv_star_expose_event (GtkWidget* self, GdkEventExpose* event)
{
        cairo_t* cr;

        cr = gdk_cairo_create (self->window);

        cairo_rectangle (cr,
                         event->area.x, event->area.y,
                         event->area.width, event->area.height);

        cairo_clip (cr);

        draw (self, cr);

        cairo_destroy (cr);

        return FALSE;
}

static void
ytv_star_size_request (GtkWidget* widget, GtkRequisition* requisition)
{
        if (requisition->width < 24)
        {
                requisition->width = 24;
        }

        if (requisition->height < 24)
        {
                requisition->height = 24;
        }

        return;
}

static void
ytv_star_set_property (GObject* object, guint prop_id,
                       const GValue* value, GParamSpec* spec)
{

        switch (prop_id)
        {
        case PROP_PERCENTAGE:
                set_percentage (YTV_STAR (object), g_value_get_float (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_star_get_property (GObject* object, guint prop_id,
                       GValue* value, GParamSpec* spec)
{
        switch (prop_id)
        {
        case PROP_PERCENTAGE:
                g_value_set_float (value, get_percentage (YTV_STAR (object)));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_star_finalize (GObject* object)
{
        YtvStarPriv* priv;
        gint i;
        YtvPoint* point;

        priv = YTV_STAR_GET_PRIVATE (object);

        for (i = 0; i < priv->star_points->len; i++)
        {
                point = g_array_index (priv->star_points, YtvPoint*, i);
                g_slice_free (YtvPoint, point);
        }
        
        g_array_free (priv->star_points, TRUE);

	(*G_OBJECT_CLASS (ytv_star_parent_class)->finalize) (object);

        return;
}

static void
ytv_star_init (YtvStar* self)
{
        YtvStarPriv* priv = YTV_STAR_GET_PRIVATE (self);

        priv->percentage = 0;
        priv->star_points = g_array_sized_new (FALSE, FALSE,
                                               sizeof (YtvPoint*), 12);

        return;
}

static void
ytv_star_class_init (YtvStarClass* klass)
{
        GObjectClass* object_class = G_OBJECT_CLASS (klass);
        GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (klass);

        g_type_class_add_private (object_class, sizeof (YtvStarPriv));

        object_class->set_property = ytv_star_set_property;
        object_class->get_property = ytv_star_get_property;
        object_class->finalize     = ytv_star_finalize;
        
        widget_class->expose_event = ytv_star_expose_event;
        widget_class->size_request = ytv_star_size_request; 
        
        g_object_class_install_property
                (object_class, PROP_PERCENTAGE,
                 g_param_spec_float
                 ("percentage", "percentage",
                  "the area percentage to fill", 0.0, 1.0, 0.0,
                  G_PARAM_READWRITE));

        return;
}

/**
 * ytv_star_new:
 * @percentage: The filled star percentage
 *
 * Generate a widget with an image of a filled star
 *
 * return value: a #YtvStar
 */
GtkWidget*
ytv_star_new (gfloat percentage)
{
        return GTK_WIDGET (g_object_new (YTV_TYPE_STAR,
                                         "percentage", percentage, NULL));
}
