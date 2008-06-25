/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-entry-text-view.c - A type that defines a GTK+ GUI entry view
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

#include <monetary.h>

#include <ytv-entry-text-view.h>

#include <gtk/gtk.h>

#include <ytv-star.h>
#include <ytv-marshal.h>

enum _YtvEntryTextViewProp
{
        PROP_0,
        PROP_ENTRY
};

enum _YtvEntryTextViewSignals
{
        LINK_CLICKED,
        LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

typedef struct _YtvEntryTextViewPriv YtvEntryTextViewPriv;
struct _YtvEntryTextViewPriv
{
        GtkTextTagTable* tagtable;
        YtvEntry* entry;
        GdkCursor* handcursor;
        GdkCursor* dfltcursor;
        gboolean hovering_over_link;
};

#define FONTSIZE_LARGE  10 * PANGO_SCALE
#define FONTSIZE_MEDIUM  8 * PANGO_SCALE
#define FONTSIZE_SMALL   7 * PANGO_SCALE

#define YTV_ENTRY_TEXT_VIEW_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_ENTRY_TEXT_VIEW, YtvEntryTextViewPriv))

G_DEFINE_TYPE (YtvEntryTextView, ytv_entry_text_view, GTK_TYPE_TEXT_VIEW)

/* Create a GtkTextTagTable which will be shared by all the TextViews */
static GtkTextTagTable*
create_tag_table (void)
{
        GtkTextTagTable* table;
        GtkTextTag* tag;

        table = gtk_text_tag_table_new ();

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG,
                                          "name", "duration",
                                          "size", FONTSIZE_SMALL, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

/*         tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "big", */
/*                                           "scale", PANGO_SCALE_LARGE, */
/*                                           "size", FONTSIZE_MEDIUM, NULL)); */
/*         gtk_text_tag_table_add (table, tag); */
/*         g_object_unref (tag); */

/*         tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "b", */
/*                                           "weight", PANGO_WEIGHT_BOLD, */
/*                                           "size", FONTSIZE, NULL)); */
        
        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG,
                                          "name", "title",
                                          "size", FONTSIZE_LARGE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG,
                                          "name", "author",
                                          "style", PANGO_STYLE_ITALIC,
                                          "foreground", "darkred",
                                          "size", FONTSIZE_MEDIUM, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG,
                                          "name", "views",
                                          "foreground", "darkgray",
                                          "size", FONTSIZE_MEDIUM, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG,
                                          "name", "category",
                                          "foreground", "blue",
                                          "size", FONTSIZE_MEDIUM, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);
        
        return table;
}

static GtkTextTagTable*
get_tag_table (void)
{
        static GtkTextTagTable* table = NULL;

        if (table == NULL)
        {
                table = create_tag_table ();
        }
        else
        {
                g_object_ref (G_OBJECT (table));
        }

        return table;
}

static GdkCursor*
get_cursor (GdkCursorType type)
{
        static GdkCursor* hand = NULL;
        static GdkCursor* dftl = NULL;
        GdkCursor* cursor;

        if (type == GDK_HAND2)
        {
                cursor = hand;
        }
        else if (type == GDK_XTERM)
        {
                cursor = dftl;
        }
        else
        {
                return NULL;
        }
               
        if (cursor == NULL)
        {
                cursor = gdk_cursor_new (type);
        }
        else
        {
                gdk_cursor_ref (cursor);
        }

        return cursor;
}       

/* Looks at all tags covering the position (x, y) in the text view,
 * and if one of them is a link, change the cursor to the "hands" cursor
 * typically used by web browsers.
 */
static void
set_cursor_if_appropriate (YtvEntryTextView* self, gint x, gint y)
{
        YtvEntryTextViewPriv* priv;
        GSList *tags = NULL, *tagp = NULL;
        GtkTextBuffer* buffer;
        GtkTextIter iter;
        GtkTextView* text_view;
        gboolean hovering = FALSE;

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);
        
        text_view = GTK_TEXT_VIEW (self);
        buffer = gtk_text_view_get_buffer (text_view);
        gtk_text_view_get_iter_at_location (text_view, &iter, x, y);

        tags = gtk_text_iter_get_tags (&iter);
        for (tagp = tags;  tagp != NULL;  tagp = tagp->next)
        {
                hovering = (g_object_get_data (G_OBJECT (tagp->data), "class")
                            != NULL);

                if (hovering == TRUE)
                {
                        break;
                }
        }

        if (hovering != priv->hovering_over_link)
        {
                priv->hovering_over_link = hovering;

                if (priv->hovering_over_link == TRUE)
                {
                        gdk_window_set_cursor
                                (gtk_text_view_get_window
                                 (text_view, GTK_TEXT_WINDOW_TEXT),
                                 priv->handcursor);
                }
                else
                {
                        gdk_window_set_cursor
                                (gtk_text_view_get_window
                                 (text_view, GTK_TEXT_WINDOW_TEXT),
                                 priv->dfltcursor);
                }
        }

        if (tags != NULL)
        {
                g_slist_free (tags);
        }

        return;
}

