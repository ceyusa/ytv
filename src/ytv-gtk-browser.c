/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-gtk-browser.c - Implementation of the browser widget with GTK+
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

#include <ytv-gtk-browser.h>
#include <ytv-error.h>
#include <ytv-list.h>
#include <ytv-iterator.h>
#include <ytv-entry.h>

enum _YtvGtkBrowserProp
{
        PROP_0,
        PROP_ORIENTATION,
        PROP_NUMENTRIES
};

typedef struct _YtvGtkBrowserPriv YtvGtkBrowserPriv;
struct _YtvGtkBrowserPriv
{
        YtvOrientation orientation;
        gint num_entries;
        gint page_num;
        gint start_idx;
        gboolean last_page;
        gint wid_pos; /* table current col or row */
};

#define YTV_GTK_BROWSER_GET_PRIVATE(obj)  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_GTK_BROWSER, YtvGtkBrowserPriv))

static void
ytv_browser_init (YtvBrowserIface* iface)
{
        iface->fetch_entries = ytv_gtk_browser_fetch_entries;
        iface->next_page     = ytv_gtk_browser_next_page;
        iface->prev_page     = ytv_gtk_browser_prev_page;
        iface->set_feed      = ytv_gtk_browser_set_feed;
        iface->get_feed      = ytv_gtk_browser_get_feed;
        iface->get_focused_entry_view = ytv_gtk_browser_get_focused_entry_view;
        iface->clean         = ytv_gtk_browser_clean;

        return;
}

G_DEFINE_TYPE_EXTENDED (YtvGtkBrowser, ytv_gtk_browser, GTK_TYPE_TABLE, 0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_BROWSER,
                                               ytv_browser_init))

static void
set_start_index (YtvGtkBrowser* self, gint idx)
{
        YtvUriBuilder* ub;
        YtvGtkBrowserPriv* priv;

        g_return_if_fail (idx >= 0);

        priv = YTV_GTK_BROWSER_GET_PRIVATE (self);

        priv->start_idx = idx;
        ub = ytv_feed_get_uri_builder (self->feed);
        g_object_set (G_OBJECT (ub), "start-index", priv->start_idx, NULL);
        g_object_unref (ub);
        
        return;
}

static gboolean
fetch_feed (YtvBrowser* self)
{
        ytv_gtk_browser_fetch_entries (self);
        return FALSE;
}

static GtkWidget*
create_entry_view (YtvFeed* feed, YtvOrientation orientation)
{
        YtvFeedFetchStrategy* fetchst;
        YtvUriBuilder* ub;
        YtvEntryView* view;
        
        if (orientation == YTV_ORIENTATION_HORIZONTAL)
        {
                view = ytv_gtk_entry_view_new (YTV_ORIENTATION_VERTICAL);
        } 
        else
        {
                view = ytv_gtk_entry_view_new (YTV_ORIENTATION_HORIZONTAL);
        }
                        
        fetchst = ytv_feed_get_fetch_strategy (feed);
        ytv_entry_view_set_fetch_strategy (view, fetchst);
        g_object_unref (fetchst);

        ub = ytv_feed_get_uri_builder (feed);
        ytv_entry_view_set_uri_builder (view, ub);
        g_object_unref (ub);
        
        gtk_widget_show_all (GTK_WIDGET (view));
        
        return GTK_WIDGET (view);
}

static void
link_clicked_cb (GtkWidget* widget,
                 const gchar* class, const gchar* param, gpointer user_data)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (user_data);

        set_start_index (self, 0);

        if (g_strrstr (class, "author") != NULL)
        {
                ytv_feed_user (self->feed, param);
        }
        else if (g_strrstr (class, "category") != NULL)
        {
                ytv_feed_keywords (self->feed, param, ""); /* @fixme */
        }
        else
        {
                return;
        }

        g_idle_add ((GSourceFunc) fetch_feed, (gpointer) self);
        
        return;
}

static void
show_entry_view (YtvGtkBrowser* self, YtvEntry* entry)
{
        GtkWidget* entryview;
        YtvGtkBrowserPriv* priv;
        
        priv = YTV_GTK_BROWSER_GET_PRIVATE (self);

        entryview = create_entry_view (self->feed, priv->orientation);

        g_signal_connect (entryview, "link-clicked",
                          G_CALLBACK (link_clicked_cb), self);

        ytv_entry_view_set_entry (YTV_ENTRY_VIEW (entryview), entry);

        if (priv->orientation == YTV_ORIENTATION_HORIZONTAL)
        {
                gtk_table_attach_defaults (GTK_TABLE (self), entryview,
                                           priv->wid_pos, priv->wid_pos + 1,
                                           0, 1);
        }
        else
        {
                gtk_table_attach_defaults (GTK_TABLE (self), entryview,
                                           0, 1,
                                           priv->wid_pos, priv->wid_pos + 1);
        }
        
        return;
}

