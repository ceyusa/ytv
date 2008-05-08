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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <json-glib/json-glib.h>

#include <ytv-error.h>
#include <ytv-json-feed-parse-strategy.h>

typedef struct _YtvJsonFeedParseStrategyPriv YtvJsonFeedParseStrategyPriv;

struct _YtvJsonFeedParseStrategyPriv
{
	JsonParser* parser;
	JsonNode* root;
};

#define YTV_JSON_FEED_PARSE_STRATEGY_GET_PRIVATE(o) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), YTV_TYPE_JSON_FEED_PARSE_STRATEGY, YtvJsonFeedParseStrategyPriv))

void
ytv_json_feed_parse_strategy_perform (YtvFeedParseStrategy* self, )

static void
ytv_feed_parse_strategy_init (YtvFeedParseStrategyIface* klass)
{
	klass->perform = ytv_json_feed_parse_strategy_perform;

	return;
}

G_DEFINE_TYPE_EXTENDED (YtvJsonFeedParseStrategy, ytv_json_feed_parse_strategy,
			G_TYPE_OBJECT, 0,
			G_IMPLEMENT_INTERFACE (YTV_TYPE_FEED_PARSE_STRATEGY,
					       ytv_feed_parse_strategy_init))
