/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_GTK_ENTRY_VIEW_H_
#define _YTV_GTK_ENTRY_VIEW_H_

/* ytv-gtk-entry-view.h - A type that defines a GTK+ GUI entry view
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

#include <ytv-entry-view.h>
#include <gtk/gtktable.h>

G_BEGIN_DECLS

#define YTV_TYPE_GTK_ENTRY_VIEW (ytv_gtk_entry_view_get_type ())
#define YTV_GTK_ENTRY_VIEW(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_GTK_ENTRY_VIEW, YtvGtkEntryView))
#define YTV_GTK_ENTRY_VIEW_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_GTK_ENTRY_VIEW, YtvGtkEntryViewClass))
#define YTV_IS_GTK_ENTRY_VIEW(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_GTK_ENTRY_VIEW))
#define YTV_IS_GTK_ENTRY_VIEW_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_GTK_ENTRY_VIEW))
#define YTV_GTK_ENTRY_VIEW_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_GTK_ENTRY_VIEW, YtvGtkEntryViewClass))

enum _YtvOrientation
{
        YTV_ORIENTATION_UNDEF,
        YTV_ORIENTATION_HORIZONTAL,
        YTV_ORIENTATION_VERTICAL
};

typedef enum _YtvOrientation YtvOrientation;

#define YTV_TYPE_ORIENTATION (ytv_orientation_get_type ())

typedef struct _YtvGtkEntryView YtvGtkEntryView;
typedef struct _YtvGtkEntryViewClass YtvGtkEntryViewClass;

/**
 * YtvGtkEntryView:
 *
 * Is a GTK+ view for a #YtvEntry
 */
struct _YtvGtkEntryView
{
        GtkTable parent;

        YtvEntry* entry;
};

struct _YtvGtkEntryViewClass
{
        GtkTableClass parent_class;

        void (*set_entry) (YtvEntryView* self, YtvEntry* entry);
        YtvEntry* (*get_entry) (YtvEntryView* self);
        void (*clean) (YtvEntryView* self);
};

GType ytv_orientation_get_type (void);

GType ytv_gtk_entry_view_get_type (void);

YtvEntryView* ytv_gtk_entry_view_new (YtvOrientation orientation);

void ytv_gtk_entry_view_set_entry (YtvEntryView* self, YtvEntry* entry);
YtvEntry* ytv_gtk_entry_view_get_entry (YtvEntryView* self);
void ytv_gtk_entry_view_clean (YtvEntryView* self);

void ytv_gtk_entry_view_set_orientation (YtvGtkEntryView* self,
                                         YtvOrientation orientation);
YtvOrientation ytv_gtk_entry_view_get_orientation (YtvGtkEntryView* self);

void ytv_gtk_entry_view_set_fetch_strategy (YtvGtkEntryView* self,
                                            YtvFeedFetchStrategy* st);
void ytv_gtk_entry_view_set_uri_builder (YtvGtkEntryView* self,
                                         YtvUriBuilder* ub);

G_END_DECLS

#endif /* _YTV_GTK_ENTRY_VIEW_H_ */
