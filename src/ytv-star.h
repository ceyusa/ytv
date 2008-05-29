/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_STAR_H_
#define _YTV_STAR_H_

/* ytv-star.h - draws a star
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
#include <gtk/gtkdrawingarea.h>

G_BEGIN_DECLS

#define YTV_TYPE_STAR (ytv_star_get_type ())
#define YTV_STAR(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_STAR, YtvStar))
#define YTV_STAR_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_STAR, YtvStarClass))
#define YTV_IS_STAR(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_STAR))
#define YTV_IS_STAR_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_STAR))
#define YTV_STAR_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_STAR, YtvStarClass))

typedef struct _YtvStar YtvStar;
typedef struct _YtvStarClass YtvStarClass;

/**
 * YtvStar:
 *
 * Draws a filled star given a rank from 0..10
 */
struct _YtvStar
{
        GtkDrawingArea parent;
};

struct _YtvStarClass
{
        GtkDrawingAreaClass parent_class;
};

GType      ytv_star_get_type (void);
GtkWidget* ytv_star_new (gfloat percentage);

G_END_DECLS

#endif /* _YTV_STAR_H_ */
