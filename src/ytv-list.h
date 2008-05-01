/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_LIST_H_
#define _YTV_LIST_H_

/* ytv-list.h - Interface for list structures
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

#include <ytv-iterator.h>

G_BEGIN_DECLS

#define YTV_TYPE_LIST \
        (ytv_list_get_type ())
#define YTV_LIST(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_LIST, YtvList))
#define YTV_IS_LIST(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_LIST))
#define YTV_LIST_GET_IFACE(inst) \
        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_LIST, YtvListIface))

typedef struct _YtvListIface YtvListIface;

struct _YtvListIface
{
        GTypeInterface parent;

        guint (*get_length_func) (YtvList *self);
        void (*prepend_func) (YtvList *self, GObject* item);
        void (*append_func) (YtvList *self, GObject* item);
        void (*remove_func) (YtvList *self, GObject* item);
        void (*foreach_func) (YtvList *self, GFunc func, gpointer user_data);
        YtvList* (*copy_func) (YtvList *self);
        YtvIterator* (*create_iterator_func) (YtvList *self);
};

GType ytv_list_get_type (void);

guint ytv_list_get_length (YtvList *self);
void ytv_list_prepend (YtvList *self, GObject* item);
void ytv_list_append (YtvList *self, GObject* item);
void ytv_list_remove (YtvList *self, GObject* item);
void ytv_list_foreach (YtvList *self, GFunc func, gpointer user_data);
YtvIterator* ytv_list_create_iterator (YtvList *self);
YtvList* ytv_list_copy (YtvList *self);

G_END_DECLS

#endif /* _YTV_LIST_H_ */
