/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-gtk-entry-view.c - A type that defines a GTK+ GUI entry view
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

#include <ytv-gtk-entry-view.h>

#include <gtk/gtk.h>

#include <ytv-rank.h>
#include <ytv-thumbnail.h>

#include <ytv-entry.h>

enum _YtvGtkEntryViewProp
{
        PROP_0,
        PROP_ORIENTATION
};

typedef struct _YtvGtkEntryViewPriv YtvGtkEntryViewPriv;
struct _YtvGtkEntryViewPriv
{
        YtvOrientation orientation;
        /* YtvText* text; */
        GtkWidget* thumb;
        GtkTextTagTable* tagtable;
};

#define FONTSIZE 10 * PANGO_SCALE

#define YTV_GTK_ENTRY_VIEW_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_GTK_ENTRY_VIEW, YtvGtkEntryViewPriv))

static void
ytv_entry_view_init (YtvEntryViewIface* iface)
{
        iface->set_entry = ytv_gtk_entry_view_set_entry;
        iface->get_entry = ytv_gtk_entry_view_get_entry;

        return;
}

G_DEFINE_TYPE_EXTENDED (YtvGtkEntryView, ytv_gtk_entry_view, GTK_TYPE_TABLE, 0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_ENTRY_VIEW,
                                               ytv_entry_view_init))

/* Create a GtkTextTagTable which will be shared by all the TextViews */
static GtkTextTagTable*
create_tag_table (void)
{
        GtkTextTagTable* table;
        GtkTextTag* tag;

        table = gtk_text_tag_table_new ();

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "p",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "b",
                                          "weight", PANGO_SCALE_LARGE,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "b",
                                          "weight", PANGO_WEIGHT_BOLD,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "big",
                                          "scale", PANGO_SCALE_LARGE,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "i",
                                          "style", PANGO_STYLE_ITALIC,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "gray",
                                          "foreground", "darkgray",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "red",
                                          "foreground", "darkred",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "blue",
                                          "foreground", "blue",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "link",
                                          "underline", PANGO_UNDERLINE_SINGLE,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);
        
        return table;
}

static GtkTextTagTable*
get_tag_table (void)
{
        static GtkTextTagTable* table = NULL;

        if (table == NULL)
        {
                table = create_tag_table ();
        }
        else
        {
                g_object_ref (G_OBJECT (table));
        }

        return table;
}

static void
resize (YtvGtkEntryView* self)
{
        YtvGtkEntryViewPriv* priv;

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);
 
        /** TODO **/
        if (priv->orientation == YTV_ORIENTATION_VERTICAL)
        {
                gtk_table_resize (GTK_TABLE (self), 1, 2);
        }
        else if (priv->orientation == YTV_ORIENTATION_HORIZONTAL)
        {
                gtk_table_resize (GTK_TABLE (self), 2, 1);
        }
        else
        {
                g_return_if_reached ();
        }

        return;
}

static void
update_widget (YtvGtkEntryView* self)
{
        YtvGtkEntryViewPriv* priv;
        gchar* id;
        
        g_return_if_fail (self->entry != NULL);

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        g_object_get (G_OBJECT (self->entry), "id", &id, NULL);

        if (id != NULL)
        {
                g_object_set (G_OBJECT (priv->thumb), "id", id, NULL);
                g_free (id);
        }
                
        return;
}
        
static void
ytv_gtk_entry_view_set_entry_default (YtvEntryView* self, YtvEntry* entry)
{
        YtvGtkEntryView* me;

        g_return_if_fail (YTV_IS_GTK_ENTRY_VIEW (self));
        g_return_if_fail (entry != NULL);

        me = YTV_GTK_ENTRY_VIEW (self);

        if (me->entry != NULL)
        {
                g_object_unref (me->entry);
        }

        me->entry = g_object_ref (entry);

        update_widget (me);

        return;
}

