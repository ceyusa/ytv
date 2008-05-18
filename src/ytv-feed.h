/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_FEED_H_
#define _YTV_FEED_H_

/* ytv-feed.h - A type that defines a feed
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

#define YTV_TYPE_FEED                           \
        (ytv_feed_get_type ())
#define YTV_FEED(obj)                           \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_FEED, YtvFeed))
#define YTV_IS_FEED(obj)                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_FEED))
#define YTV_FEED_GET_IFACE(inst)                \
        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_FEED, YtvFeedIface))

#ifndef _YTV_SHARED_H_
typedef struct _YtvFeed YtvFeed;
typedef struct _YtvFeedIface YtvFeedIface;
typedef void (*YtvGetEntriesCallback) (YtvFeed* feed, gboolean cancelled,
                                       YtvList* entries, GError *err,
                                       gpointer user_data);
#endif

struct _YtvFeedIface
{
        GTypeInterface parent;

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

GType ytv_feed_get_type (void);

void ytv_feed_set_fetch_strategy (YtvFeed* self, YtvFeedFetchStrategy* st);
YtvFeedFetchStrategy* ytv_feed_get_fetch_strategy (YtvFeed* self);
void ytv_feed_set_parse_strategy (YtvFeed* self, YtvFeedParseStrategy* st);
YtvFeedParseStrategy* ytv_feed_get_parse_strategy (YtvFeed *self);
YtvUriBuilder* ytv_feed_get_uri_builder (YtvFeed* self);
void ytv_feed_set_uri_builder (YtvFeed* self, YtvUriBuilder* ub);

void ytv_feed_standard (YtvFeed* self, guint type);
void ytv_feed_search (YtvFeed* self, const gchar* query);
void ytv_feed_user (YtvFeed* self, const gchar* user);
void ytv_feed_keywords (YtvFeed* self, const gchar* category,
                        const gchar* keywords);
void ytv_feed_related (YtvFeed* self, const gchar* vid);
void ytv_feed_get_entries_async (YtvFeed* self, YtvGetEntriesCallback callback,
                                 gpointer user_data);

G_END_DECLS

#endif /* _YTV_FEED_H_ */
