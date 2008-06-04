/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-thumbnail.c - show an image thumbnail
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
 * SECTION:ytv-thumbnail
 * @short_description: Draws an image thumbnail
 *
 * Will display an imange thumbnail keeping the same size
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <ytv-thumbnail.h>

typedef struct _YtvThumbnailPriv YtvThumbnailPriv;

struct _YtvThumbnailPriv
{
        GdkColor *normal_color;
        GdkColor *selected_color;
};

#define YTV_THUMBNAIL_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_THUMBNAIL, YtvThumbnailPriv))

G_DEFINE_TYPE (YtvThumbnail, ytv_thumbnail, GTK_TYPE_ALIGNMENT)

static void
on_realize (GtkWidget* widget, gpointer user_data)
{
        g_return_if_fail (YTV_IS_THUMBNAIL (user_data));

        YtvThumbnail* me = YTV_THUMBNAIL (user_data);
        YtvThumbnailPriv* priv = YTV_THUMBNAIL_GET_PRIVATE (me);

        if (priv->normal_color != NULL)
        {
                GtkStyle* style = gtk_widget_get_style (widget);
                priv->normal_color =
                        gdk_color_copy (&style->fg [GTK_STATE_NORMAL]);
        }

        return;
}

static gboolean
on_focus_in (GtkWidget* widget, GdkEventFocus* event, gpointer user_data)
{
        g_return_val_if_fail (YTV_IS_THUMBNAIL (widget), FALSE);

        YtvThumbnail* me = YTV_THUMBNAIL (widget);
        YtvThumbnailPriv* priv = YTV_THUMBNAIL_GET_PRIVATE (widget);

        if (priv->selected_color != NULL)
        {
                gdk_color_free (priv->selected_color);
                priv->selected_color = NULL;
        }

        GtkStyle* style = gtk_widget_get_style (GTK_WIDGET (me->evbox));
        priv->selected_color = gdk_color_copy (&style->fg [GTK_STATE_SELECTED]);

        gtk_widget_modify_bg (GTK_WIDGET (me->evbox),
                              GTK_STATE_NORMAL, priv->selected_color);

        return FALSE;
}

static gboolean
on_focus_out (GtkWidget* widget, GdkEventFocus* event, gpointer user_data)
{
        g_return_val_if_fail (YTV_IS_THUMBNAIL (widget), FALSE);

        YtvThumbnail* me = YTV_THUMBNAIL (widget);
        YtvThumbnailPriv* priv = YTV_THUMBNAIL_GET_PRIVATE (me);

        gtk_widget_modify_bg (GTK_WIDGET (me->evbox),
                              GTK_STATE_NORMAL, priv->normal_color);

        return FALSE;
}

static void
ytv_thumbnail_finalize (GObject* object)
{
        YtvThumbnailPriv* priv = YTV_THUMBNAIL_GET_PRIVATE (object);

        if (priv->normal_color != NULL)
        {
                gdk_color_free (priv->normal_color);
                priv->normal_color = NULL;
        }

        if (priv->selected_color != NULL)
        {
                gdk_color_free (priv->selected_color);
                priv->selected_color = NULL;
        }

        return;
}

static void
ytv_thumbnail_class_init (YtvThumbnailClass* klass)
{
        GObjectClass* object_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (object_class, sizeof (YtvThumbnailPriv));

        object_class->finalize = ytv_thumbnail_finalize;

        return;
}

static void
ytv_thumbnail_init (YtvThumbnail* self)
{
        YtvThumbnailPriv* priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        g_object_set (G_OBJECT (self), "xalign", 0.5, NULL);

        self->button = gtk_button_new ();
        gtk_button_set_relief (GTK_BUTTON (self->button), GTK_RELIEF_NONE);
        g_signal_connect_swapped (G_OBJECT (self->button), "focus-in-event",
                                  G_CALLBACK (on_focus_in), self);
        g_signal_connect_swapped (G_OBJECT (self->button), "focus-out-event",
                                  G_CALLBACK (on_focus_out), self);
        gtk_container_add (GTK_CONTAINER (self), self->button);

        self->evbox = gtk_event_box_new ();
        gtk_event_box_set_above_child (GTK_EVENT_BOX (self->evbox), TRUE);
        g_signal_connect (G_OBJECT (self->evbox),
                          "realize", G_CALLBACK (on_realize), self);
        gtk_container_add (GTK_CONTAINER (self->button), self->evbox);

        self->image = gtk_image_new_from_stock (GTK_STOCK_MISSING_IMAGE,
                                                GTK_ICON_SIZE_DIALOG);
        g_object_set (G_OBJECT (self->image), "xpad", 6, "ypad", 6, NULL);
        gtk_container_add (GTK_CONTAINER (self->evbox), self->image);

        priv->selected_color = NULL;
        priv->normal_color   = NULL;

        return;
}

GtkWidget*
ytv_thumbnail_new (void)
{
        return g_object_new (YTV_TYPE_THUMBNAIL, NULL);
}
