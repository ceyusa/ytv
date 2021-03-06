/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_ENTRY_H_
#define _YTV_ENTRY_H_

/* ytv-entry.h - Describes a youtube video entry
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

#define YTV_TYPE_ENTRY (ytv_entry_get_type ())
#define YTV_ENTRY(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_ENTRY, YtvEntry))
#define YTV_ENTRY_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_ENTRY, YtvEntryClass))
#define YTV_IS_ENTRY(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_ENTRY))
#define YTV_IS_ENTRY_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_ENTRY))
#define YTV_ENTRY_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_ENTRY, YtvEntryClass))

/**
 * YtvEntry:
 *
 * Describes a YouTube video entry
 */
struct _YtvEntry
{
        GObject parent;
};

struct _YtvEntryClass
{
        GObjectClass parent_class;
};

GType ytv_entry_get_type (void);

void ytv_entry_dump (YtvEntry* self);

G_END_DECLS

#endif /* _YTV_ENTRY_H_ */