static void
feed_entry_cb (YtvFeed* feed, gboolean cancelled, YtvList* list,
               GError **err, gpointer user_data)
{
        YtvGtkBrowser* self;
        YtvIterator* iter;
        YtvGtkBrowserPriv* priv;

        self = YTV_GTK_BROWSER (user_data);
        priv = YTV_GTK_BROWSER_GET_PRIVATE (self);
        
        if (*err != NULL)
        {
                g_debug ("%s", ytv_error_get_message (*err));

                if (ytv_error_get_code (*err) == YTV_PARSE_ERROR_BAD_FORMAT)
                {
                        g_signal_emit_by_name (self, "last-page");
                        priv->last_page = TRUE;
                        g_error_free (*err);
                }
                else
                {
                        g_signal_emit_by_name (self, "error-raised", *err);
                }
                
                return;
        }

        g_return_if_fail (list != NULL);

        priv->wid_pos = 0;
        iter = ytv_list_create_iterator (list);
        while (!ytv_iterator_is_done (iter))
        {
                YtvEntry* entry;
                
                entry = YTV_ENTRY (ytv_iterator_get_current (iter));

                show_entry_view (self, entry);

                priv->wid_pos++;
                
                g_object_unref (entry);
                ytv_iterator_next (iter);
        }

        g_object_unref (iter);
        g_object_unref (list);

        if (priv->wid_pos < priv->num_entries)
        {
                g_signal_emit_by_name (self, "last-page");
                priv->last_page = TRUE;
        }
        else
        {
                priv->last_page = FALSE;

                if (priv->start_idx == 0)
                {
                        g_signal_emit_by_name (self, "first-page");
                }
        }

        return;
}

static void
ytv_gtk_browser_fetch_entries_default (YtvBrowser* me)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (me);
        
        ytv_gtk_browser_clean (me);

        ytv_feed_get_entries_async (self->feed, feed_entry_cb, self);

        return;
}

static gboolean
ytv_gtk_browser_next_page_default (YtvBrowser* me)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (me);
        YtvGtkBrowserPriv* priv = YTV_GTK_BROWSER_GET_PRIVATE (self);
        
        if (priv->last_page == FALSE)
        {
                priv->start_idx += priv->num_entries;
                set_start_index (self, priv->start_idx);
                g_idle_add ((GSourceFunc) fetch_feed, (gpointer) self);
                priv->page_num++;
        }

        return priv->last_page;
}

static gboolean
ytv_gtk_browser_prev_page_default (YtvBrowser* me)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (me);
        YtvGtkBrowserPriv* priv = YTV_GTK_BROWSER_GET_PRIVATE (self);

        if (priv->start_idx - priv->num_entries > 0 && priv->page_num > 0)
        {
                priv->start_idx -= priv->num_entries;
                set_start_index (self, priv->start_idx);
                g_idle_add ((GSourceFunc) fetch_feed, (gpointer) self);

                priv->page_num--;
                /* what if is a single page with less than num_entries? */
                /* priv->last_page = FALSE; */
        }
        else
        {
                g_signal_emit_by_name (self, "first-page");
        }

        return (priv->start_idx <= 0 || priv->page_num <= 0);
}

static void
ytv_gtk_browser_set_feed_default (YtvBrowser* me, YtvFeed* feed)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (me);
        
        self->feed = g_object_ref (feed);

        return;
}

static YtvFeed*
ytv_gtk_browser_get_feed_default (YtvBrowser* me)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (me);
        return g_object_ref (self->feed);
}

static YtvEntryView*
ytv_gtk_browser_get_focused_entry_view_default (YtvBrowser* self)
{
        /* @todo */
        return NULL;
}

static void
ytv_gtk_browser_clean_default (YtvBrowser* me)
{
        YtvGtkBrowser* self = YTV_GTK_BROWSER (me);
        YtvGtkBrowserPriv* priv = YTV_GTK_BROWSER_GET_PRIVATE (self);

        gtk_container_foreach (GTK_CONTAINER (self),
                               (GtkCallback) gtk_widget_destroy, NULL);

        priv->wid_pos = 0;

        return;
}

