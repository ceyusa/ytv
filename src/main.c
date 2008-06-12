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
#include <ytv-gtk-entry-view.h>

#include <ytv-rank.h>
#include <ytv-thumbnail.h>

#include <ytv-error.h>
#include <ytv-list.h>
#include <ytv-iterator.h>

#include <gtk/gtk.h>

#define ENTRYNUM 5

typedef struct _App App;
struct _App
{
        GtkWidget* win;
        
        YtvEntryView* entryview[ENTRYNUM];
        YtvFeed* feed;
        gint start_idx;
        gboolean done;
};

static void
feed_entry_cb (YtvFeed* feed, gboolean cancelled, YtvList* list,
               GError **err, gpointer user_data)
{
        App* app;
        YtvIterator* iter;
        
        if (*err != NULL)
        {
                app->done = TRUE;
                
                g_debug ("%s", ytv_error_get_message (*err));
                g_error_free (*err);

                return;
        }

        g_return_if_fail (user_data != NULL);
        g_return_if_fail (list != NULL);
        
        app = (App*) user_data;
        
        iter = ytv_list_create_iterator (list);
        while (!ytv_iterator_is_done (iter))
        {
                YtvEntry* entry;
                static gint idx = 0;
                
                entry = YTV_ENTRY (ytv_iterator_get_current (iter));

                idx %= ENTRYNUM;
                ytv_entry_view_set_entry (app->entryview[idx], entry);
                idx++;
                
                g_object_unref (entry);
                ytv_iterator_next (iter);
        }

        g_object_unref (iter);
        g_object_unref (list);

        return;
}

static gboolean
app_fetch_feed (App* app)
{
        YtvUriBuilder* ub;
        
        g_return_val_if_fail (app != NULL, FALSE);

        ub = ytv_feed_get_uri_builder (app->feed);
        g_object_set (G_OBJECT (ub), "start-index", app->start_idx, NULL);
        g_object_unref (ub);
        app->start_idx += ENTRYNUM;
        
        ytv_feed_standard (app->feed, YTV_YOUTUBE_STD_FEED_MOST_VIEWED); 
        /* ytv_feed_user (app->feed, "pinkipons"); */
        /* ytv_feed_related (app->feed, "FOwQETKKyF0"); */

        ytv_feed_get_entries_async (app->feed, feed_entry_cb, app);

        return !app->done;
}

App*
app_new (void)
{
        gint i;
        App* app;
        YtvFeedFetchStrategy* fetchst;
        YtvFeedParseStrategy* parsest; 
        YtvUriBuilder* ub; 

        app = g_slice_new (App);

        app->win = NULL;
        app->start_idx = 0;
        app->feed = ytv_base_feed_new ();
        app->done = FALSE;
        
        fetchst = ytv_soup_feed_fetch_strategy_new ();
        parsest = ytv_json_feed_parse_strategy_new ();
        ub = ytv_youtube_uri_builder_new ();
        
        g_object_set (G_OBJECT (ub),
                      "max-results", ENTRYNUM,
                      "time", YTV_YOUTUBE_TIME_TODAY,
                      NULL);

        /* assign to feed */
        ytv_feed_set_fetch_strategy (app->feed, fetchst);
        ytv_feed_set_parse_strategy (app->feed, parsest);
        ytv_feed_set_uri_builder (app->feed, ub);

        for (i = 0; i < ENTRYNUM; i++)
        {
                app->entryview[i] =
                        ytv_gtk_entry_view_new (YTV_ORIENTATION_VERTICAL);
                ytv_gtk_entry_view_set_fetch_strategy
                        (YTV_GTK_ENTRY_VIEW (app->entryview[i]), fetchst);
                ytv_gtk_entry_view_set_uri_builder
                        (YTV_GTK_ENTRY_VIEW (app->entryview[i]), ub);
        }                

        g_object_unref (fetchst);
        g_object_unref (parsest);
        g_object_unref (ub);

        return app;
}

void
app_free (App* app)
{
        g_return_if_fail (app != NULL);

        if (app->feed != NULL)
        {
                g_object_unref (app->feed);
        }
        
        g_slice_free (App, app);
}

void
app_create_ui (App* app)
{
        GtkWidget* hbox;
        gint i;

        app->win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        /* gtk_widget_set_size_request (win, 400, 600); */

        g_signal_connect (G_OBJECT (app->win), "delete_event",
                          G_CALLBACK (gtk_main_quit), NULL);

        hbox = gtk_hbox_new (FALSE, 2);
        gtk_container_add (GTK_CONTAINER (app->win), hbox);

        for (i = 0; i < ENTRYNUM; i++)
        {
                gtk_box_pack_start (GTK_BOX (hbox),
                                    GTK_WIDGET (app->entryview[i]),
                                    FALSE, FALSE, 0);
        }

        gtk_widget_show_all (app->win);

        return;
}

gint
main (gint argc, gchar** argv)
{
        App* app;
        
        g_thread_init (NULL);
        gtk_init (&argc, &argv);
        
        app = app_new ();
        app_create_ui (app);
        g_timeout_add_seconds (5, (GSourceFunc) app_fetch_feed, app);

        gtk_main ();

        app_free (app);
        
        return 0;
}
