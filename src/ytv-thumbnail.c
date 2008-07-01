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

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <ytv-thumbnail.h>

#include <ytv-error.h>

enum _YtvThumbnailProp
{
        PROP_0,
        PROP_ID
};

enum _YtvThumbnailSignals
{
        CLICKED,
        LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

typedef struct _YtvThumbnailPriv YtvThumbnailPriv;

struct _YtvThumbnailPriv
{
        gchar* eid;
        YtvFeedFetchStrategy* fetcher;
        YtvUriBuilder* ub;

        GdkColor* normal_color;
        GdkColor* selected_color;

        GdkPixbuf* pixbuf;
};

#define YTV_THUMBNAIL_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_THUMBNAIL, YtvThumbnailPriv))

G_DEFINE_TYPE (YtvThumbnail, ytv_thumbnail, GTK_TYPE_ALIGNMENT)

static void
on_realize (GtkWidget* widget, gpointer user_data)
{
        YtvThumbnail* me;
        YtvThumbnailPriv* priv;
        GtkStyle* style;

        g_return_if_fail (YTV_IS_THUMBNAIL (user_data));

        me = YTV_THUMBNAIL (user_data);
        priv = YTV_THUMBNAIL_GET_PRIVATE (me);

        if (priv->normal_color == NULL)
        {
                style = gtk_widget_get_style (me->evbox);
                priv->normal_color =
                        gdk_color_copy (&style->bg [GTK_STATE_NORMAL]);
        }
        
        return;
}

static gboolean
on_focus_in (GtkWidget* widget, GdkEventFocus* event, gpointer user_data)
{
        YtvThumbnail* me;
        YtvThumbnailPriv* priv;
        GtkStyle* style;

        g_return_val_if_fail (YTV_IS_THUMBNAIL (widget), FALSE);

        me = YTV_THUMBNAIL (widget);
        priv = YTV_THUMBNAIL_GET_PRIVATE (widget);

        if (priv->selected_color != NULL)
        {
                gdk_color_free (priv->selected_color);
                priv->selected_color = NULL;
        }

        style = gtk_widget_get_style (GTK_WIDGET (me->evbox));
        priv->selected_color = gdk_color_copy (&style->bg [GTK_STATE_SELECTED]);

        gtk_widget_modify_bg (GTK_WIDGET (me->evbox),
                              GTK_STATE_NORMAL, priv->selected_color);

        return FALSE;
}

static gboolean
on_focus_out (GtkWidget* widget, GdkEventFocus* event, gpointer user_data)
{
        YtvThumbnail* me;
        YtvThumbnailPriv* priv;

        g_return_val_if_fail (YTV_IS_THUMBNAIL (widget), FALSE);

        me = YTV_THUMBNAIL (widget);
        priv = YTV_THUMBNAIL_GET_PRIVATE (me);

        if (priv->normal_color != NULL)
        {
                gtk_widget_modify_bg (GTK_WIDGET (me->evbox),
                                      GTK_STATE_NORMAL, priv->normal_color);
        }

        return FALSE;
}

static void
on_clicked (GtkWidget* widget, gpointer user_data)
{
        g_signal_emit (user_data, signals[CLICKED], 0);

        return;
}

static void
fetch_img_cb (YtvFeedFetchStrategy* st, const gchar* mime,
              const gint8* response, gssize length, GError **err,
              gpointer user_data)
{
        YtvThumbnail* self;
        YtvThumbnailPriv* priv;
        GError* error = NULL;
        GdkPixbufLoader* loader = NULL;
        GdkPixbuf* pixbuf = NULL;

        if (err != NULL && *err != NULL)
        {
                g_debug ("image fetching error: %s",
                         ytv_error_get_message (*err));

                g_error_free (*err);
                *err = NULL;

                return;
        }

        if (!YTV_IS_THUMBNAIL (user_data))
        {
                return;
        }

        self = YTV_THUMBNAIL (user_data);
        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        error  = NULL;
        loader = NULL;
        pixbuf = NULL;

        if (length == 0)
        {
                g_debug ("zero sized image");
                return;
        }

        if (mime != NULL)
        {
                loader = gdk_pixbuf_loader_new_with_mime_type (mime, &error);
        }

        if (error != NULL)
        {
                g_error_free (error);
                error = NULL;
        }

        if (loader == NULL)
        {
                loader = gdk_pixbuf_loader_new ();
        }

        if (!gdk_pixbuf_loader_write (loader, (guchar*) response,
                                      (gsize) length, &error))
        {
                g_debug ("image parsing error: %s",
                         ytv_error_get_message (error));
                g_error_free (error);
                error = NULL;

                goto beach;
        }

        if (!gdk_pixbuf_loader_close (loader, &error))
        {
                g_debug ("image parsing error: %s",
                         ytv_error_get_message (error));
                g_error_free (error);
                error = NULL;
        }

        pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);

