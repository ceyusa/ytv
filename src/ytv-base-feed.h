/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_BASE_FEED_H_
#define _YTV_BASE_FEED_H_

/* ytv-base-feed.h - It is a generic feed
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

#include <ytv-feed.h>

G_BEGIN_DECLS

#define YTV_TYPE_BASE_FEED (ytv_base_feed_get_type ())
#define YTV_BASE_FEED(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_BASE_FEED, YtvBaseFeed))
#define YTV_BASE_FEED_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_BASE_FEED, YtvBaseFeedClass))
#define YTV_IS_BASE_FEED(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_BASE_FEED))
#define YTV_IS_BASE_FEED_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_BASE_FEED))
#define YTV_BASE_FEED_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_BASE_FEED, YtvBaseFeedClass))

typedef struct _YtvBaseFeed YtvBaseFeed;
typedef struct _YtvBaseFeedClass YtvBaseFeedClass;

/**
 * YtvBaseFeed:
 *
 * Describes a YouTube video feed
 */
struct _YtvBaseFeed
{
        GObject parent;

        YtvFeedFetchStrategy* fetchst;
        YtvFeedParseStrategy* parsest;
        YtvUriBuilder*        uribuild;
};

struct _YtvBaseFeedClass
{
        GObjectClass parent_class;

        /* Methods */
        YtvFeedFetchStrategy* (*get_fetch_strategy) (YtvFeed* self);
        void (*set_fetch_strategy) (YtvFeed* self, YtvFeedFetchStrategy* st);
        YtvFeedParseStrategy* (*get_parse_strategy) (YtvFeed* self);
        void (*set_parse_strategy) (YtvFeed* self, YtvFeedParseStrategy* st);
        YtvUriBuilder* (*get_uri_builder) (YtvFeed* self);
        void (*set_uri_builder) (YtvFeed* self, YtvUriBuilder* ub);

        void (*standard) (YtvFeed* self, guint type);
        void (*search) (YtvFeed* self, const gchar* query);
        void (*user) (YtvFeed* self, const gchar* user);
        void (*keywords) (YtvFeed* self, const gchar* category,
                          const gchar* keywords);
        void (*related) (YtvFeed* self, const gchar* vid);
        void (*get_entries_async) (YtvFeed* self,
                                   YtvGetEntriesCallback callback,
                                   gpointer user_data);
};

GType ytv_base_feed_get_type (void);

void ytv_base_feed_set_fetch_strategy (YtvFeed* self,
                                       YtvFeedFetchStrategy* st);
YtvFeedFetchStrategy* ytv_base_feed_get_fetch_strategy (YtvFeed* self);
void ytv_base_feed_set_parse_strategy (YtvFeed* self,
                                       YtvFeedParseStrategy* st);
YtvFeedParseStrategy* ytv_base_feed_get_parse_strategy (YtvFeed *self);
YtvUriBuilder* ytv_base_feed_get_uri_builder (YtvFeed* self);
void ytv_base_feed_set_uri_builder (YtvFeed* self, YtvUriBuilder* ub);

void ytv_base_feed_standard (YtvFeed* self, guint type);
void ytv_base_feed_search (YtvFeed* self, const gchar* query);
void ytv_base_feed_user (YtvFeed* self, const gchar* user);
void ytv_base_feed_keywords (YtvFeed* self, const gchar* category,
                             const gchar* keywords);
void ytv_base_feed_related (YtvFeed* self, const gchar* vid);
void ytv_base_feed_get_entries_async (YtvFeed* self,
                                      YtvGetEntriesCallback callback,
                                      gpointer user_data);

G_END_DECLS

#endif /* _YTV_BASE_FEED_H_ */
