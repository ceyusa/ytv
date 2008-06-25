/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_ENTRY_VIEW_H_
#define _YTV_ENTRY_VIEW_H_

/* ytv-entry-view.h - A type that defines a GUI entry view
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

#define YTV_TYPE_ENTRY_VIEW                           \
        (ytv_entry_view_get_type ())
#define YTV_ENTRY_VIEW(obj)                           \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_ENTRY_VIEW, YtvEntryView))
#define YTV_IS_ENTRY_VIEW(obj)                        \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_ENTRY_VIEW))
#define YTV_ENTRY_VIEW_GET_IFACE(inst)                \
        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_ENTRY_VIEW, YtvEntryViewIface))

typedef struct _YtvEntryView YtvEntryView;
typedef struct _YtvEntryViewIface YtvEntryViewIface;

struct _YtvEntryViewIface
{
        GTypeInterface parent;

        /* signals */
        void (*show_details) (YtvEntryView* self);
        void (*play_stream) (YtvEntryView* self);
        void (*link_clicked) (YtvEntryView* self,
                              const gchar* class, const gchar* param);
        
        /* Methods */
        void (*set_entry) (YtvEntryView* self, YtvEntry* entry);
        YtvEntry* (*get_entry) (YtvEntryView* self);

        void (*set_fetch_strategy) (YtvEntryView* self,
                                    YtvFeedFetchStrategy* st);
        YtvFeedFetchStrategy* (*get_fetch_strategy) (YtvEntryView* self);

        void (*set_uri_builder) (YtvEntryView* self, YtvUriBuilder* ub);
        YtvUriBuilder* (*get_uri_builder) (YtvEntryView* self);
        
        void (*clean) (YtvEntryView* self);
};
        
GType ytv_entry_view_get_type (void);

void ytv_entry_view_set_entry (YtvEntryView* self, YtvEntry* entry);
YtvEntry* ytv_entry_view_get_entry (YtvEntryView* self);

void ytv_entry_view_set_fetch_strategy (YtvEntryView* self,
                                        YtvFeedFetchStrategy* st);
YtvFeedFetchStrategy* ytv_entry_view_get_fetch_strategy (YtvEntryView* self);

void ytv_entry_view_set_uri_builder (YtvEntryView* self, YtvUriBuilder* ub);
YtvUriBuilder* ytv_entry_view_get_uri_builder (YtvEntryView* self);

void ytv_entry_view_clean (YtvEntryView* self);

G_END_DECLS

#endif /* _YTV_ENTRY_VIEW_H_ */
