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

static gboolean horizontal = FALSE;
static gboolean vertical = FALSE;
        
static const GOptionEntry entries[] =
{
        { "horizontal", 'h', 0, G_OPTION_ARG_NONE, &horizontal,
          "horizontal layout (default", NULL },
        { "vertical", 'v', 0, G_OPTION_ARG_NONE, &vertical,
          "vertical layout", NULL },
        { NULL }
};

typedef struct _App App;
struct _App
{
        GtkWidget* win;
        GtkWidget* box;
        GtkWidget* entryview[ENTRYNUM];
        YtvFeed* feed;
        gint start_idx;
        gboolean done;
        YtvOrientation orientation;
};

static GtkWidget*
create_entry_view (YtvFeed* feed, YtvOrientation orientation)
{
        YtvFeedFetchStrategy* fetchst;
        YtvUriBuilder* ub;
        YtvEntryView* view;
        
        if (orientation == YTV_ORIENTATION_HORIZONTAL)
        {
                view = ytv_gtk_entry_view_new (YTV_ORIENTATION_VERTICAL);
        } 
        else
        {
                view = ytv_gtk_entry_view_new (YTV_ORIENTATION_HORIZONTAL);
        }
                        
        fetchst = ytv_feed_get_fetch_strategy (feed);
        ytv_gtk_entry_view_set_fetch_strategy (YTV_GTK_ENTRY_VIEW (view),
                                               fetchst);
        g_object_unref (fetchst);

        ub = ytv_feed_get_uri_builder (feed);
        ytv_gtk_entry_view_set_uri_builder (YTV_GTK_ENTRY_VIEW (view), ub);
        g_object_unref (ub);

        gtk_widget_show_all (GTK_WIDGET (view));
        
        return GTK_WIDGET (view);
}

static void
show_entry (App* app, YtvEntry* entry)
{
        static gint idx = 0;

        idx %= ENTRYNUM;

        if (app->entryview[idx] == NULL) /* create view */
        {
                app->entryview[idx] = create_entry_view (app->feed,
                                                         app->orientation);
                gtk_box_pack_start (GTK_BOX (app->box), app->entryview[idx],
                                    TRUE, TRUE, 0);
        }

        ytv_entry_view_set_entry (YTV_ENTRY_VIEW (app->entryview[idx]), entry);

        idx++;

        return;
}

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
                
                entry = YTV_ENTRY (ytv_iterator_get_current (iter));

                show_entry (app, entry);
                
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
        app->orientation = YTV_ORIENTATION_HORIZONTAL;

        for (i = 0; i < ENTRYNUM; i++)
        {
                app->entryview[i] = NULL;
        }
        
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
        GtkWidget* box;
        GtkWidget* prev;
        GtkWidget* next;

        app->win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        /* gtk_widget_set_size_request (win, 400, 600); */
        gtk_window_set_title (GTK_WINDOW (app->win), "YouTube Viewer");

        g_signal_connect (G_OBJECT (app->win), "delete_event",
                          G_CALLBACK (gtk_main_quit), NULL);

        if (app->orientation == YTV_ORIENTATION_HORIZONTAL)
        {
                box = gtk_hbox_new (FALSE, 0);
                app->box = gtk_hbox_new (FALSE, 0);
                prev = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
                next = gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD);
        }
        else
        {
                box = gtk_vbox_new (FALSE, 0);
                app->box = gtk_vbox_new (FALSE, 0);
                prev = gtk_button_new_from_stock (GTK_STOCK_GO_UP);
                next = gtk_button_new_from_stock (GTK_STOCK_GO_DOWN);
        }

        gtk_box_pack_start (GTK_BOX (box), prev, TRUE, TRUE, 2);
        gtk_box_pack_start (GTK_BOX (box), app->box, TRUE, TRUE, 0);
        gtk_box_pack_end (GTK_BOX (box), next, TRUE, TRUE, 2);
        
        gtk_container_add (GTK_CONTAINER (app->win), box);

        gtk_widget_show_all (app->win);

        return;
}

static gboolean
parse_options (App* app, gint* argc, gchar*** argv)
{
        GError* error = NULL;
        GOptionContext* context;
        
        context = g_option_context_new ("- YouTube Viewer");
        g_option_context_add_main_entries (context, entries, NULL);
        g_option_context_add_group (context, gtk_get_option_group (TRUE));

        if (!g_option_context_parse (context, argc, argv, &error))
        {
                g_print ("option parsing failed: %s\n", error->message);
                return FALSE;
        }

        if (horizontal == TRUE)
        {
                app->orientation = YTV_ORIENTATION_HORIZONTAL;
        }
        else if (vertical == TRUE)
        {
                app->orientation = YTV_ORIENTATION_VERTICAL;
        }

        g_option_context_free (context);
        
        return TRUE;
}

gint
main (gint argc, gchar** argv)
{
        App* app;
        
        g_thread_init (NULL);
        gtk_init (&argc, &argv);

        app = app_new ();

        if (!parse_options (app, &argc, &argv))
        {
                goto beach;
        }
        
        app_create_ui (app);
        g_timeout_add_seconds (5, (GSourceFunc) app_fetch_feed, app);

        gtk_main ();

beach:
        app_free (app);
        
        return 0;
}
