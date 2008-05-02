/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_ENTRY_LIST_H_
#define _YTV_ENTRY_LIST_H_

/* ytv-entry-list.h - List of entries
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

#include <ytv-shared.h>

G_BEGIN_DECLS

#define YTV_TYPE_ENTRY_LIST ytv_entry_list_get_type()

#define YTV_ENTRY_LIST(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_ENTRY_LIST, YtvEntryList))
#define YTV_ENTRY_LIST_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_ENTRY_LIST, YtvEntryListClass))
#define YTV_IS_ENTRY_LIST(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_ENTRY_LIST))
#define YTV_IS_ENTRY_LIST_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_ENTRY_LIST))
#define YTV_ENTRY_LIST_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_ENTRY_LIST, YtvEntryListClass))

struct _YtvEntryList
{
        GObject parent;
        GList* first;
        GMutex* list_lock;
};

struct _YtvEntryListClass
{
        GObjectClass parent_class;
};

GType ytv_entry_list_get_type (void);
YtvList* ytv_entry_list_new (void);

G_END_DECLS

#endif /* _YTV_ENTRY_LIST_H_ */