        if (pixbuf != NULL)
        {
                if (priv->pixbuf != NULL)
                {
                        g_object_unref (priv->pixbuf);
                }
                
                priv->pixbuf = gdk_pixbuf_scale_simple (pixbuf, 130, 97,
                                                        GDK_INTERP_BILINEAR);

                gtk_image_set_from_pixbuf (GTK_IMAGE (self->image),
                                           priv->pixbuf);
        }
        else
        {
                g_debug ("Not enough data for image");
        }

beach:
        g_object_unref (loader); /* unref also the pixbuf */

        return;
}

static void
fetch_image (YtvThumbnail* self)
{
        gchar* uri;
        YtvThumbnailPriv* priv;

        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        g_return_if_fail (priv->fetcher != NULL);
        g_return_if_fail (priv->ub != NULL);
        g_return_if_fail (priv->eid != NULL);

        gtk_image_set_from_stock (GTK_IMAGE (self->image),
                                  GTK_STOCK_MISSING_IMAGE,
                                  GTK_ICON_SIZE_DIALOG);
        /* gtk_widget_set_size_request (self->image, 136, 103); */ /* 130+6x97+6 */
        
        uri = ytv_uri_builder_get_thumbnail (priv->ub, priv->eid);
        g_return_if_fail (uri != NULL);

        ytv_feed_fetch_strategy_perform (priv->fetcher,
                                         uri, fetch_img_cb, self);

        g_free (uri);

        return;
}

