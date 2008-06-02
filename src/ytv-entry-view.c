/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-entry-view.c - A type that defines a GUI entry view
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

/**
 * SECTION: ytv-entry-view
 * @title: YtvEntryView
 * @short_description: A type that defines an user interface view for a 
 * #YtvEntry
 *
 * An abstract type that defines an user interface view for a #YtvEntry. The
 * implementators are in charge to choose the graphical toolkit
 */

/**
 * YtvEntryView:
 *
 * A ui view for #YtvEntry
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-entry-view.h>
#include <ytv-entry.h>

/**
 * ytv_entry_view_set_entry:
 * @self: a #YtvEntryView
 * @entry: a #YtvEntry
 *
 * Set the entry wich view @self must display.
 */
void
ytv_entry_view_set_entry (YtvEntreView* self, YtvEntry* entry)
{
        if (YTV_ENTRY_VIEW_GET_IFACE (self)->set_entry == NULL)
        {
                g_critical ("You must implement ytv_entry_view_set_view\n");
        }

        YTV_ENTRY_VIEW_GET_IFACE (self)->set_entry (self, entry);

        return;
}

static void
ytv_entry_view_base_init (gpointer g_class)
{
        static gboolean initializated = FALSE;

        if (!initializated)
        {
                initializated = TRUE;
        }

        return;
}

static gpointer
ytv_entry_view_register_type (gpointer notused)
{
        GType type = 0;

        static const GTypeInfo info =
        {
                sizeof (YtvEntryViewIface),
                ytv_entry_view_base_init,
                NULL,
                NULL,
                NULL,
                NULL,
                0,
                0,
                NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "YtvEntryView", &info, 0);

        return GUINT_TO_POINTER (type);
}

GType
ytv_entry_view_get_type (void)
{
        static GOnce once = G_ONCE_INIT;
        g_once (&once, ytv_entry_view_register_type, NULL);
        return GPOINTER_TO_UINT (once.retval);
}
