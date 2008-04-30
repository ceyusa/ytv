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

static void
create_entry ()
{
/* 	GObject* object = g_object_new (G_TYPE_OBJECT, NULL); */
/* 	g_object_unref (object); */
	
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

	g_object_unref (entry);
	return;
}

gint
main (gint argc, gchar** argv)
{
	g_type_init ();

	create_entry ();

	return;
}	
