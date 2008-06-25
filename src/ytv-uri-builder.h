/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_URI_BUILDER_H_
#define _YTV_URI_BUILDER_H_

/* ytv-uri-builder.h - A type that defines URIs for the video repository
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

#define YTV_TYPE_URI_BUILDER                    \
        (ytv_uri_builder_get_type ())
#define YTV_URI_BUILDER(obj)                                            \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_URI_BUILDER, YtvUriBuilder))
#define YTV_IS_URI_BUILDER(obj)                                         \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_URI_BUILDER))
#define YTV_URI_BUILDER_GET_IFACE(inst)                                 \
        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_URI_BUILDER, YtvUriBuilderIface))

#ifndef _YTV_SHARED_H_
typedef struct _YtvUriBuilder YtvUriBuilder;
typedef struct _YtvUriBuilderIface YtvUriBuilderIface;
#endif

enum _YtvUriBuilderReqFeedType
{
        YTV_URI_BUILDER_REQ_FEED_TYPE_STANDARD = 1,
        YTV_URI_BUILDER_REQ_FEED_TYPE_SEARCH,
        YTV_URI_BUILDER_REQ_FEED_TYPE_USER,
        YTV_URI_BUILDER_REQ_FEED_TYPE_KEYWORDS,
        YTV_URI_BUILDER_REQ_FEED_TYPE_RELATED
};
typedef enum _YtvUriBuilderReqFeedType YtvUriBuilderReqFeedType;

struct _YtvUriBuilderIface
{
        GTypeInterface parent;

        /* methods */
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

GType ytv_uri_builder_get_type (void);

gchar* ytv_uri_builder_get_standard_feed (YtvUriBuilder* self, guint type);
gchar* ytv_uri_builder_search_feed (YtvUriBuilder* self, const gchar* query);
gchar* ytv_uri_builder_get_user_feed (YtvUriBuilder* self, const gchar* user);
gchar* ytv_uri_builder_get_keywords_feed (YtvUriBuilder* self,
                                          const gchar* category,
                                          const gchar* keywords);
gchar* ytv_uri_builder_get_related_feed (YtvUriBuilder* self, const gchar* vid);
gchar* ytv_uri_builder_get_thumbnail (YtvUriBuilder* self, const gchar* vid);
gchar* ytv_uri_builder_get_current_feed (YtvUriBuilder* self);

G_END_DECLS

#endif /* _YTV_URI_BUILDER_H_ */
