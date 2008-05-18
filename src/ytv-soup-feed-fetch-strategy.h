/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_SOUP_FEED_FETCH_STRATEGY_H_
#define _YTV_SOUP_FEED_FETCH_STRATEGY_H_

/* ytv-soup-feed-fetch-strategy.h - An object which implements the strategy
 *                                  for HTTP request using libsoup
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

#include <ytv-feed-fetch-strategy.h>

G_BEGIN_DECLS

#define YTV_TYPE_SOUP_FEED_FETCH_STRATEGY               \
        (ytv_soup_feed_fetch_strategy_get_type ())
#define YTV_SOUP_FEED_FETCH_STRATEGY(obj)                               \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_SOUP_FEED_FETCH_STRATEGY, YtvSoupFeedFetchStrategy))
#define YTV_SOUP_FEED_FETCH_STRATEGY_CLASS(klass)                       \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_SOUP_FEED_FETCH_STRATEGY, YtvSoupFeedFetchStrategyClass))
#define YTV_IS_SOUP_FEED_FETCH_STRATEGY(obj)                            \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_SOUP_FEED_FETCH_STRATEGY))
#define YTV_IS_SOUP_FEED_FETCH_STRATEGY_CLASS(klass)                    \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_SOUP_FEED_FETCH_STRATEGY))
#define YTV_SOUP_FEED_FETCH_STRATEGY_GET_CLASS(obj)                     \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_SOUP_FEED_FETCH_STRATEGY, YtvSoupFeedFetchStrategyClass))

typedef struct _YtvSoupFeedFetchStrategy YtvSoupFeedFetchStrategy;
typedef struct _YtvSoupFeedFetchStrategyClass YtvSoupFeedFetchStrategyClass;

struct _YtvSoupFeedFetchStrategy
{
        GObject parent;
};

struct _YtvSoupFeedFetchStrategyClass
{
        GObjectClass parent_class;

        void (*perform) (YtvFeedFetchStrategy* self, const gchar* uri,
                         YtvGetResponseCallback callback, gpointer user_data);
};

GType ytv_soup_feed_fetch_strategy_get_type (void);

YtvFeedFetchStrategy* ytv_soup_feed_fetch_strategy_new (void);
void ytv_soup_feed_fetch_strategy_perform (YtvFeedFetchStrategy *self,
                                           const gchar* uri,
                                           YtvGetResponseCallback callback,
                                           gpointer user_data);


G_END_DECLS

#endif /* _YTV_SOUP_FEED_FETCH_STRATEGY_H_ */
