/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_ITERATOR_H_
#define _YTV_ITERATOR_H_

/* ytv-iterator.h - Interface for iterator pattern
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

G_BEGIN_DECLS

#define YTV_TYPE_ITERATOR \
	(ytv_iterator_get_type ())
#define YTV_ITERATOR(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_ITERATOR, YtvIterator))
#define YTV_IS_ITERATOR(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_ITERATOR))
#define YTV_ITERATOR_GET_IFACE(inst) \
	(G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_ITERATOR, YtvIteratorIface))

typedef struct _YtvList YtvList;
typedef struct _YtvIterator YtvIterator;
typedef struct _YtvIteratorIface YtvIteratorIface;

struct _YtvIteratorIface
{
	GTypeInterface parent;
	
	void (*next_func) (YtvIterator *self);
	void (*prev_func) (YtvIterator *self);
	void (*first_func) (YtvIterator *self);
	void (*nth_func) (YtvIterator *self, guint nth);
	GObject* (*get_current_func) (YtvIterator *self);

	gboolean (*is_done_func) (YtvIterator *self);
	YtvList* (*get_list_func) (YtvIterator *self);
};

GType ytv_iterator_get_type (void);

void ytv_iterator_next (YtvIterator *self);
void ytv_iterator_prev (YtvIterator *self);
void ytv_iterator_first (YtvIterator *self);
void ytv_iterator_nth (YtvIterator *self, guint nth);
GObject* ytv_iterator_get_current (YtvIterator *self);
gboolean ytv_iterator_is_done (YtvIterator *self);
YtvList* ytv_iterator_get_list (YtvIterator *self);

G_END_DECLS

#endif /* _YTV_ITERATOR_H_ */
