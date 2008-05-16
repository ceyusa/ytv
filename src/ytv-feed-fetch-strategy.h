/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_FEED_FETCH_STRATEGY_H_
#define _YTV_FEED_FETCH_STRATEGY_H_

/* ytv-feed-fetch-strategy.h - A type that defines a strategy for
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

#include <glib-object.h>
#include <ytv-shared.h>

G_BEGIN_DECLS

#define YTV_TYPE_FEED_FETCH_STRATEGY            \
        (ytv_feed_fetch_strategy_get_type ())
#define YTV_FEED_FETCH_STRATEGY(obj)                                    \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_FEED_FETCH_STRATEGY, YtvFeedFetchStrategy))
#define YTV_IS_FEED_FETCH_STRATEGY(obj)                                 \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_FEED_FETCH_STRATEGY))
#define YTV_FEED_FETCH_STRATEGY_GET_IFACE(inst)                         \
        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_FEED_FETCH_STRATEGY, YtvFeedFetchStrategyIface))

#ifndef _YTV_SHARED_H_
typedef struct _YtvFeedFetchStrategy YtvFeedFetchStrategy;
typedef struct _YtvFeedFetchStrategyIface YtvFeedFetchStrategyIface;
typedef void (*YtvGetResponseCallback) (YtvFeedFetchStrategy* st,
                                        const gchar* mimetype,
                                        const gint8* response,
                                        gssize length,
                                        GError *err);
#endif

struct _YtvFeedFetchStrategyIface
{
        GTypeInterface parent;

        void (*perform) (YtvFeedFetchStrategy* self, const gchar* uri,
                         YtvGetResponseCallback callback);
};

GType ytv_feed_fetch_strategy_get_type (void);

void ytv_feed_fetch_strategy_perform (YtvFeedFetchStrategy* self,
                                      const gchar* uri,
                                      YtvGetResponseCallback callback);

G_END_DECLS


#endif /* _YTV_FEED_FETCH_STRATEGY_H_ */
