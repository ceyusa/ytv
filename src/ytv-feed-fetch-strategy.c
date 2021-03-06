/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-feed-fetch-strategy.c - A type that defines a strategy for
 *                             fetch feeds
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
 * SECTION: ytv-feed-fetch-strategy
 * @title: YtvFeedFetchStrategy
 * @short_description: A type that defines a strategy for fetch feeds
 *
 * An abstract type that defines a strategy for fetch feed. The
 * implementation mean to be an asynchronous mechanism for HTTP requests.
 */

/**
 * YtvFeedFetchStrategy:
 *
 * A strategy for fetching resources trough the HTTP protocol
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-feed-fetch-strategy.h>

/**
 * YtvGetResponseCallback:
 * @st: a #YtvFeedFetchStrategy that caused the callback
 * @mimetype: (null-ok): the response's MIME type
 * @response: (null-ok): the response data
 * @length: length of the response data buffer or -1
 * @err: (null-ok): if an error occurred
 *
 * A callback for when a response is retrieved. The @mimetype and the
 * @response might be NULL in case of error.
 */

/**
 * ytv_feed_fetch_strategy_perform:
 * @self: a #YtvFeedFetchStrategy instance
 * @uri: the URI to fetch
 * @callback: the #YtvGetResponseCallback callback with the result
 *
 * Performs the async fetch of a feed through HTTP.
 */
void
ytv_feed_fetch_strategy_perform (YtvFeedFetchStrategy* self, const gchar* uri,
                                 YtvGetResponseCallback callback,
                                 gpointer user_data)
{
        g_assert (YTV_IS_FEED_FETCH_STRATEGY (self));
        g_assert (uri != NULL);
        g_assert (YTV_FEED_FETCH_STRATEGY_GET_IFACE (self)->perform != NULL);

        YTV_FEED_FETCH_STRATEGY_GET_IFACE (self)->perform (self, uri, callback,
                                                           user_data);
        
        return;
}

/**
 * ytv_feed_fetch_strategy_encode:
 * @self: (not-null): the #YtvFeedFetchStrategy implementation
 * @part: (not-null): the string to encode
 *
 * This will implement the library specific implementation string encoder
 *
 * return value: (not-null): a new allocated encoded string. Free it after use.
 */
gchar*
ytv_feed_fetch_strategy_encode (YtvFeedFetchStrategy* self,
                                const gchar* part)
{
        gchar* retval;
        
        g_assert (YTV_IS_FEED_FETCH_STRATEGY (self));
        g_assert (part != NULL);

        g_assert (YTV_FEED_FETCH_STRATEGY_GET_IFACE (self)->encode != NULL);

        retval = YTV_FEED_FETCH_STRATEGY_GET_IFACE (self)->encode (self, part);

        g_assert (retval != NULL);
        
        return retval;
}

/**
 * ytv_feed_fetch_strategy_get_date:
 * @self: (not-null): a #YtvFeedFetchStrategy instance
 * @data: (not-null): a date string to convert
 *
 * Parse a string date given normally by a web server and converts it to
 * a time_t structure
 *
 * return value: the number of seconds since 00:00:00 1970/01/01 UTC
 */
time_t
ytv_feed_fetch_strategy_get_date (YtvFeedFetchStrategy* self,
                                  const gchar* datestr)
{
        time_t retval;
        
        g_assert (YTV_IS_FEED_FETCH_STRATEGY (self));
        g_assert (datestr != NULL);

        g_assert (YTV_FEED_FETCH_STRATEGY_GET_IFACE (self)->get_date != NULL);

        retval =
                YTV_FEED_FETCH_STRATEGY_GET_IFACE (self)->get_date (self,
                                                                    datestr);

        g_assert (retval != -1);
        
        return retval;
}

static void
ytv_feed_fetch_strategy_base_init (gpointer g_class)
{
        static gboolean initialized = FALSE;

        if (!initialized)
        {
                initialized = TRUE;
        }

        return;
}

GType
ytv_feed_fetch_strategy_get_type (void)
{
	static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GTypeInfo info =
                {
                        sizeof (YtvFeedFetchStrategyIface),
                        ytv_feed_fetch_strategy_base_init,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        0,
                        0,
                        NULL,
                        NULL
                };

                type = g_type_register_static (G_TYPE_INTERFACE,
                                               "YtvFeedFetchStrategy",
                                               &info, 0);

                g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
        }

        return type;
}