/* Looks at all tags covering the position of iter in the text view,
 * and if one of them is a link, follow it by showing the page identified
 * by the data attached to it.
 */
static void
follow_if_link (YtvEntryTextView* self, GtkTextIter* iter)
{
        YtvEntryTextViewPriv* priv;
        GSList *tags = NULL, *tagp = NULL;

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);
        tags = gtk_text_iter_get_tags (iter);
        for (tagp = tags;  tagp != NULL;  tagp = tagp->next)
        {
                gchar* class = g_object_get_data (G_OBJECT (tagp->data),
                                                  "class");

                if (class != NULL)
                {
                        gchar* param = g_object_get_data (G_OBJECT (tagp->data),
                                                          "param");
                        g_signal_emit (self, signals[LINK_CLICKED],
                                       0, class, param);
                        break;
                }
        }

        if (tags)
        {
                g_slist_free (tags);
        }

        return;
}

static gboolean 
motion_notify_event (GtkWidget* view, GdkEventMotion* event)
{
        gint x, y;

        gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (view),
                                               GTK_TEXT_WINDOW_WIDGET,
                                               event->x, event->y, &x, &y);

        set_cursor_if_appropriate (YTV_ENTRY_TEXT_VIEW (view), x, y);

        gdk_window_get_pointer (view->window, NULL, NULL, NULL);
        
        return FALSE;
}

static gboolean
event_after (GtkWidget* view, GdkEvent* ev)
{
        GtkTextIter start, end, iter;
        GtkTextBuffer *buffer;
        GdkEventButton *event;
        gint x, y;

        if (ev->type != GDK_BUTTON_RELEASE)
        {
                return FALSE;
        }

        event = (GdkEventButton*) ev;

        if (event->button != 1)
        {
                return FALSE;
        }

        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

        /* we shouldn't follow a link if the user has selected something */
        gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
        
        if (gtk_text_iter_get_offset (&start) !=
            gtk_text_iter_get_offset (&end))
        {
                return FALSE;
        }

        gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (view),
                                               GTK_TEXT_WINDOW_WIDGET,
                                               event->x, event->y, &x, &y);

        gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (view), &iter, x, y);

        follow_if_link (YTV_ENTRY_TEXT_VIEW (view), &iter);

        return FALSE;
}

static GtkWidget*
get_rating_widget (gfloat rating)
{
        gint i;
        gfloat restrank;
        GtkWidget* retval;
        GdkColor white;
        
        restrank = rating;
        
        retval = gtk_hbox_new (FALSE, 0);

        if (gdk_color_parse ("white", &white))
        {
                gtk_widget_modify_bg (retval, GTK_STATE_NORMAL, &white);
        }
        
        for (i = 0; i < 5; i++)
        {
                GtkWidget* star;
                
                if (restrank >= 1.0)
                {
                        star = ytv_star_new (1.0);
                        restrank -= 1;
                }
                else if (restrank > 0.0)
                {
                        star = ytv_star_new (restrank);
                        restrank = 0.0;
                }
                else if (restrank == 0.0)
                {
                        star = ytv_star_new (0.0);
                }
                else
                {
                        g_assert_not_reached ();
                }
                
                gtk_widget_set_size_request (star, 18, 18);
                gtk_widget_modify_bg (star, GTK_STATE_NORMAL, &white);
                gtk_box_pack_start (GTK_BOX (retval), star, FALSE, FALSE, 0);
        }

        return retval;
}

static void
insert_link (GtkTextBuffer* buffer, GtkTextIter* iter, const gchar* text,
             const gchar* class, const gchar* param, const gchar* tagname)
{
        GtkTextTag* linktag;
        GtkTextTag* extratag = NULL;
        GtkTextTagTable* tagtable = gtk_text_buffer_get_tag_table (buffer);

        if (text == NULL)
        {
                return;
        }
        
        if (tagname != NULL)
        {
                extratag = gtk_text_tag_table_lookup (tagtable, tagname);
        }

        linktag = gtk_text_buffer_create_tag
                (buffer, NULL, "underline", PANGO_UNDERLINE_SINGLE, NULL);

        if (class != NULL)
        {
                g_object_set_data_full (G_OBJECT (linktag),
                                        "class", g_strdup (class), g_free);
        }

        if (param != NULL)
        {
                g_object_set_data_full (G_OBJECT (linktag),
                                        "param", g_strdup (param), g_free);
        }
        
        gtk_text_buffer_insert_with_tags (buffer, iter, text, -1,
                                          extratag, linktag, NULL);

        return;
}

