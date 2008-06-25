/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-entry-view.c - A type that defines a GUI entry view
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
 * SECTION: ytv-entry-view
 * @title: YtvEntryView
 * @short_description: A type that defines an user interface view for a 
 * #YtvEntry
 *
 * An abstract type that defines an user interface view for a #YtvEntry. The
 * implementators are in charge to choose the graphical toolkit
 */

/**
 * YtvEntryView:
 *
 * A ui view for #YtvEntry
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-entry-view.h>
#include <ytv-entry.h>
#include <ytv-feed-fetch-strategy.h>
#include <ytv-uri-builder.h>

/**
 * ytv_entry_view_set_entry:
 * @self: a #YtvEntryView
 * @entry: (not-null): a #YtvEntry
 *
 * Set the entry which view @self must display.
 */
void
ytv_entry_view_set_entry (YtvEntryView* self, YtvEntry* entry)
{
        YtvEntry* validate;

        g_assert (YTV_IS_ENTRY_VIEW (self));
        g_assert (YTV_IS_ENTRY (entry));
        
        if (YTV_ENTRY_VIEW_GET_IFACE (self)->set_entry == NULL)
        {
                g_critical ("You must implement ytv_entry_view_set_view\n");
        }

        YTV_ENTRY_VIEW_GET_IFACE (self)->set_entry (self, entry);

        validate = ytv_entry_view_get_entry (self);
        g_assert (validate == entry);
        g_object_unref (validate);

        return;
}

/**
 * ytv_entry_view_get_entry:
 * @self: a #YtvEntryView
 *
 * Get the entry which view @self must display.
 *
 * returns: (not-null) (caller-owns): The displayed #YtvEntry
 */
YtvEntry* 
ytv_entry_view_get_entry (YtvEntryView* self)
{
        YtvEntry* retval;

        g_assert (YTV_IS_ENTRY_VIEW (self));

        if (YTV_ENTRY_VIEW_GET_IFACE (self)->get_entry == NULL)
        {
                g_critical ("You must implement ytv_entry_view_get_view\n");
        }

        retval = YTV_ENTRY_VIEW_GET_IFACE (self)->get_entry (self);

        g_assert (retval != NULL);

        return retval;
}

/**
 * ytv_entry_view_set_fetch_strategy:
 * @self: a #YtvEntryView
 * @st: a #YtvFeedFetchStrategy
 *
 * Sets the fetch strategy. It is used commonly by the thumbnailer.
 */
void
ytv_entry_view_set_fetch_strategy (YtvEntryView* self, YtvFeedFetchStrategy* st)
{ 
        YtvFeedFetchStrategy* validate;

        g_assert (YTV_IS_ENTRY_VIEW (self));
        g_assert (YTV_IS_FEED_FETCH_STRATEGY (st));
        
        if (YTV_ENTRY_VIEW_GET_IFACE (self)->set_fetch_strategy == NULL)
        {
                g_critical ("You must implement ytv_entry_view_set_fetch_strategy\n");
        }

        YTV_ENTRY_VIEW_GET_IFACE (self)->set_fetch_strategy (self, st);

        validate = ytv_entry_view_get_fetch_strategy (self);
        g_assert (validate == st);
        g_object_unref (validate);

        return;
}

/**
 * ytv_entry_view_get_fetch_strategy:
 * @self: a #YtvEntryView
 *
 * Gets the fetch strategy used by @self.
 *
 * returns: (not-null) (caller-owns): The used #YtvFeedFetchStrategy
 */
YtvFeedFetchStrategy* 
ytv_entry_view_get_fetch_strategy (YtvEntryView* self)
{
        YtvFeedFetchStrategy* retval;

        g_assert (YTV_IS_ENTRY_VIEW (self));

        if (YTV_ENTRY_VIEW_GET_IFACE (self)->get_fetch_strategy == NULL)
        {
                g_critical ("You must implement ytv_entry_view_get_fetch_strategy\n");
        }

        retval = YTV_ENTRY_VIEW_GET_IFACE (self)->get_fetch_strategy (self);

        g_assert (retval != NULL);

        return retval;
}

