/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-browser.h - An interface for an entry browser widget
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
 * SECTION: ytv-browser
 * @title: YtvBrowser
 * @short_descriptions: A type that defines an user interface to browse
 * a set of #YtvEntreView
 *
 * An abstract type that defines the user interface API for a set of
 * #YtvEntryView. The implementators are in charge to choose the graphical
 * toolkit.
 */

/**
 * YtvBrowser:
 *
 * A UI view for a set of #YtvEntryView
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-browser.h>

#include <ytv-marshal.h>

/**
 * ytv_browser_fetch_entries:
 * @self: a #YtvBrowser
 *
 * Request a new set of #YtvEntry to the #YtvFeed
 */
void
ytv_browser_fetch_entries (YtvBrowser* self)
{
        g_assert (YTV_IS_BROWSER (self));

        if (YTV_BROWSER_GET_IFACE (self)->fetch_entries == NULL)
        {
                g_critical ("You must implement ytv_browser_fetch_entries\n");
        }

        YTV_BROWSER_GET_IFACE (self)->fetch_entries (self);

        return;
}

gboolean
ytv_browser_next_page (YtvBrowser* self)
{
}

gboolean
ytv_browser_prev_page (YtvBrowser* self)
{
}

void
ytv_browser_set_feed (YtvBrowser* self, YtvFeed* feed)
{
}

YtvFeed*
ytv_browser_get_feed (YtvBrowser* self)
{
}

YtvEntreView*
ytv_browser_get_focused_entry_view (YtvBrowser* self)
{
}

void
ytv_browser_clean (YtvBrowser* self)
{
}

static void
ytv_browser_base_init (gpointer g_class)
{
        static gboolean initializated = FALSE;

        if (!initializated)
        {
                g_signal_new ("error-raised",
                              YTV_TYPE_BROWSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvBrowserIface, error_raised),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__POINTER,
                              G_TYPE_NONE, 1, G_TYPE_POINTER);

                g_signal_new ("last-page",
                              YTV_TYPE_BROWSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvBrowserIface, last_page),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);

                g_signal_new ("prev-page",
                              YTV_TYPE_BROWSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvBrowserIface, prev_page),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE, 0);

                g_signal_new ("entry-details",
                              YTV_TYPE_BROWSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvBrowserIface, entry_details),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__OBJECT,
                              G_TYPE_NONE, 1, G_TYPE_OBJECT);

                g_signal_new ("play-stream",
                              YTV_TYPE_BROWSER,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvBrowserIface, play_stream),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__OBJECT,
                              G_TYPE_NONE, 1, G_TYPE_OBJECT);
                
                initializated = TRUE;
        }

        return;
}

static gpointer
ytv_browser_register_type (gpointer notused)
{
        GType type = 0;

        static const GTypeInfo info =
        {
                sizeof (YtvBrowserIface),
                ytv_browser_base_init,
                NULL,
                NULL,
                NULL,
                NULL,
                0,
                0,
                NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "YtvBrowser", &info, 0);

        return GUINT_TO_POINTER (type);
}

GType
ytv_browser_get_type (void)
{
        static GOnce once = G_ONCE_INIT;
        g_once (&once, ytv_browser_register_type, NULL);
        return GPOINTER_TO_UINT (once.retval);
}
