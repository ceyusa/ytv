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
 * @short_description: A type that defines a feed
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
#include <ytv-uri-builder.h>

/**
 * ytv_feed_get_fetch_strategy:
 * @self: a #YtvFeed
 *
 * Get the strategy for receiving a feed. The return value of this method
 * must be unreferenced after use.
 *
 * returns: (null-ok) (caller-owns): the strategy for receiving a feed
 */
YtvFeedFetchStrategy*
ytv_feed_get_fetch_strategy (YtvFeed* self)
{
        YtvFeedFetchStrategy* retval;

        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->get_fetch_strategy != NULL);

        retval = YTV_FEED_GET_IFACE (self)->get_fetch_strategy (self);
        
        if (retval != NULL)
        {
                g_assert (YTV_IS_FEED_FETCH_STRATEGY (retval));
        }

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
 * returns: (null-ok) (caller-owns): the strategy for parsing a feed
 */
YtvFeedParseStrategy*
ytv_feed_get_parse_strategy (YtvFeed* self)
{
        YtvFeedParseStrategy* retval;

        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->get_parse_strategy != NULL);

        retval = YTV_FEED_GET_IFACE (self)->get_parse_strategy (self);

        if (retval != NULL)
        {
                g_assert (YTV_IS_FEED_PARSE_STRATEGY (retval));
        }

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
 * ytv_feed_set_uri_builder:
 * @self: (not-null): a #YtvFeed instance
 * @ub: (not-null): a #YtvUriBuilder implementaiton
 *
 * Set the #YtvUriBuilder for the #YtvFeed
 */
void
ytv_feed_set_uri_builder (YtvFeed* self, YtvUriBuilder* ub)
{
        g_assert (YTV_IS_FEED (self));
        g_assert (ub != NULL);
        g_assert (YTV_IS_URI_BUILDER (ub));
        g_assert (YTV_FEED_GET_IFACE (self)->set_uri_builder != NULL);

        YTV_FEED_GET_IFACE (self)->set_uri_builder (self, ub);

        YtvUriBuilder* test = ytv_feed_get_uri_builder (self);
        g_assert (test);
        g_assert (YTV_IS_URI_BUILDER (test));
        g_assert (test == ub);
        g_object_unref (G_OBJECT (test));

        return;
}

/**
 * ytv_feed_get_uri_builder:
 * @self: (not-null): a #YtvFeed instance
 *
 * Get the assigned #YtvUriBuilder instance
 *
 * return value: (null-ok) (caller-owns): the assigned #YtvUriBuilder
 */
YtvUriBuilder*
ytv_feed_get_uri_builder (YtvFeed* self)
{
        YtvUriBuilder* retval;

        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->get_uri_builder != NULL);

        retval = YTV_FEED_GET_IFACE (self)->get_uri_builder (self);

        if (retval != NULL)
        {
                g_assert (YTV_IS_URI_BUILDER (retval));
        }

        return retval;
}

/**
 * ytv_feed_standard:
 * @self: a #YtvFeed
 * @type: a type of standard of feed
 *
 * Configures the object to retrieve a standard feed indicated by @type
 */
void
ytv_feed_standard (YtvFeed* self, guint type)
{
        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->standard != NULL);

        YTV_FEED_GET_IFACE (self)->standard (self, type);

        return;
}

/**
 * ytv_feed_search:
 * @self: a #YtvFeed
 * @query: the query string
 *
 * Configures the #YtvFeed to retrieve query results
 */
void
ytv_feed_search (YtvFeed* self, const gchar* query)
{
        g_assert (query != NULL);
        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->search != NULL);

        YTV_FEED_GET_IFACE (self)->search (self, query);

        return;
}

/**
 * ytv_feed_user:
 * @self: a #YtvFeed
 * @user: the user id
 *
 * Configures the object to retrieve the feed with tye @user entries
 */
void
ytv_feed_user (YtvFeed* self, const gchar* user)
{
        g_assert (user != NULL);
        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->user != NULL);

        YTV_FEED_GET_IFACE (self)->user (self, user);

        return ;
}

/**
 * ytv_feed_keywords:
 * @self: a #YtvFeed
 * @category: a feed category to browse
 * @keywords: tags set to select
 *
 * Configures the object to retrieve a category and tagged feed
 */
void
ytv_feed_keywords (YtvFeed* self, const gchar* category, const gchar* keywords)
{
        g_assert (category != NULL || keywords != NULL);
        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->keywords != NULL);

        YTV_FEED_GET_IFACE (self)->keywords (self, category, keywords);

        return;
}

/**
 * ytv_feed_related:
 * @self: a #YtvFeed
 * @vid: the entry id 
 *
 * Configures the object to retrieve a feed with entries related to @vid
 */
void
ytv_feed_related (YtvFeed* self, const gchar* vid)
{
        g_assert (vid != NULL);
        g_assert (YTV_IS_FEED (self));
        g_assert (YTV_FEED_GET_IFACE (self)->related != NULL);

        YTV_FEED_GET_IFACE (self)->related (self, vid);

        return;
}

/**
 * YtvGetEntriesCallback:
 * @feed: a #YtvFeed that caused the callback
 * @cancelled: if the operation got cancelled
 * @entries: (null-ok): a #YtvList of fetched #YtvEntry or NULL
 * @err: (null-ok): if an error ocurred
 * @user_data: (null-ok): user data that was passed to the callbacks
 *
 * A feed callback for when a feed fetch was requested. If allocated, you
 * must cleanup @user_data at the end of your implementation of the callback.
 * All other fields in the parameters of the callback are read-only.
 *
 * When cancelled, @cancelled will be TRUE while @err might nonetheless NULL.
 * If @err is not NULL, an error occurred that you should handle gracefully.
 * The @entries parameter might be NULL in case of error or cancellation.
 */

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
        g_assert (callback != NULL);
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