/**
 * ytv_entry_view_set_uri_builder:
 * @self: a #YtvEntryView
 * @ub: a #YtvUriBuilder
 *
 * Sets the URI builder. It is used commonly by the thumbnailer.
 */
void
ytv_entry_view_set_uri_builder (YtvEntryView* self, YtvUriBuilder* ub)
{ 
        YtvUriBuilder* validate;

        g_assert (YTV_IS_ENTRY_VIEW (self));
        g_assert (YTV_IS_URI_BUILDER (ub));
        
        if (YTV_ENTRY_VIEW_GET_IFACE (self)->set_uri_builder == NULL)
        {
                g_critical ("You must implement ytv_entry_view_set_uri_builder\n");
        }

        YTV_ENTRY_VIEW_GET_IFACE (self)->set_uri_builder (self, ub);

        validate = ytv_entry_view_get_uri_builder (self);
        g_assert (validate == ub);
        g_object_unref (validate);

        return;
}

/**
 * ytv_entry_view_get_uri_builder:
 * @self: a #YtvEntryView
 *
 * Gets the URI builder used by @self.
 *
 * returns: (not-null) (caller-owns): The used #YtvUriBuilder
 */
YtvUriBuilder* 
ytv_entry_view_get_uri_builder (YtvEntryView* self)
{
        YtvUriBuilder* retval;

        g_assert (YTV_IS_ENTRY_VIEW (self));

        if (YTV_ENTRY_VIEW_GET_IFACE (self)->get_uri_builder == NULL)
        {
                g_critical ("You must implement ytv_entry_view_get_uri_builder\n");
        }

        retval = YTV_ENTRY_VIEW_GET_IFACE (self)->get_uri_builder (self);

        g_assert (retval != NULL);

        return retval;
}

/**
 * ytv_entry_view_clean:
 * @self: a #YtvEntryView
 *
 * Removes the entry data from the view.
 */
void
ytv_entry_view_clean (YtvEntryView* self)
{
        g_assert (YTV_IS_ENTRY_VIEW (self));

        if (YTV_ENTRY_VIEW_GET_IFACE (self)->clean == NULL)
        {
                g_critical ("You must implement ytv_entry_view_clean\n");
        }

        YTV_ENTRY_VIEW_GET_IFACE (self)->clean (self);

        return;
}

static void
ytv_entry_view_base_init (gpointer g_class)
{
        static gboolean initializated = FALSE;

        if (!initializated)
        {
                g_signal_new ("show-details",
                              YTV_TYPE_ENTRY_VIEW,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvEntryViewIface,
                                               show_details),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);
                
                g_signal_new ("play-stream",
                              YTV_TYPE_ENTRY_VIEW,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvEntryViewIface,
                                               play_stream),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);

                g_signal_new ("link-clicked",
                              YTV_TYPE_ENTRY_VIEW,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvEntryViewIface,
                                               link_clicked),
                              NULL, NULL,
                              ytv_cclosure_marshal_VOID__STRING_STRING,
                              G_TYPE_NONE, 2,
                              G_TYPE_STRING, G_TYPE_STRING);
                
                initializated = TRUE;
        }

        return;
}

static gpointer
ytv_entry_view_register_type (gpointer notused)
{
        GType type = 0;

        static const GTypeInfo info =
        {
                sizeof (YtvEntryViewIface),
                ytv_entry_view_base_init,
                NULL,
                NULL,
                NULL,
                NULL,
                0,
                0,
                NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "YtvEntryView", &info, 0);

        return GUINT_TO_POINTER (type);
}

GType
ytv_entry_view_get_type (void)
{
        static GOnce once = G_ONCE_INIT;
        g_once (&once, ytv_entry_view_register_type, NULL);
        return GPOINTER_TO_UINT (once.retval);
}
