/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_SIMPLE_LIST_PRIV_H_
#define _YTV_SIMPLE_LIST_PRIV_H_

/* ytv-simple-list-priv.h
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

G_BEGIN_DECLS

typedef struct _YtvSimpleListPriv YtvSimpleListPriv;

struct _YtvSimpleListPriv
{
        GList* first;
        GMutex* iterator_lock;
};

#define YTV_SIMPLE_LIST_GET_PRIVATE(o) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), YTV_TYPE_SIMPLE_LIST, YtvSimpleListPriv))

G_END_DECLS

#endif /* _YTV_SIMPLE_LIST_H_ */