static void
ytv_gtk_browser_get_property (GObject* object, guint prop_id,
                              GValue* value, GParamSpec* spec)
{
        YtvGtkBrowserPriv* priv;

        priv = YTV_GTK_BROWSER_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ORIENTATION:
                g_value_set_enum (value, priv->orientation);
                break;
        case PROP_NUMENTRIES:
                g_value_set_int (value, priv->num_entries);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_gtk_browser_set_property (GObject* object, guint prop_id,
                              const GValue* value, GParamSpec* spec)
{
        YtvGtkBrowser* self;
        YtvGtkBrowserPriv* priv;

        self = YTV_GTK_BROWSER (object);
        priv = YTV_GTK_BROWSER_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ORIENTATION:
                priv->orientation = g_value_get_enum (value);
                g_object_notify (object, "orientation");
                break;
        case PROP_NUMENTRIES:
                priv->num_entries = g_value_get_int (value);
                g_object_notify (object, "num-entries");
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_gtk_browser_dispose (GObject* object)
{
        YtvGtkBrowser* me;

        (*G_OBJECT_CLASS (ytv_gtk_browser_parent_class)->dispose) (object);

        me = YTV_GTK_BROWSER (object);

        if (me->feed != NULL)
        {
                g_object_unref (me->feed);
                me->feed = NULL;
        }

        return;
}

static void
ytv_gtk_browser_class_init (YtvGtkBrowserClass* klass)
{
        GObjectClass* object_class;

        object_class = G_OBJECT_CLASS (klass);
        
        g_type_class_add_private (object_class, sizeof (YtvGtkBrowserPriv));

        object_class->get_property = ytv_gtk_browser_get_property;
        object_class->set_property = ytv_gtk_browser_set_property;
        object_class->dispose      = ytv_gtk_browser_dispose;

        klass->fetch_entries = ytv_gtk_browser_fetch_entries_default;
        klass->next_page     = ytv_gtk_browser_next_page_default;
        klass->prev_page     = ytv_gtk_browser_prev_page_default;
        klass->set_feed      = ytv_gtk_browser_set_feed_default;
        klass->get_feed      = ytv_gtk_browser_get_feed_default;
        klass->get_focused_entry_view = ytv_gtk_browser_get_focused_entry_view_default;
        klass->clean         = ytv_gtk_browser_clean_default;
        
        g_object_class_install_property
                (object_class, PROP_ORIENTATION,
                 g_param_spec_enum
                 ("orientation", "Orientation", "Widget orientation",
                  YTV_TYPE_ORIENTATION, YTV_ORIENTATION_UNDEF,
                  G_PARAM_READWRITE));

        g_object_class_install_property
                (object_class, PROP_NUMENTRIES,
                 g_param_spec_int ("num-entries",
                                   "num_entries_per_page",
                                   "Number of entries per page",
                                   0, 25, 5, G_PARAM_READWRITE));

        return;
}

static void
ytv_gtk_browser_init (YtvGtkBrowser* self)
{
        YtvGtkBrowserPriv* priv;
        
        priv = YTV_GTK_BROWSER_GET_PRIVATE (self);
        priv->orientation = YTV_ORIENTATION_UNDEF;
        priv->num_entries = 5;
        priv->page_num    = 0;
        priv->start_idx   = 0;
        priv->last_page   = FALSE;
        priv->wid_pos     = 0;

        self->feed = NULL;
        
        g_object_set (G_OBJECT (self),
                      "column-spacing", 0,
                      "row-spacing", 0,
                      "homogeneous", FALSE,
                      "n-columns", 1,
                      "n-rows", 1, NULL);

        return;
}

YtvBrowser*
ytv_gtk_browser_new (YtvOrientation orientation)
{
        YtvBrowser* retval;

        retval = g_object_new (YTV_TYPE_GTK_BROWSER,
                               "orientation", orientation, NULL);

        return retval;
}

void
ytv_gtk_browser_fetch_entries (YtvBrowser* self)
{
        g_assert (YTV_IS_GTK_BROWSER (self));

        YTV_GTK_BROWSER_GET_CLASS (self)->fetch_entries (self);

        return;
}

gboolean
ytv_gtk_browser_next_page (YtvBrowser* self)
{
        g_assert (YTV_IS_GTK_BROWSER (self));

        return YTV_GTK_BROWSER_GET_CLASS (self)->next_page (self);
}

gboolean
ytv_gtk_browser_prev_page (YtvBrowser* self)
{
        g_assert (YTV_IS_GTK_BROWSER (self));

        return YTV_GTK_BROWSER_GET_CLASS (self)->prev_page (self);
}

void
ytv_gtk_browser_set_feed (YtvBrowser* self, YtvFeed* feed)
{
        g_assert (YTV_IS_GTK_BROWSER (self));
        g_assert (feed != NULL);

        YTV_GTK_BROWSER_GET_CLASS (self)->set_feed (self, feed);

        return;
}

YtvFeed*
ytv_gtk_browser_get_feed (YtvBrowser* self)
{
        YtvFeed* retval;

        g_assert (YTV_IS_GTK_BROWSER (self));

        retval = YTV_GTK_BROWSER_GET_CLASS (self)->get_feed (self);

        return retval;
}

YtvEntryView*
ytv_gtk_browser_get_focused_entry_view (YtvBrowser* self)
{
        YtvEntryView* retval;

        g_assert (YTV_IS_GTK_BROWSER (self));

        retval = YTV_GTK_BROWSER_GET_CLASS (self)->get_focused_entry_view (self);

        return retval;
}

void
ytv_gtk_browser_clean (YtvBrowser* self)
{
        g_assert (YTV_IS_GTK_BROWSER (self));

        YTV_GTK_BROWSER_GET_CLASS (self)->clean (self);

        return;
}
