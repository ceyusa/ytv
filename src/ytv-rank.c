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

struct _YtvRankPriv
{
        gfloat rank;
};

#define YTV_RANK_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_RANK, YtvRankPriv))

G_DEFINE_TYPE (YtvRank, ytv_rank, GTK_TYPE_HBOX)

static gboolean
ytv_rank_expose (GtkWidget* self, GdkEventExpose* event)
{
        YtvRankPriv* priv = YTV_RANK_GET_PRIVATE (object);

        gint integerpart = rint (priv->rank);
        gfloat fractionpart = intergerpart - priv->rank;

        GtkWidget* star;

        gint i;
        for (i = 0; i < integerpart; i++)
        {
                star = ytv_star_new (1.0);
                gtk_widget_set_size_request (star, 24, 24);
                gtk_box_pack_start (GTK_BOX (self), star, FALSE, FALSE, 0);
        }

        if (fractionpart > 0)
        {
                star = ytv_star_new (fractionpart);
                gtk_widget_set_size_request (star, 24, 24);
                gtk_box_pack_start (GTK_BOX (self), star, FALSE, FALSE, 0);
        }

        return (*GTK_WIDGET_CLASS (ytv_rank_parent_class)->expose_event)
                (self, event);
}

static void
ytv_rank_set_property (GObject* object, guint prop_id,
                       const GValue* value, GParamSpec* spec)
{
        YtvRankPriv* priv = YTV_RANK_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_RANK:
                priv->rank = g_value_get_float (value);
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
        widget_class->expose_event = ytv_rank_expose;
        
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
