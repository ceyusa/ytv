/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-rank.c - Draws the entry rank with filled stars
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
 * SECTION:ytv-rank
 * @short_description: Draws the entry rank with filled stars
 *
 * The #YtvRank draws an area with a filled stars according to a
 * rank from 0.0 to 5.0
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-rank.h>
#include <ytv-star.h>

#include <glib.h>
#include <gtk/gtk.h>

enum _YtvRankProperties
{
        PROP_0,
        PROP_RANK
};

typedef struct _YtvRankPriv YtvRankPriv;

struct _YtvRankPriv
{
        gfloat rank;
        GtkWidget* stars[5];
};

#define YTV_RANK_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_RANK, YtvRankPriv))

G_DEFINE_TYPE (YtvRank, ytv_rank, GTK_TYPE_HBOX)

static void
draw (YtvRank* self)
{
        YtvRankPriv* priv = YTV_RANK_GET_PRIVATE (self);
        gfloat restrank = priv->rank;
        gint i;

        for (i = 0; i < 5; i++)
        {
                if (priv->stars[i] == NULL)
                {
                        priv->stars[i] = g_object_new (YTV_TYPE_STAR, NULL);
                        gtk_box_pack_start (GTK_BOX (self),
                                            priv->stars[i], TRUE, TRUE, 0);
                }
                
                if (restrank >= 1.0)
                {
                        g_object_set (priv->stars[i], "percentage", 1.0, NULL);
                        restrank -= 1;
                }
                else if (restrank > 0.0)
                {
                        g_object_set (priv->stars[i],
                                      "percentage", restrank, NULL);
                        restrank = 0;
                }
                else if (restrank == 0)
                {
                        g_object_set (priv->stars[i], "percentage", 0.0, NULL);
                }
                else
                {
                        g_assert_not_reached ();
                }
        }

        return;
}

static void
set_rank (YtvRank* self, gfloat rank)
{
        YtvRankPriv* priv = YTV_RANK_GET_PRIVATE (self);

        priv->rank = rank;
        draw (self);
        g_object_notify (G_OBJECT (self), "rank");

        return;
}

static gboolean
ytv_rank_expose_event (GtkWidget* self, GdkEventExpose* event)
{
        draw (YTV_RANK (self));
        
        return FALSE;
}

static void
ytv_rank_set_property (GObject* object, guint prop_id,
                       const GValue* value, GParamSpec* spec)
{
        switch (prop_id)
        {
        case PROP_RANK:
                set_rank (YTV_RANK (object), g_value_get_float (value));
                break;
        default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
		break;
        }

        return;
}

static void
ytv_rank_get_property (GObject* object, guint prop_id,
                       GValue* value, GParamSpec* spec)
{
        YtvRankPriv* priv = YTV_RANK_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_RANK:
                g_value_set_float (value, priv->rank);
                break;
        default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
		break;
        }

        return;
}

static void
ytv_rank_init (YtvRank* self)
{
        YtvRankPriv* priv = YTV_RANK_GET_PRIVATE (self);

        priv->rank = 0;

        gint i;
        for (i = 0; i < 6; i++)
        {
                priv->stars[i] = NULL;
        }

        return;
}

static void
ytv_rank_class_init (YtvRankClass* klass)
{
        GObjectClass* object_class = G_OBJECT_CLASS (klass);
        GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (klass);

        g_type_class_add_private (object_class, sizeof (YtvRankPriv));

        object_class->set_property = ytv_rank_set_property;
        object_class->get_property = ytv_rank_get_property;

        /* widget_class->expose_event = ytv_rank_expose_event; */

        g_object_class_install_property
                (object_class, PROP_RANK,
                 g_param_spec_float
                 ("rank", "rank", "the area to fill", 0.0, 5.0, 0.0,
                  G_PARAM_READWRITE));

        return;
}

/**
 * ytv_rank_new:
 * @rank: The filled rank percentage
 *
 * Generate a widget with an image of a filled stars representing rank
 *
 * return value: a #YtvRank
 */
GtkWidget*
ytv_rank_new (gfloat rank)
{
        return GTK_WIDGET (g_object_new (YTV_TYPE_RANK, "rank", rank, NULL));
}