static void
ytv_thumbnail_set_property (GObject* object, guint prop_id,
                            const GValue* value, GParamSpec* spec)
{
        YtvThumbnailPriv* priv;
        priv = YTV_THUMBNAIL_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ID:
                ytv_thumbnail_set_id (YTV_THUMBNAIL (object),
                                      g_value_get_string (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_thumbnail_get_property (GObject* object, guint prop_id,
                            GValue* value, GParamSpec* spec)
{
        YtvThumbnailPriv* priv;
        priv = YTV_THUMBNAIL_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ID:
                g_value_set_string (value, priv->eid);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

	return;
}

static void
ytv_thumbnail_dispose (GObject* object)
{
        YtvThumbnailPriv* priv;
        priv = YTV_THUMBNAIL_GET_PRIVATE (object);

        (*G_OBJECT_CLASS (ytv_thumbnail_parent_class)->dispose) (object);

        if (priv->fetcher != NULL)
        {
                g_object_unref (priv->fetcher);
                priv->fetcher = NULL;
        }

        if (priv->ub != NULL)
        {
                g_object_unref (priv->ub);
                priv->ub = NULL;
        }

        if (priv->pixbuf != NULL)
        {
                g_object_unref (priv->pixbuf);
                priv->pixbuf = NULL;
        }

        return;
}

static void
ytv_thumbnail_finalize (GObject* object)
{
        YtvThumbnailPriv* priv;
        priv = YTV_THUMBNAIL_GET_PRIVATE (object);

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

        if (priv->eid != NULL)
        {
                g_free (priv->eid);
        }

        (*G_OBJECT_CLASS (ytv_thumbnail_parent_class)->finalize) (object);

        return;
}

static void
ytv_thumbnail_class_init (YtvThumbnailClass* klass)
{
        GObjectClass* object_class;
        object_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (object_class, sizeof (YtvThumbnailPriv));

        object_class->get_property = ytv_thumbnail_get_property;
        object_class->set_property = ytv_thumbnail_set_property;
        object_class->dispose      = ytv_thumbnail_dispose;
        object_class->finalize     = ytv_thumbnail_finalize;

        g_object_class_install_property
                (object_class, PROP_ID,
                 g_param_spec_string
                 ("id", "vid", "Video identificator string", NULL,
                  G_PARAM_READWRITE));

        signals[CLICKED] =
                g_signal_new ("clicked",
                              YTV_TYPE_THUMBNAIL,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvThumbnailClass,
                                               clicked),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);
                                               
        return;
}

static void
ytv_thumbnail_init (YtvThumbnail* self)
{
        YtvThumbnailPriv* priv;
        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        g_object_set (G_OBJECT (self), "xalign", 0.5, "yalign", 0.0, NULL);

        self->button = gtk_button_new ();        
        gtk_button_set_relief (GTK_BUTTON (self->button), GTK_RELIEF_NONE);
        g_signal_connect_swapped (G_OBJECT (self->button), "focus-in-event",
                                  G_CALLBACK (on_focus_in), self);
        g_signal_connect_swapped (G_OBJECT (self->button), "focus-out-event",
                                  G_CALLBACK (on_focus_out), self);
        g_signal_connect (G_OBJECT (self->button), "clicked",
                          G_CALLBACK (on_clicked), self);
        gtk_container_add (GTK_CONTAINER (self), self->button);

        self->evbox = gtk_event_box_new ();
        gtk_event_box_set_above_child (GTK_EVENT_BOX (self->evbox), TRUE);
        g_signal_connect (G_OBJECT (self->evbox),
                          "realize", G_CALLBACK (on_realize), self);
        gtk_container_add (GTK_CONTAINER (self->button), self->evbox);

        self->image = gtk_image_new ();
        g_object_set (G_OBJECT (self->image), "xpad", 6, "ypad", 6, NULL);

        gtk_container_add (GTK_CONTAINER (self->evbox), self->image);

        priv->selected_color = NULL;
        priv->normal_color   = NULL;
        priv->eid            = NULL;
        priv->fetcher        = NULL;
        priv->ub             = NULL;
        priv->pixbuf         = NULL;

        return;
}

/**
 * ytv_thumbnail_new:
 * @id: The #YtvEntry id
 *
 * Creates a widget which will display the #YtvEntry thumbnail
 */
GtkWidget*
ytv_thumbnail_new (void)
{
        YtvThumbnail* thumb;
        thumb = g_object_new (YTV_TYPE_THUMBNAIL, NULL);
        return GTK_WIDGET (thumb);
}

/**
 * ytv_thumbnail_set_id:
 * @self: a #YtvThumbnail
 * @id: (not-null): The #YtvEntry id
 *
 * Sets the #YtvEntry's id to fetch its thumbnail
 */
void
ytv_thumbnail_set_id (YtvThumbnail* self, const gchar* id)
{
        YtvThumbnailPriv* priv;

        g_return_if_fail (YTV_IS_THUMBNAIL (self));
        g_return_if_fail (id != NULL);

        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        if (priv->eid != NULL)
        {
                g_free (priv->eid);
        }

        priv->eid = g_strdup (id);

        fetch_image (self);

        g_object_notify (G_OBJECT (self), "id");

        return;
}

/**
 * ytv_thumbnail_get_id:
 * @self: a #YtvThumbnail
 *
 * Gets the #YtvEntry's id of the fetched thumbnail
 *
 * returns: (null-ok) (caller-owns): the #YtvEntry's id of the thumbnail
 */
gchar*
ytv_thumbnail_get_id (YtvThumbnail* self)
{
        gchar* id;

        g_return_val_if_fail (YTV_IS_THUMBNAIL (self), NULL);

        g_object_get (G_OBJECT (self), "id", id, NULL);

        return id;
}

/**
 * ytv_thumbnail_set_fetch_stragegy:
 * @self: a #YtvThumbnail
 * @fetcher: a #YtvFeedFetchStrategy implementation
 *
 * Set an implementation of #YtvFeedFetchStrategy for image downloading
 */
void
ytv_thumbnail_set_fetch_strategy (YtvThumbnail* self,
                                  YtvFeedFetchStrategy* fetcher)
{
        YtvThumbnailPriv* priv;

        g_return_if_fail (YTV_IS_THUMBNAIL (self));
        g_return_if_fail (YTV_IS_FEED_FETCH_STRATEGY (fetcher));

        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        if (priv->fetcher != NULL)
        {
                g_object_unref (priv->fetcher);
        }

        priv->fetcher = g_object_ref (fetcher);
        
        return;
}

/**
 * ytv_thumbnail_get_fetch_strategy:
 * @self: #YtvThumbnail
 *
 * Gets the used #YtvFeedFetchStrategy implementation
 *
 * returns (null-ok) (caller-owns): a #YtvFeedFetchStrategy implementation
 */
YtvFeedFetchStrategy*
ytv_thumbnail_get_fetch_strategy (YtvThumbnail* self)
{
        YtvThumbnailPriv* priv;

        g_return_val_if_fail (YTV_IS_THUMBNAIL (self), NULL);

        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        if (priv->fetcher != NULL)
        {
                return g_object_ref (priv->fetcher);
        }

        return NULL;
}

/**
 * ytv_thumbnail_set_uri_builder:
 * @self: a #YtvThumbnail
 * @fetcher: a #YtvUriBuilder implementation
 *
 * Set an implementation of #YtvUriBuilder for thumbnail downloading
 */
void
ytv_thumbnail_set_uri_builder (YtvThumbnail* self, YtvUriBuilder *ub)
{
        YtvThumbnailPriv* priv;

        g_return_if_fail (YTV_IS_THUMBNAIL (self));
        g_return_if_fail (YTV_IS_URI_BUILDER (ub));

        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        if (priv->ub != NULL)
        {
                g_object_unref (priv->ub);
        }

        priv->ub = g_object_ref (ub);

        return;
}

/**
 * ytv_thumbnail_get_uri_builder:
 * @self: #YtvThumbnail
 *
 * Gets the used #YtvUriBuilder implementation
 *
 * returns (null-ok) (caller-owns): a #YtvUriBuilder implementation
 */
YtvUriBuilder*
ytv_thumbnail_get_uri_builder (YtvThumbnail* self)
{
        YtvThumbnailPriv* priv;

        g_return_val_if_fail (YTV_IS_THUMBNAIL (self), NULL);

        priv = YTV_THUMBNAIL_GET_PRIVATE (self);

        if (priv->ub != NULL)
        {
                return g_object_ref (priv->ub);
        }

        return NULL;
}

/**
 * ytv_thumbnail_clean:
 * @self: a #YtvThumbnail
 *
 * Removes the image
 */
void
ytv_thumbnail_clean (YtvThumbnail* self)
{
        g_return_if_fail (YTV_IS_THUMBNAIL (self));

        gtk_image_clear (GTK_IMAGE (self->image));

        return;
}
