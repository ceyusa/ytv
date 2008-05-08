/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* main.c - main function file
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

#include <ytv-entry.h>

#include <ytv-feed-fetch-strategy.h>
#include <ytv-soup-feed-fetch-strategy.h>

GMainLoop *loop;

static void
create_entry ()
{
	YtvEntry *entry = g_object_new (YTV_TYPE_ENTRY,
					"id", "7-moeVw_r_4",
					"author", "bbgames",
					"title", "Muse - Invincible (Official Video)",
					"duration", 274,
					"rating", 4.94,
					"published", "2007-03-16T17:46:04.000-07:00",
					"views", 110401,
					"category", "Music",
					"tags", "absolution, and, bellamy, black, channel, dom, fantastic, four, good, holes, invincible, matt, muse, new, official, revelations, video",
					"description", "Muse - Invincible (Official Video)",
					NULL);

	
	gchar* id = NULL;
	g_object_get (entry, "id", &id, NULL);
	g_print ("id = %s\n", id);
	g_free (id);
	g_object_unref (entry);
	return;
}

static void
fetch_feed_cb (YtvFeedFetchStrategy* st, const gchar* mime,
               const gint8* response, gint64 length, GError *err)
{
        if (err == NULL)
        {
                g_print ("%s\n", (gchar *) response);
        }
        else
        {
                g_print ("%s\n", ytv_error_get_message (err));
                g_error_free (err);
        }

        g_main_loop_quit (loop);
        
        return;
}

static gboolean
fetch_feed ()
{
        gchar* base = "http://gdata.youtube.com/feeds/api/videos?max-results=3&vq=muse+invincible&alt=json";
        /* gchar* base = "http://www.ceyusa.com/feeds/api/videos?max-results=3&vq=muse+invincible&alt=json"; */
        
        YtvFeedFetchStrategy* st = ytv_soup_feed_fetch_strategy_new ();
        ytv_feed_fetch_strategy_perform (st, base, fetch_feed_cb);

        return FALSE;
}        

gint
main (gint argc, gchar** argv)
{
        g_thread_init (NULL);
	g_type_init ();

	create_entry ();

        loop = g_main_loop_new (NULL, TRUE);
        g_idle_add ((GSourceFunc) fetch_feed, NULL);
        g_main_loop_run (loop);
	g_main_loop_unref (loop);
        
	return 0;
}	
