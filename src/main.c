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

#include <ytv-soup-feed-fetch-strategy.h>
#include <ytv-json-feed-parse-strategy.h>
#include <ytv-youtube-uri-builder.h>
#include <ytv-base-feed.h>

#include <ytv-rank.h>

#include <gtk/gtk.h>

/* GMainLoop *loop; */
/* YtvFeedFetchStrategy* st; */
YtvFeed* feed;

#if 0 /* deprecated */
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
#endif

#if 0 /* deprecated */
static void
fetch_feed_cb (YtvFeedFetchStrategy* st, const gchar* mime,
               const gint8* response, gssize length, GError **err,
               gpointer user_data)
{
        if (*err == NULL)
        {
                GError *tmp_error = NULL;
                YtvFeedParseStrategy* parse_st = ytv_json_feed_parse_strategy_new ();

                if (g_strrstr (mime,
                               ytv_feed_parse_strategy_get_mime (parse_st)) != NULL)
                {
                        YtvList *feed =
                                ytv_feed_parse_strategy_perform (parse_st,
                                                                 response,
                                                                 length,
                                                                 &tmp_error);

                        ytv_list_foreach (feed, ytv_entry_dump, NULL);
                        g_object_unref (feed);
                }

                g_object_unref (parse_st);
        }
        else
        {
                g_print ("%s\n", ytv_error_get_message (*err));
                g_error_free (*err);
        }

        g_main_loop_quit (loop);

        return;
}
#endif

#if 0 /* deprecated */
static gboolean
fetch_feed ()
{
        /* gchar* base = "http://gdata.youtube.com/feeds/api/videos?vq=muse+invincible&alt=json"; */
        /* gchar* base = "http://www.ceyusa.com/feeds/api/videos?max-results=3&vq=muse+invincible&alt=json"; */

        gchar *furi = NULL;

        YtvUriBuilder* ub = ytv_youtube_uri_builder_new ();
/*         g_object_set (G_OBJECT (ub), "max-results", 5, NULL); */
/*         furi = ytv_uri_builder_get_standard_feed */
/*                 (ub, YTV_YOUTUBE_STD_FEED_MOST_RECENT); */
/*         g_warning ("%s", furi); */
/*         ytv_feed_fetch_strategy_perform (st, furi, fetch_feed_cb); */
/*         g_free (furi); */


/*         g_object_set (G_OBJECT (ub), */
/*                       "max-results", 25, */
/*                       "time", YTV_YOUTUBE_TIME_TODAY, */
/*                       NULL); */
/*         furi = ytv_uri_builder_get_standard_feed */
/*                 (ub, YTV_YOUTUBE_STD_FEED_MOST_VIEWED); */

/*         furi = ytv_uri_builder_get_user_feed (ub, "pinkipons"); */

        furi = ytv_uri_builder_get_related_feed (ub, "FOwQETKKyF0");
        g_warning ("%s", furi);
        ytv_feed_fetch_strategy_perform (st, furi, fetch_feed_cb, NULL);
        g_free (furi);

        g_object_unref (ub);

        return FALSE;
}
#endif

#if 0 /* deprecated */
static void
build_uris ()
{
        YtvUriBuilder* ub = ytv_youtube_uri_builder_new ();
        gchar *furi = NULL;

        furi = ytv_uri_builder_get_standard_feed
                (ub, YTV_YOUTUBE_STD_FEED_TOP_RATED);

        g_print ("top rated = %s\n", furi);

        g_free (furi);

        return;
}
#endif

#if 0 /* deprecated */
static void
star_test ()
{
        GtkWidget* win = gtk_window_new (GTK_WINDOW_TOPLEVEL);

        g_signal_connect (G_OBJECT (win), "delete_event",
                          G_CALLBACK (gtk_main_quit), NULL);

        GtkWidget* box = gtk_hbox_new (TRUE, 5);
        gtk_container_add (GTK_CONTAINER (win), box);

        gint i;
        for (i = 0; i < 11; i++)
        {
                GtkWidget *star = ytv_star_new (i * 0.10);
                gtk_widget_set_size_request (star, 24, 24);
                gtk_container_add (GTK_CONTAINER (box), star);
        }

        gtk_widget_show_all (win);
}
#endif

static gboolean
change_rank (gpointer user_data)
{
        gdouble rank = g_random_double_range (0, 5);
        
        g_debug ("changing rank to %f...", rank);
        g_object_set (YTV_RANK (user_data), "rank", rank, NULL);

        return TRUE;
}

static void
rank_test ()
{
        GtkWidget* win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        /* gtk_widget_set_size_request (win, 400, 600); */

        g_signal_connect (G_OBJECT (win), "delete_event",
                          G_CALLBACK (gtk_main_quit), NULL);

        GtkWidget* rank = ytv_rank_new (0.0);
        
        gtk_container_add (GTK_CONTAINER (win), rank);

        g_timeout_add_seconds (2, change_rank, rank);
        
        gtk_widget_show_all (win);
}

static void
feed_entry_cb (YtvFeed* feed, gboolean cancelled, YtvList* list,
               GError **err, gpointer user_data)
{
        
        if (*err != NULL)
        {
                g_print ("%s\n", ytv_error_get_message (*err));
                g_error_free (*err);

                return;
        }

        g_return_if_fail (list != NULL);

        ytv_list_foreach (list, ytv_entry_dump, NULL);

        g_object_unref (list);

        return;
}

static gboolean
fetch_feed (YtvFeed* feed)
{
        YtvFeedFetchStrategy* fetchst = ytv_soup_feed_fetch_strategy_new ();
        YtvFeedParseStrategy* parsest = ytv_json_feed_parse_strategy_new ();
        YtvUriBuilder* ub = ytv_youtube_uri_builder_new ();

        g_object_set (G_OBJECT (ub),
                      "max-results", 5,
                      "time", YTV_YOUTUBE_TIME_TODAY,
                      NULL);

        ytv_feed_set_fetch_strategy (feed, fetchst);
        ytv_feed_set_parse_strategy (feed, parsest);
        ytv_feed_set_uri_builder (feed, ub);

        g_object_unref (fetchst);
        g_object_unref (parsest);
        g_object_unref (ub);

        ytv_feed_standard (feed, YTV_YOUTUBE_STD_FEED_MOST_VIEWED);
        ytv_feed_get_entries_async (feed, feed_entry_cb, NULL);

        return FALSE;
}

gint
main (gint argc, gchar** argv)
{
        g_thread_init (NULL);
        /* g_type_init (); */
        gtk_init (&argc, &argv);

        YtvFeed* feed = ytv_base_feed_new ();
        g_idle_add ((GSourceFunc) fetch_feed, feed);

        /* star_test (); */
        rank_test ();

        gtk_main ();

        g_object_unref (feed);

	/* create_entry (); */
        /* build_uris ();   */

/*         st = ytv_soup_feed_fetch_strategy_new (); */

/*         loop = g_main_loop_new (NULL, TRUE); */
/*         g_idle_add ((GSourceFunc) fetch_feed, NULL); */
/*         g_main_loop_run (loop); */
/* 	g_main_loop_unref (loop); */

/*         g_object_unref (st); */

	return 0;
}
