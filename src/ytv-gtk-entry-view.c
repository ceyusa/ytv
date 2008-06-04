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
#include <ytv-entry.h>

enum _YtvGtkEntryViewProp
{
        PROP_0,
        PROP_ORIENTATION
};

typedef struct _YtvGtkEntryViewPriv YtvGtkEntryViewPriv;
struct _YtvGtkEntryViewPriv
{
        GtkWidget* rank;
        YtvOrientation orientation;
        /* YtvText* text; */
};

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

static void
ytv_gtk_entry_view_set_entry_default (YtvEntryView* self, YtvEntry* entry)
{
        g_return_if_fail (YTV_IS_GTK_ENTRY_VIEW (self));
        g_return_if_fail (entry != NULL);

        YtvGtkEntryView* me = YTV_GTK_ENTRY_VIEW (self);

        if (me->entry != NULL)
        {
                g_object_unref (me->entry);
        }
        
        me->entry = g_object_ref (entry);

        /* TODO: update_widget (self); */

        return;
}

static YtvEntry*
ytv_gtk_entry_view_get_entry_default (YtvEntryView* self)
{
        g_return_if_fail (YTV_IS_GTK_ENTRY_VIEW (self));

        YtvGtkEntryView* me = YTV_GTK_ENTRY_VIEW (self);

        return g_object_ref (me->entry);
}

static void
ytv_gtk_entry_view_get_property (GObject* object, guint prop_id,
                                 GValue* value, GParamSpec* spec)
{
        YtvGtkEntryViewPriv* priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (object);

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
        (*G_OBJECT_CLASS (ytv_gtk_entry_view_parent_class)->dispose) (object);

        YtvGtkEntryView* me = YTV_GTK_ENTRY_VIEW (object);
        YtvGtkEntryViewPriv* priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (object);

        if (priv->rank != NULL)
        {
                g_object_unref (priv->rank);
        }

        if (me->entry != NULL)
        {
                g_object_unref (me->entry);
        }

        return;
}

static void
ytv_gtk_entry_view_class_init (YtvGtkEntryViewClass* klass)
{
        GObjectClass* g_klass = G_OBJECT_CLASS (klass);

        g_type_class_add_private (g_klass, sizeof (YtvGtkEntryViewPriv));
        
        g_klass->get_property = ytv_gtk_entry_view_get_property;
        g_klass->set_property = ytv_gtk_entry_view_set_property;
        g_klass->dispose      = ytv_gtk_entry_view_dispose;

        klass->set_entry = ytv_gtk_entry_view_set_entry_default;
        klass->get_entry = ytv_gtk_entry_view_get_entry_default;

        g_object_class_install_property
                (g_klass, PROP_ORIENTATION,
                 g_param_spec_enum
                 ("orientation", "Orientation", "Widget orientation",
                  YTV_TYPE_ORIENTATION, YTV_ORIENTATION_HORIZONTAL,
                  G_PARAM_READWRITE));

        return;
}

static void
ytv_gtk_entry_view_init (YtvGtkEntryView* self)
{
        YtvGtkEntryViewPriv* priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        priv->rank = ytv_rank_new (0);
        priv->orientation = YTV_ORIENTATION_VERTICAL;

        self->entry = NULL;

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
        g_return_if_fail (self != NULL);
        
        YtvGtkEntryViewPriv* priv = YTV_GTK_ENTRY_VIEW_GET_PRIVATE (self);

        if (orientation == priv->orientation)
        {
                return;
        }

        /* TODO */
        if (orientation == YTV_ORIENTATION_VERTICAL)
        {
                gtk_table_resize (GTK_TABLE (self), 1, 2);
        }
        else if (orientation == YTV_ORIENTATION_HORIZONTAL)
        {
                gtk_table_resize (GTK_TABLE (self), 2, 1);
        }
        else
        {
                g_return_if_reached ();
        }

        priv->orientation = orientation;
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
        g_return_if_fail (self != NULL);

        YtvOrientation orientation;
        g_object_get (G_OBJECT (self), "orientation", &orientation, NULL);

        return orientation;
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
