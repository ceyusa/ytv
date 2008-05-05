/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-feed-parse-strategy.c - A type that defines a strategy for
 *                             parse feeds
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
 * SECTION: ytv-feed-parse-strategy
 * @title: YtvFeedParseStrategy
 * @short_description: A type that defines a strategy for parse feeds
 *
 * An abstract type that defines a strategy for parse feed. The
 * implementation mean to be a mechanism to transform the serialized data
 * (json/xml/etc) to a list of entries.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-feed-parse-strategy.h>

/**
 * ytv_feed_parse_strategy_perform:
 * @self: a #YtvFeedParseStrategy instance
 * @data: the serialized data to parse. Each implementation could parse a
 *        a different format, such as json, xml, and so on, or could use
 *        a different parsing library
 * @err: (null-ok): A #GError or NULL
 *
 * Parse the recevied data in order to extract a #YtvList of #YtvEntries
 * contained in a #YtvFeed.
 */
YtvList*
ytv_feed_parse_strategy_perform (YtvFeedParseStrategy* self, gchar* data,
                                 GError **err)
{
        g_assert (YTV_IS_FEED_PARSE_STRATEGY (self));
        g_assert (uri != NULL);
        g_assert (YTV_FEED_PARSE_STRATEGY_GET_IFACE (self)->perform != NULL);

        YtvList* l;
        l = YTV_FEED_PARSE_STRATEGY_GET_IFACE (self)->perform (self, data);

        if (l != NULL)
        {
                g_assert (YTV_IS_LIST (l));
        }

        return l;
}

static void
ytv_feed_parse_strategy_base_init (gpointer g_class)
{
        static gboolean initialized = FALSE;

        if (!initialized)
        {
                initialized = TRUE;
        }

        return;
}

GType
ytv_feed_parse_strategy_get_type (void)
{
	static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GTypeInfo info =
                {
                        sizeof (YtvFeedParseStrategy),
                        ytv_feed_parse_strategy_base_init,
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
                                               "YtvFeedParseStrategy",
                                               &info, 0);

                g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
        }

        return type;
}
