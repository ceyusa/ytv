/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_ENTRY_TEXT_VIEW_H_
#define _YTV_ENTRY_TEXT_VIEW_H_

/* ytv-entry-text-view.h - A type that defines a GTK+ GUI entry view
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

#include <gtk/gtktextview.h>
#include <ytv-entry.h>

G_BEGIN_DECLS

#define YTV_TYPE_ENTRY_TEXT_VIEW (ytv_entry_text_view_get_type ())
#define YTV_ENTRY_TEXT_VIEW(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_ENTRY_TEXT_VIEW, YtvEntryTextView))
#define YTV_ENTRY_TEXT_VIEW_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_ENTRY_TEXT_VIEW, YtvEntryTextViewClass))
#define YTV_IS_ENTRY_TEXT_VIEW(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_ENTRY_TEXT_VIEW))
#define YTV_IS_ENTRY_TEXT_VIEW_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_ENTRY_TEXT_VIEW))
#define YTV_ENTRY_TEXT_VIEW_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_ENTRY_TEXT_VIEW, YtvEntryTextViewClass))

typedef struct _YtvEntryTextView YtvEntryTextView;
typedef struct _YtvEntryTextViewClass YtvEntryTextViewClass;

/**
 * YtvEntryTextView:
 *
 * It is a GTK+ view for the text info in #YtvEntry
 */
struct _YtvEntryTextView
{
        GtkTextView parent;
};

struct _YtvEntryTextViewClass
{
        GtkTextViewClass parent_class;
};

GType ytv_entry_text_view_get_type (void);

GtkWidget* ytv_entry_text_view_new (void);
void ytv_entry_text_view_set_entry (YtvEntryTextView* self, YtvEntry* entry);

G_END_DECLS

#endif /* _YTV_ENTRY_TEXT_VIEW_H_ */
