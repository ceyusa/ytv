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

#include <ytv-entre-text-view.h>

#include <gtk/gtk.h>

#include <ytv-rank.h>

#include <ytv-entry.h>

typedef struct _YtvEntryTextViewPriv YtvEntryTextViewPriv
struct _YtvEntryTextViewPriv
{
        GtkTextTagTable* tagtable;
};

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