static void
update_widget (YtvEntryTextView* self)
{
        YtvEntryTextViewPriv* priv;
        GtkTextBuffer* buffer;
        GtkTextIter iter;
        GtkTextChildAnchor* anchor;
        GtkWidget* rank;

        gchar* title;
        gint duration;
        gchar* author;
        gint views;
        gfloat rating;
        gchar* category;
        gchar* id;
        
        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);
        buffer = gtk_text_buffer_new (priv->tagtable);
        gtk_text_view_set_buffer (GTK_TEXT_VIEW (self), buffer);

        /* remove current GtkTextBuffer */
        g_object_set (G_OBJECT (buffer), "text", "", NULL);
        gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);

        /* id */
        g_object_get (G_OBJECT (priv->entry), "id", &id, NULL);
        
        /* title */
        g_object_get (G_OBJECT (priv->entry), "title", &title, NULL);

        if (title != NULL)
        {
                insert_link (buffer, &iter, title, "info", id, "title");
                gtk_text_buffer_insert (buffer, &iter, "\n", -1);
                g_free (title);
        }

        /* duration */
        g_object_get (G_OBJECT (priv->entry), "duration", &duration, NULL);

        {
                gchar* dur;

                gint sec = duration % 60;
                gint min = (duration / 60) % 60;
                gint hou = duration / 1200;

                dur = g_strdup_printf ("%02d:%02d", min, sec);

                if (hou > 0)
                {
                        gchar* tmp = dur;
                        dur = g_strdup_printf ("%02d:%s", hou, tmp);
                        g_free (tmp);
                }
                
                gtk_text_buffer_insert_with_tags_by_name
                        (buffer, &iter, dur, -1, "duration", NULL);
                gtk_text_buffer_insert (buffer, &iter, " ", -1);
                g_free (dur);
        }

        /* author */
        g_object_get (G_OBJECT (priv->entry), "author", &author, NULL);

        if (author != NULL)
        {
                insert_link (buffer, &iter, author,
                             "author", author, "author");
                gtk_text_buffer_insert (buffer, &iter, "\n", -1);

                g_free (author);
        }

        /* views */
        g_object_get (G_OBJECT (priv->entry), "views", &views, NULL);

        {
                gchar nv[BUFSIZ];
                gchar* v;

                strfmon (nv, BUFSIZ - 1, "%!.0n", (gdouble) views);
                v = g_strdup_printf ("%s views", nv);

                gtk_text_buffer_insert_with_tags_by_name
                        (buffer, &iter, v, -1, "views", NULL);
                gtk_text_buffer_insert (buffer, &iter, " ", -1);
                g_free (v);
        }

        /* category */
        g_object_get (G_OBJECT (priv->entry), "category", &category, NULL);

        if (category != NULL)
        {
                insert_link (buffer, &iter, category,
                             "category", category, "category");
                gtk_text_buffer_insert (buffer, &iter, "\n", -1);
                g_free (category);                
        }

        /* rating */
        g_object_get (G_OBJECT (priv->entry), "rating", &rating, NULL);

        {
                anchor = gtk_text_buffer_create_child_anchor (buffer, &iter);
                
                if (rating < 0)
                {
                        rating = 0;
                }

                rank = get_rating_widget (rating);
                
                gtk_text_view_add_child_at_anchor (GTK_TEXT_VIEW (self),
                                                   rank, anchor);

                gtk_widget_show_all (rank);
                
        }

        g_free (id);
        g_object_unref (buffer);

        return;
}

