/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_BROWSER_H_
#define _YTV_BROWSER_H_

/* ytv-browser.h - An interface for an entry browser widget
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

#include <ytv-feed.h>
#include <ytv-entry-view.h>

G_BEGIN_DECLS

#define YTV_TYPE_BROWSER              \
        (yt_browser_get_type ())
#define YTV_BROWSER(obj)              \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_BROWSER, YtvBrowser))
#define YTV_IS_BROWSER(obj)           \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_BROWSER))
#define YTV_BROWSER_GET_IFACE(inst)   \
        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_BROWSER, YtvBrowserIface))

typedef struct _YtvBrowser YtvBrowser;
typedef struct _YtvBrowserIface YtvBrowser;

struct _YtvBrowserIface
{
        GTypeInterface parent;

        /* signals */
        void (*error_raised) (YtvBrowser* self, GError* error);
        void (*last_page) (YtvBrowser* self);
        void (*first_page) (YtvBrowser* self);
        void (*entry_details) (YtvBrowser* self, YtvEntry* entry);
        void (*play_stream) (YtvBrowser* self, YtvEntry* entry);

        /* methods */
        void (*fetch_entries) (YtvBrowser* self);
        gboolean (*next_page) (YtvBrowser* self);
        gboolean (*prev_page) (YtvBrowser* self);
        void (*set_feed) (YtvBrowser* self, YtvFeed* feed);
        YtvFeed* (*get_feed) (YtvBrowser* self);
        YtvEntryView* (*get_focused_entry_view) (YtvBrowser* self);
        void (*clean) (YtvBrowser* self);
};

GType ytv_browser_get_type (void);

void ytv_browser_fetch_entries (YtvBrowser* self);
gboolean ytv_browser_next_page (YtvBrowser* self);
gboolean ytv_browser_prev_page (YtvBrowser* self);
void ytv_browser_set_feed (YtvBrowser* self, YtvFeed* feed);
YtvFeed* ytv_browser_get_feed (YtvBrowser* self);
YtvEntreView* ytv_browser_get_focused_entry_view (YtvBrowser* self);
void ytv_browser_clean (YtvBrowser* self);

G_END_DECLS

#endif /* _YTV_BROWSER_H_ */