static YtvEntry*
ytv_gtk_entry_view_get_entry_default (YtvEntryView* self)
{
        YtvGtkEntryView* me;

        g_return_val_if_fail (YTV_IS_GTK_ENTRY_VIEW (self), NULL);

        me = YTV_GTK_ENTRY_VIEW (self);

        g_object_ref (me->entry);

        return me->entry;
}

static void
ytv_gtk_entry_view_get_property (GObject* object, guint prop_id,
                                 GValue* value, GParamSpec* spec)
{
        YtvGtkEntryViewPriv* priv;

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ORIENTATION:
                g_value_set_enum (value, priv->orientation);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_gtk_entry_view_set_property (GObject* object, guint prop_id,
                                 const GValue* value, GParamSpec* spec)
{
        switch (prop_id)
        {
        case PROP_ORIENTATION:
                ytv_gtk_entry_view_set_orientation (YTV_GTK_ENTRY_VIEW (object),
                                                    g_value_get_enum (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_gtk_entry_view_dispose (GObject* object)
{
        YtvGtkEntryView* me;
        YtvGtkEntryViewPriv* priv;

        (*G_OBJECT_CLASS (ytv_gtk_entry_view_parent_class)->dispose) (object);

        me = YTV_GTK_ENTRY_VIEW (object);
        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (object);

        if (priv->tagtable != NULL)
        {
                g_object_unref (priv->tagtable);
                priv->tagtable = NULL;
        }

        if (priv->thumb != NULL)
        {
                g_object_unref (priv->thumb);
                priv->thumb = NULL;
        }

        if (me->entry != NULL)
        {
                g_object_unref (me->entry);
                me->entry = NULL;
        }

        return;
}

static void
ytv_gtk_entry_view_class_init (YtvGtkEntryViewClass* klass)
{
        GObjectClass* object_class;

        object_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (object_class, sizeof (YtvGtkEntryViewPriv));

        object_class->get_property = ytv_gtk_entry_view_get_property;
        object_class->set_property = ytv_gtk_entry_view_set_property;
        object_class->dispose      = ytv_gtk_entry_view_dispose;

        klass->set_entry = ytv_gtk_entry_view_set_entry_default;
        klass->get_entry = ytv_gtk_entry_view_get_entry_default;

        g_object_class_install_property
                (object_class, PROP_ORIENTATION,
                 g_param_spec_enum
                 ("orientation", "Orientation", "Widget orientation",
                  YTV_TYPE_ORIENTATION, YTV_ORIENTATION_HORIZONTAL,
                  G_PARAM_READWRITE));

        return;
}

static void
ytv_gtk_entry_view_init (YtvGtkEntryView* self)
{
        YtvGtkEntryViewPriv* priv;

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        priv->orientation = YTV_ORIENTATION_VERTICAL;
        priv->tagtable    = get_tag_table ();
        priv->thumb       = ytv_thumbnail_new ();

        self->entry = NULL;

        resize (self);
        gtk_table_attach_defaults (GTK_TABLE (self), priv->thumb, 0, 1, 0, 1);

        return;
}

/**
 * ytv_entry_view_set_entry:
 * @self: a #YtvEntryView
 * @entry: (not-null): a #YtvEntry
 *
 * Set the entry which view @self must display.
 */
void
ytv_gtk_entry_view_set_entry (YtvEntryView* self, YtvEntry* entry)
{
        g_assert (self != NULL);
        g_assert (YTV_IS_ENTRY_VIEW (self));
        g_assert (entry != NULL);
        g_assert (YTV_IS_ENTRY (entry));

        YTV_GTK_ENTRY_VIEW_GET_CLASS (self)->set_entry (self, entry);

        return;
}

/**
 * ytv_gtk_entry_view_get_entry:
 * @self: a #YtvEntryView
 *
 * Get the entry which view @self must display.
 *
 * returns: (not-null) (caller-owns): The displayed #YtvEntry
 */
YtvEntry*
ytv_gtk_entry_view_get_entry (YtvEntryView* self)
{
        g_assert (self != NULL);
        g_assert (YTV_IS_ENTRY_VIEW (self));

        return YTV_GTK_ENTRY_VIEW_GET_CLASS (self)->get_entry (self);
}

/**
 * ytv_gtk_entry_view_set_orientation:
 * @self: (not-null): a #YtvGtkEntryView
 * @orientation: the widget orientation
 *
 * Set the visual orientation of the data display: #YTV_ORIENTATION_HORIZONTAL
 * for landscape and #YTV_ORIENTATION_VERTICAL for portrait.
 */
void
ytv_gtk_entry_view_set_orientation (YtvGtkEntryView* self,
                                    YtvOrientation orientation)
{
        YtvGtkEntryViewPriv* priv;

        g_return_if_fail (self != NULL);

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        if (orientation == priv->orientation)
        {
                return;
        }

        priv->orientation = orientation;

        resize (self);
        
        g_object_notify (G_OBJECT (self), "orientation");

        return;
}

/**
 * ytv_gtk_entry_view_get_orientation:
 * @self: (not-null): a #YtvGtkEntryView
 *
 * Get the visual orientation of the data display: #YTV_ORIENTATION_HORIZONTAL
 * for landscape and #YTV_ORIENTATION_VERTICAL for portrait.
 *
 * returns: #YTV_ORIENTATION_VERTICAL or #YTV_ORIENTATION_HORIZONTAL
 */
YtvOrientation
ytv_gtk_entry_view_get_orientation (YtvGtkEntryView* self)
{
        YtvOrientation orientation;

        g_return_val_if_fail (self != NULL, YTV_ORIENTATION_HORIZONTAL);

        g_object_get (G_OBJECT (self), "orientation", &orientation, NULL);

        return orientation;
}

/**
 * ytv_gtk_entry_view_set_fetch_strategy:
 * @self: a #YtvGtkEntryView
 * @st: a #YtvFeedFetchStrategy instance
 *
 * Helper. Sets the HTTP Fetch strategy for thumbnail fetching.
 */
void
ytv_gtk_entry_view_set_fetch_strategy (YtvGtkEntryView* self,
                                       YtvFeedFetchStrategy* st)
{
        YtvGtkEntryViewPriv* priv;

        g_return_if_fail (YTV_IS_GTK_ENTRY_VIEW (self));
        g_return_if_fail (YTV_IS_FEED_FETCH_STRATEGY (st));

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        if (priv->thumb != NULL)
        {
                ytv_thumbnail_set_fetch_strategy (YTV_THUMBNAIL (priv->thumb),
                                                  st);
        }

        return;
}

/**
 * ytv_gtk_entry_view_set_uri_builder:
 * @self: a #YtvGtkEntryView
 * @ub: a #YtvUriBuilder instance
 *
 * Helper. Sets the URI builder for fetching the thumbnail.
 */
void
ytv_gtk_entry_view_set_uri_builder (YtvGtkEntryView* self,
                                    YtvUriBuilder* ub)
{
        YtvGtkEntryViewPriv* priv;

        g_return_if_fail (YTV_IS_GTK_ENTRY_VIEW (self));
        g_return_if_fail (YTV_IS_URI_BUILDER (ub));

        priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        if (priv->thumb != NULL)
        {
                ytv_thumbnail_set_uri_builder (YTV_THUMBNAIL (priv->thumb), ub);
        }

        return;        
}

/**
 * ytv_orientation_get_type:
 *
 * GType system helper function
 *
 * returns: a #GType
 */
GType
ytv_orientation_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GEnumValue values[] = {
                        { YTV_ORIENTATION_HORIZONTAL,
                          "YTV_ORIENTATION_HORIZONTAL", "horizontal" },
                        { YTV_ORIENTATION_VERTICAL,
                          "YTV_ORIENTATION_VERTICAL", "vertical" },
                        { 0, NULL, NULL }
                };

                type = g_enum_register_static ("YtvOrientationType", values);
        }

        return type;
}
