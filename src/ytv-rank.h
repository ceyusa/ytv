/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_RANK_H_
#define _YTV_RANK_H_

/* ytv-rank.h - Draws the entry rank with filled stars
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
#include <gtk/gtkhbox.h>

G_BEGIN_DECLS

#define YTV_TYPE_RANK (ytv_rank_get_type ())
#define YTV_RANK(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_RANK, YtvRank))
#define YTV_RANK_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_RANK, YtvRankClass))
#define YTV_IS_RANK(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_RANK))
#define YTV_IS_RANK_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_RANK))
#define YTV_RANK_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_RANK, YtvRankClass))

typedef struct _YtvRank YtvRank;
typedef struct _YtvRankClass YtvRankClass;

/**
 * YtvRank:
 *
 * Draws a filled rank given a rank from 0..10
 */
struct _YtvRank
{
        GtkHBox parent;
};

struct _YtvRankClass
{
        GtkHBoxClass parent_class;
};

GType ytv_rank_get_type (void);
GtkWidget* ytv_rank_new (gfloat rank);
void ytv_rank_set_rank (YtvRank*self, gfloat rank);

G_END_DECLS

#endif /* _YTV_RANK_H_ */
