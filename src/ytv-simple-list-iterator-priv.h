/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_SIMPLE_LIST_ITERATOR_H_
#define _YTV_SIMPLE_LIST_ITERATOR_H_

/* ytv-simple-list-iterator.h - Object for a simple list iterator
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
#include <ytv-list.h>
#include <ytv-iterator.h>

G_BEGIN_DECLS

#define YTV_TYPE_SIMPLE_LIST_ITERATOR \
        (_ytv_simple_list_iterator_get_type ())
#define YTV_SIMPLE_LIST_ITERATOR(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_SIMPLE_LIST_ITERATOR, YtvSimpleListIterator))
#define YTV_SIMPLE_LIST_ITERATOR_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_SIMPLE_LIST_ITERATOR, YtvSimpleListIteratorClass))
#define YTV_IS_SIMPLE_LIST_ITERATOR(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_SIMPLE_LIST_ITERATOR))
#define YTV_IS_SIMPLE_LIST_ITERATOR_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_SIMPLE_LIST_ITERATOR))
#define YTV_SIMPLE_LIST_ITERATOR_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_SIMPLE_LIST_ITERATOR, YtvSimpleListIteratorClass))

typedef struct _YtvSimpleIterator YtvSimpleIterator;
typedef struct _YtvSimpleIteratorClass YtvSimpleIteratorClass;

struct _YtvSimpleListIterator
{
        GObject parent;

        YtvSimpleList* model;
        GList* current;
};

struct _YtvSimpleListIteratorClass
{
        GObjectClass parent;
};

GType _ytv_simple_list_iterator_get_type (void);

YtvIterator* _ytv_simple_list_iterator_new (YtvSimpleList* model);
void _ytv_simple_list_iterator_set_model (YtvSimpleListIterator* self,
                                          YtvSimpleList* model);

G_END_DECLS

#endif /* _YTV_SIMPLE_LIST_ITERATOR_H_ */
