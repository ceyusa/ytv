/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-feed.c - A type that defines a feed
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
 * SECTION: ytv-feed
 * @title: YtvFeed
 * @short_description: A type that defines a feed.
 *
 * An abstract type that defines a feed, which is a mean to retrive
 * a set of related #YtvEntry
 */

/**
 * YtvFeed:
 *
 * A feed, or entry collection
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-feed.h>
#include <ytv-list.h>
#include <ytv-feed-fetch-strategy.h>
#include <ytv-feed-parse-strategy.h>

/**
 * ytv_feed_get_fetch_strategy:
 * @self: a #YtvFeed
 *
 * Get the strategy for receiving a feed. The return value of this method
 * must be unreferenced after use.
 *
 * returns: (caller-owns): the strategy for receiving a feed
 */
YtvFeedFetchStrategy*
ytv_feed_get_fetch_strategy (YtvFeed* self)
{
        YtvFeedFetchStrategy* retval;

        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->get_fetch_strategy != NULL);

        retval = YTV_FEED_GET_IFACE (self)->get_fetch_strategy (self);

        g_assert (YTV_IS_FEED_FETCH_STRATEGY (retval));

        return retval;
}

/**
 * ytv_feed_set_fetch_strategy:
 * @self: a #YtvFeed
 * @st: a #YtvFeedFetchStrategy
 *
 * Set the strategy for receiving a feed.
 */
void
ytv_feed_set_fetch_strategy (YtvFeed* self, YtvFeedFetchStrategy* st)
{
        g_assert (YTV_IS_FEED (self));
        g_assert (st != NULL);
        g_assert (YTV_IS_FEED_FETCH_STRATEGY (st));
        g_assert (YTV_FEED_GET_IFACE (self)->set_fetch_strategy != NULL);

        YTV_FEED_GET_IFACE (self)->set_fetch_strategy (self, st);

        YtvFeedFetchStrategy* test = ytv_feed_get_fetch_strategy (self);
        g_assert (test);
        g_assert (YTV_IS_FEED_FETCH_STRATEGY (test));
        g_assert (test == st);
        g_object_unref (G_OBJECT (test));

        return;
}

/**
 * ytv_feed_get_parse_strategy:
 * @self: a #YtvFeed
 *
 * Get the strategy for parsing a feed. The return value of this method
 * must be unreferenced after use.
 *
 * returns: (caller-owns): the strategy for parsing a feed
 */
YtvFeedParseStrategy*
ytv_feed_get_parse_strategy (YtvFeed* self)
{
        YtvFeedParseStrategy* retval;

        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->get_parse_strategy != NULL);

        retval = YTV_FEED_GET_IFACE (self)->get_parse_strategy (self);

        g_assert (YTV_IS_FEED_PARSE_STRATEGY (retval));

        return retval;
}

/**
 * ytv_feed_set_parse_strategy:
 * @self: a #YtvFeed
 * @st: a #YtvFeedParseStrategy
 *
 * Set the strategy for receiving a feed.
 */
void
ytv_feed_set_parse_strategy (YtvFeed* self, YtvFeedParseStrategy* st)
{
        g_assert (YTV_IS_FEED (self));
        g_assert (st != NULL);
        g_assert (YTV_IS_FEED_PARSE_STRATEGY (st));
        g_assert (YTV_FEED_GET_IFACE (self)->set_parse_strategy != NULL);

        YTV_FEED_GET_IFACE (self)->set_parse_strategy (self, st);

        YtvFeedParseStrategy* test = ytv_feed_get_parse_strategy (self);
        g_assert (test);
        g_assert (YTV_IS_FEED_PARSE_STRATEGY (test));
        g_assert (test == st);
        g_object_unref (G_OBJECT (test));

        return;
}

/**
 * ytv_feed_get_entries_async:
 * @self: a #YtvFeed
 * @callback: (null-ok): a #YtvGetEntriesCallback or NULL
 * @user_data: (null-ok): user data that will be passed to the callbacks
 *
 * Get the entries in @self asynchronously.
 *
 * Example:
 * <informalexample><programlisting>
 * static void
 * feed_get_entries_cb (YtvFeed *feed, gboolean cancelled, YtvList* entries,
 *                      GError **err, gpointer user_data)
 * {
 *         YtvFeedView* feed_view = user_data;
 *         if (!err && entries && !cancelled)
 *                 ytv_feed_view_set_entries (feed_view, entries);
 * }
 * YtvFeedView* feed_view = ytv_platform_factory_new_feed_view (platfact);
 * YtvFeed *feed = ...;
 * ytv_feed_get_entries_async (feed, feed_get_entries_cb, feed_view);
 * </programlisting></informalexample>
 */
void
ytv_feed_get_entries_async (YtvFeed* self, YtvGetEntriesCallback callback,
                            gpointer user_data)
{
        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->get_entries_async != NULL);

        YTV_FEED_GET_IFACE (self)->get_entries_async (self, callback, user_data);

        return;
}


static void
ytv_feed_base_init (gpointer g_class)
{
        static gboolean initialized = FALSE;

        if (!initialized)
        {
                initialized = TRUE;
        }

        return;
}

GType
ytv_feed_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GTypeInfo info =
                {
                        sizeof (YtvFeedIface),
                        ytv_feed_base_init,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        0,
                        0,
                        NULL,
                        NULL
                };

                type = g_type_register_static (G_TYPE_INTERFACE, "YtvFeed",
                                               &info, 0);

                g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
        }

        return type;
}
