/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

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

#include <libsoup/soup.h>

#include <ytv-soup-feed-fetch-strategy.h>

typedef struct _YtvSoupFeedFetchStrategyPriv YtvSoupFeedFetchStrategyPriv;

struct _YtvSoupFeedFetchStrategyPriv
{
	SoupSession* session;
};

static void
ytv_soup_feed_fetch_strategy_perform (YtvFeedFetchStrategy *self, gchar* uri)
{
	YTV_SOUP_FEED_FETCH_STRATEGY_GET_CLASS (self)->perform (self, uri);

	return;
}

static void
ytv_soup_feed_fetch_strategy_perform_default (YtvFeedFetchStrategy *self,
					      gchar* uri)
{
}

static void
ytv_feed_fetch_strategy_init (YtvFeedFetchStrategyIface* klass)
{
	klass->perform = ytv_soup_feed_fetch_strategy_perform;

	return;
}

G_DEFINE_TYPE_EXTENDED (YtvSoupFeedFetchStrategy, yt_soup_feed_fetch,
			G_TYPE_OBJECT, 0,
			G_IMPLEMENT_INTERFACE (YTV_FEED_FETCH_STRATEGY,
					       ytv_feed_fetch_strategy_init))

static void
ytv_soup_feed_fetch_strategy_class_init (YtvSoupFeedFetchStrategyClass* klass)
{
	GOjbectClass *g_klass;

	g_klass = G_OBJECT_CLASS (klass);
	
	klass->perform = ytv_soup_feed_fetch_strategy_perform_default;
	g_klass->finalize = ytv_soup_feed_fetch_strategy_finalize;

	return;
}

static void
ytv_soup_feed_fetch_strategy_init (YtvSoupFeedFetchStrategy* self)
{
	return;
}

YtvFeedFetchStrategy*
ytv_soup_feed_fetch_strategy_new (void)
{
	YtvSoupFeedFetchStrategy* self = g_object_new
		(YTV_SOUP_FEED_FETCH_STRATEGY, NULL);

	return YTV_FEED_FETCH_STRATEGY (self);
}
