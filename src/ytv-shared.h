/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_SHARED_H_
#define _YTV_SHARED_H_

/* ytv-shared.h - Shared data declarations
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

/* GTK+ uses G_PRIORITY_HIGH_IDLE + 10 for resizing operations,
 * and G_PRIORITY_HIGH_IDLE + 20 for redrawing operations;
 * this makes sure that status callbacks happen after redraws, so we don't
 * get a lot of notifications but very little visual feedback */
#define YTV_PRIORITY_LOWER_THAN_GTK_REDRAWS G_PRIORITY_HIGH_IDLE + 30

typedef struct _YtvList YtvList;
typedef struct _YtvListIface YtvListIface;
typedef struct _YtvIterator YtvIterator;
typedef struct _YtvIteratorIface YtvIteratorIface;
typedef struct _YtvEntryList YtvEntryList;
typedef struct _YtvEntryListClass YtvEntryListClass;
typedef struct _YtvEntryIterator YtvEntryIterator;
typedef struct _YtvEntryIteratorClass YtvEntryIteratorClass;
typedef struct _YtvFeed YtvFeed;
typedef struct _YtvFeedIface YtvFeedIface;
typedef struct _YtvEntry YtvEntry;
typedef struct _YtvEntryClass YtvEntryClass;

/**
 * YtvGetFeedCallback:
 * @feed: a #YtvFeed that caused the callback
 * @cancelled: if the operation got cancelled
 * @entries: (null-ok): a #YtvList of fetched #YtvEntry or NULL
 * @err: (null-ok): if an error ocurred
 * @user_data: (null-ok): user data that was passed to the callbacks
 *
 * A feed callback for when a feed fetch was requested. If allocated, you
 * must cleanup @user_data at the end of your implementation of the callback.
 * All other fields in the parameters of the callback are read-only.
 *
 * When cancelled, @cancelled will be TRUE while @err might nonetheless NULL.
 * If @err is not NULL, an error occurred that you should handle gracefully.
 * The @entries parameter might be NULL in case of error or cancellation.
 */
void (*YtvGetEntriesCallback) (YtvFeed* feed, gboolean canceled,
			       YtvList* entries, GError *err, gpointer data);


G_END_DECLS

#endif /* _YTV_SHARED_H_ */