static void
ytv_entry_text_view_set_property (GObject* object, guint prop_id,
                                  const GValue* value, GParamSpec* spec)
{
        switch (prop_id)
        {
        case PROP_ENTRY:
                ytv_entry_text_view_set_entry (YTV_ENTRY_TEXT_VIEW (object),
                                               g_value_get_object (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_entry_text_view_get_property (GObject* object, guint prop_id,
                                  GValue* value, GParamSpec* spec)
{
        YtvEntryTextViewPriv* priv;

        switch (prop_id)
        {
        case PROP_ENTRY:
                priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (object);
                g_value_set_object (value, priv->entry);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_entry_text_view_dispose (GObject* object)
{
        YtvEntryTextViewPriv* priv;

        (*G_OBJECT_CLASS (ytv_entry_text_view_parent_class)->dispose) (object);

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (object);

        if (priv->tagtable != NULL)
        {
                g_object_unref (priv->tagtable);
                priv->tagtable = NULL;
        }

        if (priv->entry != NULL)
        {
                g_object_unref (priv->entry);
                priv->entry = NULL;
        }

        if (priv->handcursor != NULL)
        {
                gdk_cursor_unref (priv->handcursor);
                priv->handcursor = NULL;
        }

        if (priv->dfltcursor != NULL)
        {
                gdk_cursor_unref (priv->dfltcursor);
                priv->dfltcursor = NULL;
        }

        return;
}

static void
ytv_entry_text_view_class_init (YtvEntryTextViewClass* klass)
{
        GObjectClass* object_class;

        object_class = G_OBJECT_CLASS (klass);

        g_type_class_add_private (object_class, sizeof (YtvEntryTextViewPriv));

        object_class->get_property = ytv_entry_text_view_get_property;
        object_class->set_property = ytv_entry_text_view_set_property;
        object_class->dispose      = ytv_entry_text_view_dispose;

        g_object_class_install_property
                (object_class, PROP_ENTRY,
                 g_param_spec_object
                 ("entry", "Entry", "The feed's entry to show",
                  YTV_TYPE_ENTRY, G_PARAM_READWRITE));

        signals[LINK_CLICKED] =
                g_signal_new ("link-clicked",
                              YTV_TYPE_ENTRY_TEXT_VIEW,
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (YtvEntryTextViewClass,
                                               link_clicked),
                              NULL, NULL,
                              ytv_cclosure_marshal_VOID__STRING_STRING,
                              G_TYPE_NONE, 2,
                              G_TYPE_STRING, G_TYPE_STRING);
        
        return;
}

static void
ytv_entry_text_view_init (YtvEntryTextView* self)
{
        YtvEntryTextViewPriv* priv;

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);

        priv->tagtable   = get_tag_table ();
        priv->entry      = NULL;
        priv->handcursor = get_cursor (GDK_HAND2);
        priv->dfltcursor = get_cursor (GDK_XTERM);
        priv->hovering_over_link = FALSE;

        g_object_set (G_OBJECT (self),
                      "editable", FALSE, "cursor-visible", FALSE,
                      "can-focus", FALSE, "wrap-mode", GTK_WRAP_WORD,
                      "left-margin", 6, "right-margin", 6,
                      "pixels-above-lines", 6,  NULL);

        g_signal_connect (G_OBJECT (self), "motion-notify-event",
                          G_CALLBACK (motion_notify_event), NULL);

        g_signal_connect (G_OBJECT (self), "event-after",
                          G_CALLBACK (event_after), NULL);
        
        return;
}

/**
 * ytv_entry_text_view_new:
 *
 * Create a new widget for show entry data
 */
GtkWidget*
ytv_entry_text_view_new (void)
{
        return GTK_WIDGET (g_object_new (YTV_TYPE_ENTRY_TEXT_VIEW, NULL));
}

/**
 * ytv_entry_text_view_set_entry:
 * @self: A #YtvEntryTextView
 * @entry: A #YtvEntry
 *
 * Set the entry to display in the widget
 */
void
ytv_entry_text_view_set_entry (YtvEntryTextView* self, YtvEntry* entry)
{
        YtvEntryTextViewPriv* priv;

        g_return_if_fail (entry != NULL);

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);

        if (priv->entry != NULL)
        {
                g_object_unref (priv->entry);
                priv->entry = NULL;
        }

        priv->entry = g_object_ref (entry);

        update_widget (self);

        g_object_notify (G_OBJECT (self), "entry");

        return;
}

/**
 * ytv_entry_text_view_clean:
 * @self: a #YtvEntryTextView
 *
 * Clean the widget data.
 */
void
ytv_entry_text_view_clean (YtvEntryTextView* self)
{
        YtvEntryTextViewPriv* priv;
        GtkTextBuffer* buffer;

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);

        if (priv->entry != NULL)
        {
                g_object_unref (priv->entry);
                priv->entry = NULL;
        }

        buffer = gtk_text_buffer_new (priv->tagtable);
        g_object_set (G_OBJECT (buffer), "text", "", NULL);
        gtk_text_view_set_buffer (GTK_TEXT_VIEW (self), buffer);

        return;
}
