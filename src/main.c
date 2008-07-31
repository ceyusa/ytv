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
#include <ytv-error.h>
#include <ytv-shell.h>
#include <ytv-gtk-browser.h>

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
        GtkWidget* shell;
        YtvFeed* feed;
        YtvOrientation orientation;
};

static void
error_raised_cb (YtvShell* shell, GError *err, gpointer user_data)
{
        GtkWidget* dialog;
        
        dialog = gtk_message_dialog_new (NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_ERROR,
                                         GTK_BUTTONS_OK,
                                         ytv_error_get_message (err));
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
                
        g_error_free (err);

        return;
}

static gboolean
app_fetch_feed (App* app)
{
        YtvBrowser* browser;

        browser = ytv_shell_get_browser (YTV_SHELL (app->shell));
        ytv_browser_set_feed (browser, app->feed);
        ytv_browser_fetch_entries (browser);
        g_object_unref (browser);

        return FALSE;
}


static App*
app_new (void)
{
        App* app;
        YtvFeedFetchStrategy* fetchst;
        YtvFeedParseStrategy* parsest; 
        YtvUriBuilder* ub;

        app = g_slice_new (App);

        app->win = NULL;
        app->orientation = YTV_ORIENTATION_HORIZONTAL;

        app->feed = ytv_base_feed_new ();

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

        /* initial feed to show */
        /* ytv_feed_standard (app->feed, YTV_YOUTUBE_STD_FEED_MOST_RECENT); */
        ytv_feed_standard (app->feed, YTV_YOUTUBE_STD_FEED_MOST_VIEWED);
        /* ytv_feed_user (app->feed, "pinkipons"); */
        /* ytv_feed_related (app->feed, "FOwQETKKyF0"); */
        /* ytv_feed_search (app->feed, "café tacvba"); */
        
        return app;
}

static void
app_free (App* app)
{
        g_return_if_fail (app != NULL);

        if (app->feed != NULL)
        {
                g_object_unref (app->feed);
        }
        
        g_slice_free (App, app);
}

static void
app_create_ui (App* app)
{
        GtkWidget* box;

        app->win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        /* gtk_widget_set_size_request (win, 400, 600); */
        gtk_window_set_title (GTK_WINDOW (app->win), "YouTube Viewer");

        g_signal_connect (G_OBJECT (app->win), "delete_event",
                          G_CALLBACK (gtk_main_quit), NULL);

        box = gtk_hbox_new (FALSE, 0);

        app->shell = ytv_shell_new ();
        g_signal_connect (app->shell, "error-raised",
                          G_CALLBACK (error_raised_cb), app);
        gtk_box_pack_start (GTK_BOX (box), app->shell, TRUE, TRUE, 0);
        
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
        /* g_timeout_add_seconds (5, (GSourceFunc) app_fetch_feed, app); */
        g_idle_add ((GSourceFunc) app_fetch_feed, (gpointer) app);
        
        gtk_main ();

beach:
        app_free (app);
        
        return 0;
}
