/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_YOUTUBE_URI_BUILDER_H_
#define _YTV_YOUTUBE_URI_BUILDER_H_

/* ytv-youtube-uri-builder.h - A type that defines URIs for the
 * youtube video repository
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

#include <ytv-uri-builder.h>

G_BEGIN_DECLS

#define YTV_TYPE_YOUTUBE_URI_BUILDER		\
        (ytv_youtube_uri_builder_get_type ())
#define YTV_YOUTUBE_URI_BUILDER(obj)					\
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_YOUTUBE_URI_BUILDER, YtvYoutubeUriBuilder))
#define YTV_YOUTUBE_URI_BUILDER_CLASS(klass)				\
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_YOUTUBE_URI_BUILDER, YtvYoutubeUriBuilderClass))
#define YTV_IS_YOUTUBE_URI_BUILDER(obj)					\
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_YOUTUBE_URI_BUILDER))
#define YTV_IS_YOUTUBE_URI_BUILDER_CLASS(klass)				\
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_YOUTUBE_URI_BUILDER))
#define YTV_YOUTUBE_URI_BUILDER_GET_CLASS(obj)				\
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_YOUTUBE_URI_BUILDER, YtvYoutubeUriBuilderClass))

#define YTV_TYPE_YOUTUBE_ORDER (ytv_youtube_order_get_type ())

enum _YtvYoutubeOrder
{
	YTV_YOUTUBE_ORDER_RELEVANCE,
	YTV_YOUTUBE_ORDER_PUBLISHED,
	YTV_YOUTUBE_ORDER_VIEWCOUNT,
	YTV_YOUTUBE_ORDER_RATING
};

typedef enum _YtvYoutubeOrder YtvYoutubeOrder;

#define YTV_TYPE_YOUTUBE_ALT (ytv_youtube_alt_get_type ())

enum _YtvYoutubeAlt
{
	YTV_YOUTUBE_ALT_ATOM,
	YTV_YOUTUBE_ALT_RSS,
	YTV_YOUTUBE_ALT_JSON,
	YTV_YOUTUBE_ALT_JSON_IN_SCRIPT
};

typedef enum _YtvYoutubeAlt YtvYoutubeAlt;

#define YTV_TYPE_YOUTUBE_TIME (ytv_youtube_time_get_type ())

enum _YtvYoutubeTime
{
	YTV_YOUTUBE_TIME_TODAY,
	YTV_YOUTUBE_TIME_THIS_WEEK,
	YTV_YOUTUBE_TIME_THIS_MONTH,
	YTV_YOUTUBE_TIME_ALL_TIME
};

typedef enum _YtvYoutubeTime YtvYoutubeTime;
	
enum _YtvYoutubeStdFeed
{
	YTV_YOUTUBE_STD_FEED_TOP_RATED = 1,
	YTV_YOUTUBE_STD_FEED_TOP_FAVORITES,
	YTV_YOUTUBE_STD_FEED_MOST_VIEWED,
	YTV_YOUTUBE_STD_FEED_MOST_RECENT,
	YTV_YOUTUBE_STD_FEED_MOST_DISCUSSED,
	YTV_YOUTUBE_STD_FEED_MOST_LINKED,
	YTV_YOUTUBE_STD_FEED_MOST_RESPONDED,
	YTV_YOUTUBE_STD_FEED_RECENTLY_FEATURED,
	YTV_YOUTUBE_STD_FEED_WATCH_ON_MOBILE
};

typedef enum _YtvYoutubeStdFeed YtvYoutubeStdFeed;

typedef struct _YtvYoutubeUriBuilder YtvYoutubeUriBuilder;
typedef struct _YtvYoutubeUriBuilderClass YtvYoutubeUriBuilderClass;

struct _YtvYoutubeUriBuilder
{
	GObject parent;
};

struct _YtvYoutubeUriBuilderClass
{
	GObjectClass parent_class;

	/* virtual methods */
        gchar* (*get_standard_feed) (YtvUriBuilder* self, guint type);
        gchar* (*search_feed) (YtvUriBuilder* self, const gchar* query);
        gchar* (*get_user_feed) (YtvUriBuilder* self, const gchar* user);
        gchar* (*get_keywords_feed) (YtvUriBuilder* self,
                                     const gchar* category,
                                     const gchar* keywords);
        gchar* (*get_related_feed) (YtvUriBuilder* self, const gchar* vid);
        gchar* (*get_thumbnail) (YtvUriBuilder* self, const gchar* vid);
        gchar* (*get_current_feed) (YtvUriBuilder* self);
};

GType ytv_youtube_order_get_type (void);
GType ytv_youtube_uri_builder_get_type (void);
GType ytv_youtube_alt_get_type (void);
GType ytv_youtube_time_get_type (void);

YtvUriBuilder* ytv_youtube_uri_builder_new (void);
gchar* ytv_youtube_uri_builder_get_standard_feed (YtvUriBuilder* self,
                                                  guint type);
gchar* ytv_youtube_uri_builder_search_feed (YtvUriBuilder* self,
                                            const gchar* query);
gchar* ytv_youtube_uri_builder_get_user_feed (YtvUriBuilder* self,
                                              const gchar* user);
gchar* ytv_youtube_uri_builder_get_keywords_feed (YtvUriBuilder* self,
                                                  const gchar* category,
                                                  const gchar* keywords);
gchar* ytv_youtube_uri_builder_get_related_feed (YtvUriBuilder* self,
                                                 const gchar* vid);
gchar* ytv_youtube_uri_builder_get_thumbnail (YtvUriBuilder* self,
                                              const gchar* vid);
gchar* ytv_youtube_uri_builder_get_current_feed (YtvUriBuilder* self);

G_END_DECLS

#endif /* _YTV_YOUTUBE_URI_BUILDER_H_ */
