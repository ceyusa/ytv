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

#include <ytv-entry-text-view.h>

#include <gtk/gtk.h>

#include <ytv-rank.h>
#include <ytv-star.h>

enum _YtvEntryTextViewProp
{
        PROP_0,
        PROP_ENTRY
};

typedef struct _YtvEntryTextViewPriv YtvEntryTextViewPriv;
struct _YtvEntryTextViewPriv
{
        GtkTextTagTable* tagtable;
        YtvEntry* entry;
        GtkWidget* rating;
};

#define FONTSIZE 10 * PANGO_SCALE

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

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "p",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "big",
                                          "scale", PANGO_SCALE_LARGE,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "b",
                                          "weight", PANGO_WEIGHT_BOLD,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "i",
                                          "style", PANGO_STYLE_ITALIC,
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "gray",
                                          "foreground", "darkgray",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "red",
                                          "foreground", "darkred",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "blue",
                                          "foreground", "blue",
                                          "size", FONTSIZE, NULL));
        gtk_text_tag_table_add (table, tag);
        g_object_unref (tag);

        tag = GTK_TEXT_TAG (g_object_new (GTK_TYPE_TEXT_TAG, "name", "link",
                                          "underline", PANGO_UNDERLINE_SINGLE,
                                          "size", FONTSIZE, NULL));
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
        
        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);
        buffer = gtk_text_buffer_new (priv->tagtable);
        gtk_text_view_set_buffer (GTK_TEXT_VIEW (self), buffer);

        /* remove current GtkTextBuffer */
        g_object_set (G_OBJECT (buffer), "text", "", NULL);
        gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);

        /* title */
        g_object_get (G_OBJECT (priv->entry), "title", &title, NULL);

        if (title != NULL)
        {
                gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,
                                                          (const gchar*) title,
                                                          -1, "b", NULL);
                gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,
                                                          "\n",
                                                          -1, "b", NULL);
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
                
                gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,
                                                          (const gchar*) dur,
                                                          -1, "p", NULL);
                gtk_text_buffer_insert (buffer, &iter, " ", -1);
                g_free (dur);
        }

        /* author */
        g_object_get (G_OBJECT (priv->entry), "author", &author, NULL);

        if (author != NULL)
        {
                gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,
                                                          (const gchar*) author,
                                                          -1, "red", "i", NULL);
                gtk_text_buffer_insert (buffer, &iter, "\n", -1);
                g_free (author);
        }

        /* views */
        g_object_get (G_OBJECT (priv->entry), "views", &views, NULL);

        {
                gchar* v = g_strdup_printf ("%d views", views);

                gtk_text_buffer_insert_with_tags_by_name (buffer, &iter,
                                                          (const gchar*) v,
                                                          -1, "gray", NULL);
                gtk_text_buffer_insert (buffer, &iter, "\n", -1);
                g_free (v);
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

        return;
}

static void
ytv_entry_text_view_init (YtvEntryTextView* self)
{
        YtvEntryTextViewPriv* priv;

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);

        priv->tagtable = get_tag_table ();
        priv->entry = NULL;
        /* priv->rating = GTK_WIDGET (g_object_new (YTV_TYPE_RANK, NULL)); */
        /* ytv_rank_new (0.0); */

        g_object_set (G_OBJECT (self),
                      "editable", FALSE, "cursor-visible", FALSE,
                      "can-focus", FALSE, "wrap-mode", GTK_WRAP_WORD,
                      "left-margin", 6, "right-margin", 6,
                      "pixels-above-lines", 6,  NULL);
        
        return;
}

GtkWidget*
ytv_entry_text_view_new (void)
{
        return GTK_WIDGET (g_object_new (YTV_TYPE_ENTRY_TEXT_VIEW, NULL));
}

void
ytv_entry_text_view_set_entry (YtvEntryTextView* self, YtvEntry* entry)
{
        YtvEntryTextViewPriv* priv;

        g_return_if_fail (entry != NULL);

        priv = YTV_ENTRY_TEXT_VIEW_GET_PRIVATE (self);

        if (priv->entry != NULL)
        {
                g_object_unref (priv->entry);
        }

        priv->entry = g_object_ref (entry);

        update_widget (self);

        g_object_notify (G_OBJECT (self), "entry");

        return;
}
