/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_GTK_BROWSER_H_
#define _YTV_GTK_BROWSER_H_

/* ytv-gtk-browser.h - Implementation of the browser widget with GTK+
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

#include <gtk/gtktable.h>

#include <ytv-browser.h>

#include <ytv-gtk-entry-view.h>

G_BEGIN_DECLS

#define YTV_TYPE_GTK_BROWSER                          \
        (ytv_gtk_browser_get_type ())
#define YTV_GTK_BROWSER(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_GTK_BROWSER, YtvGtkBrowser))
#define YTV_GTK_BROWSER_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_GTK_BROWSER, YtvGtkBrowserClass))
#define YTV_IS_GTK_BROWSER(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_GTK_BROWSER))
#define YTV_IS_GTK_BROWSER_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_GTK_BROWSER))
#define YTV_GTK_BROWSER_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_GTK_BROWSER, YtvGtkBrowserClass))

typedef struct _YtvGtkBrowser YtvGtkBrowser;
typedef struct _YtvGtkBrowserClass YtvGtkBrowserClass;

/**
 * YtvGtkBrowser:
 *
 * Is a GTK+ view for a #YtvFeed
 */
struct _YtvGtkBrowser
{
        GtkTable parent;

        YtvFeed* feed;
};

struct _YtvGtkBrowserClass
{
        GtkTableClass parent_class;

        /* methods */
        void (*fetch_entries) (YtvBrowser* self);
        gboolean (*next_page) (YtvBrowser* self);
        gboolean (*prev_page) (YtvBrowser* self);
        void (*set_feed) (YtvBrowser* self, YtvFeed* feed);
        YtvFeed* (*get_feed) (YtvBrowser* self);
        YtvEntryView* (*get_focused_entry_view) (YtvBrowser* self);
        void (*clean) (YtvBrowser* self);        
};

GType ytv_gtk_browser_get_type (void);

YtvBrowser* ytv_gtk_browser_new (YtvOrientation orientation);

void ytv_gtk_browser_fetch_entries (YtvBrowser* self);
gboolean ytv_gtk_browser_next_page (YtvBrowser* self);
gboolean ytv_gtk_browser_prev_page (YtvBrowser* self);
void ytv_gtk_browser_set_feed (YtvBrowser* self, YtvFeed* feed);
YtvFeed* ytv_gtk_browser_get_feed (YtvBrowser* self);
YtvEntryView* ytv_gtk_browser_get_focused_entry_view (YtvBrowser* self);
void ytv_gtk_browser_clean (YtvBrowser* self);

G_END_DECLS

#endif /* _YTV_GTK_BROWSER_H_ */
