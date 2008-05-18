/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_JSON_FEED_PARSE_STRATEGY_H_
#define _YTV_JSON_FEED_PARSE_STRATEGY_H_

/* ytv-json-feed-parse-strategy.h - An object which implements a strategy
 *                                  for feed parsing using json-glib
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

#include <ytv-feed-parse-strategy.h>

G_BEGIN_DECLS

#define YTV_TYPE_JSON_FEED_PARSE_STRATEGY               \
        (ytv_json_feed_parse_strategy_get_type ())
#define YTV_JSON_FEED_PARSE_STRATEGY(obj)                               \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_JSON_FEED_PARSE_STRATEGY, YtvJsonFeedParseStrategy))
#define YTV_JSON_FEED_PARSE_STRATEGY_CLASS(klass)                       \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_JSON_FEED_PARSE_STRATEGY, YtvJsonFeedParseStrategyClass))
#define YTV_IS_JSON_FEED_PARSE_STRATEGY(obj)                            \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_JSON_FEED_PARSE_STRATEGY))
#define YTV_IS_JSON_FEED_PARSE_STRATEGY_CLASS(klass)                    \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_JSON_FEED_PARSE_STRATEGY))
#define YTV_JSON_FEED_PARSE_STRATEGY_GET_CLASS(obj)                     \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_JSON_FEED_PARSE_STRATEGY, YtvJsonFeedParseStrategyClass))

typedef struct _YtvJsonFeedParseStrategy YtvJsonFeedParseStrategy;
typedef struct _YtvJsonFeedParseStrategyClass YtvJsonFeedParseStrategyClass;

struct _YtvJsonFeedParseStrategy
{
        GObject parent;
};

struct _YtvJsonFeedParseStrategyClass
{
        GObjectClass parent_class;

        YtvList* (*perform) (YtvFeedParseStrategy* self, const gchar* data,
                             gssize length, GError **err);
        gchar* (*get_mime) (YtvFeedParseStrategy* self);
};

GType ytv_json_feed_parse_strategy_get_type (void);
YtvFeedParseStrategy* ytv_json_feed_parse_strategy_new (void);
YtvList* ytv_json_feed_parse_strategy_perform (YtvFeedParseStrategy *self,
					       const gchar* data, gssize length,
					       GError **err);
gchar* ytv_json_feed_parse_strategy_get_mime (YtvFeedParseStrategy* self);

G_END_DECLS

#endif /* _YTV_JSON_FEED_PARSE_STRATEGY_H_ */
