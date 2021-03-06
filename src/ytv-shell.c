/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-shell.c - GTK+ widget for graphical shell
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

#include <ytv-shell.h>

#include <ytv-gtk-browser.h>
#include <ytv-youtube-uri-builder.h>

#include <gtk/gtk.h>

enum _YtvShellSignals
{
        ERROR_RAISED,
        LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

typedef struct _YtvShellPriv YtvShellPriv;
struct _YtvShellPriv
{
        YtvBrowser* browser;
        GtkWidget* last_focused;
        GtkWidget* next;
        GtkWidget* prev;
        GtkWidget* search_entry;
};

#define YTV_SHELL_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_SHELL, YtvShellPriv))

G_DEFINE_TYPE (YtvShell, ytv_shell, GTK_TYPE_NOTEBOOK)

static void
error_raised_cb (YtvBrowser *browser, GError *err, gpointer user_data)
{
        YtvShell* self = YTV_SHELL (user_data);

        /* @todo update the status bar */
        g_signal_emit (self, signals[ERROR_RAISED], 0, err);

        return;
}

static void
first_page_cb (YtvBrowser* browser, gpointer user_data)
{
        YtvShellPriv* priv;
        YtvShell* self = YTV_SHELL (user_data);
        
        priv = YTV_SHELL_GET_PRIVATE (self);

        gtk_widget_set_sensitive (priv->prev, FALSE);

        return;
}

static void
last_page_cb (YtvBrowser* browser, gpointer user_data)
{
        YtvShellPriv* priv;
        YtvShell* self = YTV_SHELL (user_data);
        
        priv = YTV_SHELL_GET_PRIVATE (self);

        gtk_widget_set_sensitive (priv->next, FALSE);

        return;
}

static void
next_page_cb (GtkWidget* wid, gpointer user_data)
{
        YtvShellPriv* priv;
        YtvShell* self = YTV_SHELL (user_data);
        
        priv = YTV_SHELL_GET_PRIVATE (self);

        ytv_browser_next_page (priv->browser);

        gtk_widget_set_sensitive (priv->prev, TRUE);
        gtk_widget_set_sensitive (priv->next, TRUE);

        return;
}

static void
prev_page_cb (GtkWidget* wid, gpointer user_data)
{
        YtvShellPriv* priv;
        YtvShell* self = YTV_SHELL (user_data);
        
        priv = YTV_SHELL_GET_PRIVATE (self);

        ytv_browser_prev_page (priv->browser);

        gtk_widget_set_sensitive (priv->prev, TRUE);
        gtk_widget_set_sensitive (priv->next, TRUE);

        return;
}

static void
search_focus_cb (GtkContainer *container, GtkWidget* w, gpointer user_data)
{
        YtvShell* self = YTV_SHELL (user_data);
        
        if (w != NULL)
        {
                YtvShellPriv* priv = YTV_SHELL_GET_PRIVATE (self);

                priv->last_focused = w;
        }

        return;
}

static void
search_clicked_cb (GtkWidget* button, gpointer user_data)
{
        YtvShellPriv* priv;
        YtvShell* self = YTV_SHELL (user_data);
        
        priv = YTV_SHELL_GET_PRIVATE (self);
        gtk_widget_activate (priv->search_entry);

        return;
}

static void
search_activated_cb (GtkWidget* entry, gpointer user_data)
{
        const gchar* q;

        q = gtk_entry_get_text (GTK_ENTRY (entry));

        if (q != NULL)
        {
                YtvFeed* feed;
                YtvShell* self;
                YtvShellPriv* priv;

                self = YTV_SHELL (user_data);
                priv = YTV_SHELL_GET_PRIVATE (self);

                /* @todo update status bar */
                feed = ytv_browser_get_feed (priv->browser);

                if (feed != NULL)
                {
                        YtvUriBuilder* ub = ytv_feed_get_uri_builder (feed);
                        g_object_set (G_OBJECT (ub),
                                      "time", YTV_YOUTUBE_TIME_ALL_TIME,
                                      "start-index", 0, NULL);
                        g_object_unref (ub);
                        
                        ytv_feed_search (feed, q);
                        g_object_unref (feed);

                        ytv_browser_fetch_entries (priv->browser);
                        
                        gtk_widget_set_sensitive (priv->prev, TRUE);
                        gtk_widget_set_sensitive (priv->next, TRUE);
                }
                else
                {
                        g_warning ("You forgot to set a feed in the browser");
                }
        }

        return;
}

static GtkWidget*
create_browser_tab (YtvShell* self)
{
        YtvShellPriv* priv;
        GtkWidget* box;
        GtkWidget* searchbox;
        GtkWidget* searchentry;
        GtkWidget* searchbutton;
        GtkWidget* scrolledwin;

        priv = YTV_SHELL_GET_PRIVATE (self);
        
        box = gtk_vbox_new (FALSE, 0);
        
        priv->browser = ytv_gtk_browser_new (YTV_ORIENTATION_HORIZONTAL);
        g_signal_connect (priv->browser, "error-raised",
                          G_CALLBACK (error_raised_cb), self);
        g_signal_connect (priv->browser, "last-page",
                          G_CALLBACK (last_page_cb), self);
        g_signal_connect (priv->browser, "first-page",
                          G_CALLBACK (first_page_cb), self);


        scrolledwin = gtk_scrolled_window_new (NULL, NULL);
        g_object_set (scrolledwin, "scrollbar-spacing", 0, NULL);
        gtk_scrolled_window_set_policy (
                GTK_SCROLLED_WINDOW (scrolledwin),
                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_shadow_type (
                GTK_SCROLLED_WINDOW (scrolledwin), GTK_SHADOW_NONE);
        gtk_scrolled_window_add_with_viewport (
                GTK_SCROLLED_WINDOW (scrolledwin),
                GTK_WIDGET (priv->browser)
                );
        
        gtk_box_pack_start (GTK_BOX (box), scrolledwin, TRUE, TRUE, 0);

        searchbox = gtk_hbox_new (FALSE, 0);
        g_signal_connect (searchbox, "set-focus-child",
                          G_CALLBACK (search_focus_cb), self);
        gtk_box_pack_start (GTK_BOX (box), searchbox, FALSE, FALSE, 0);

        searchbutton = GTK_WIDGET (
                gtk_tool_button_new_from_stock (GTK_STOCK_FIND)
                );
        g_signal_connect (searchbutton, "clicked",
                          G_CALLBACK (search_clicked_cb), self);
        gtk_box_pack_start (GTK_BOX (searchbox), searchbutton, FALSE, FALSE, 0);

        searchentry = gtk_combo_box_entry_new ();
        priv->search_entry = gtk_bin_get_child (GTK_BIN (searchentry));
        g_signal_connect (priv->search_entry, "realize",
                          G_CALLBACK (gtk_widget_grab_focus), NULL);
        g_signal_connect (priv->search_entry, "activate",
                          G_CALLBACK (search_activated_cb), self);
        gtk_box_pack_start (GTK_BOX (searchbox), searchentry, FALSE, FALSE, 0);

        priv->prev = GTK_WIDGET (
                gtk_tool_button_new_from_stock (GTK_STOCK_GO_BACK)
                );
        g_signal_connect (priv->prev, "clicked",
                          G_CALLBACK (prev_page_cb), self);
        gtk_box_pack_start (GTK_BOX (searchbox), priv->prev, FALSE, FALSE, 0);

        priv->next = GTK_WIDGET (
                gtk_tool_button_new_from_stock (GTK_STOCK_GO_FORWARD)
                );
        g_signal_connect (priv->next, "clicked",
                          G_CALLBACK (next_page_cb), self);
        gtk_box_pack_start (GTK_BOX (searchbox), priv->next, FALSE, FALSE, 0);

        priv->last_focused = searchbutton;
        
        return box;
}

static GtkWidget*
create_player_tab (YtvShell* self)
{
        return NULL;
}

static GtkWidget*
create_info_tab (YtvShell* self)
{
        return NULL;
}

static void
create_ui (YtvShell* self)
{
        gtk_notebook_append_page (GTK_NOTEBOOK (self),
                                  create_browser_tab (self),
                                  gtk_label_new ("Browser"));
        gtk_notebook_append_page (GTK_NOTEBOOK (self),
                                  create_player_tab (self),
                                  gtk_label_new ("Player"));
        gtk_notebook_append_page (GTK_NOTEBOOK (self),
                                  create_info_tab (self),
                                  gtk_label_new ("Info"));

/*         g_signal_connect (self, "switch-page", */
/*                           G_CALLBACK (switch_page_cb), NULL); */

        /* @todo update the status bar */
}

static void
ytv_shell_class_init (YtvShellClass* klass)
{
        GObjectClass* object_class;

        object_class = G_OBJECT_CLASS (klass);
        
        g_type_class_add_private (object_class, sizeof (YtvShellPriv));

        /**
         * YtvShell::error-raised:
         * @self: the #YtvShell instance that emitted the signal
         * @err: the raised #GError
         *
         * The ::error-raised signal is emmited when something went wrong
         */
        signals[ERROR_RAISED] = g_signal_new ("error-raised",
                                              YTV_TYPE_SHELL,
                                              G_SIGNAL_RUN_LAST,
                                              G_STRUCT_OFFSET (YtvShellClass,
                                                               error_raised),
                                              NULL, NULL,
                                              g_cclosure_marshal_VOID__POINTER,
                                              G_TYPE_NONE, 1, G_TYPE_POINTER);
        
        return;
}

static void
ytv_shell_init (YtvShell* self)
{
        g_object_set (G_OBJECT (self),
                      "tab-pos", GTK_POS_BOTTOM,
                      "show-tabs", FALSE, NULL);

        create_ui (self);
        
        return;
}

/**
 * ytv_shell_new:
 *
 * returns: (not-null): a new shell widget
 */
GtkWidget*
ytv_shell_new ()
{
        return GTK_WIDGET (g_object_new (YTV_TYPE_SHELL, NULL));
}

/**
 * ytv_shell_get_browser:
 * @self: a #YtvShell
 *
 * returns: (not-null) (caller-owns): The created #YtvBrowser in the #YtvShell
 */
YtvBrowser*
ytv_shell_get_browser (YtvShell* self)
{
        YtvShellPriv* priv;

        g_return_val_if_fail (YTV_IS_SHELL (self), NULL);
        
        priv = YTV_SHELL_GET_PRIVATE (self);

        return g_object_ref (priv->browser);
}


/**
 * ytv_shell_set_feed:
 * @self: a #YtvShell
 * @feed (not-null): a #YtvFeed
 *
 * Assign the @feed to the internal @YtvBrowser
 */
void
ytv_shell_set_feed (YtvShell* self, YtvFeed* feed)
{
        YtvShellPriv* priv;

        g_return_if_fail (YTV_IS_SHELL (self));
        g_return_if_fail (YTV_IS_FEED (feed));

        priv = YTV_SHELL_GET_PRIVATE (self);

        ytv_browser_set_feed (priv->browser, feed);

        return;
}
