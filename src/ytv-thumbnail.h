/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_THUMBNAIL_H_
#define _YTV_THUMBNAIL_H_

/* ytv-thumbnail.h - show an image thumbnail
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

#include <gtk/gtkalignment.h>

#include <ytv-feed-fetch-strategy.h>
#include <ytv-uri-builder.h>

G_BEGIN_DECLS

#define YTV_TYPE_THUMBNAIL (ytv_thumbnail_get_type ())
#define YTV_THUMBNAIL(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_THUMBNAIL, YtvThumbnail))
#define YTV_THUMBNAIL_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_THUMBNAIL, YtvThumbnailClass))
#define YTV_IS_THUMBNAIL(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_THUMBNAIL))
#define YTV_IS_THUMBNAIL_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_THUMBNAIL))
#define YTV_THUMBNAIL_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_THUMBNAIL, YtvThumbnailClass))

typedef struct _YtvThumbnail YtvThumbnail;
typedef struct _YtvThumbnailClass YtvThumbnailClass;

/**
 * YtvThumbnail:
 *
 * Draws an image thumbnail
 */
struct _YtvThumbnail
{
        GtkAlignment parent;

        GtkWidget* button;
        GtkWidget* evbox;
        GtkWidget* image;
};

struct _YtvThumbnailClass
{
        GtkAlignmentClass parent_class;
};

GType      ytv_thumbnail_get_type (void);
GtkWidget* ytv_thumbnail_new (void);

void ytv_thumbnail_set_fetch_strategy (YtvThumbnail* self,
                                       YtvFeedFetchStrategy* fetcher);
YtvFeedFetchStrategy* ytv_thumbnail_get_fetch_strategy (YtvThumbnail* self);
void ytv_thumbnail_set_uri_builder (YtvThumbnail* self, YtvUriBuilder *ub);
YtvUriBuilder* ytv_thumbnail_get_uri_builder (YtvThumbnail* self);
void ytv_thumbnail_set_id (YtvThumbnail* self, const gchar* id);
gchar* ytv_thumbnail_get_id (YtvThumbnail* self);
void ytv_thumbnail_clean (YtvThumbnail* self);

G_END_DECLS


#endif /* _YTV_THUMBNAIL_H_ */
