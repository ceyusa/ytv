/* json-parse.c - proof of concept for a JSON parser for the YT responses
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

#include <glib.h>
#include <json-glib/json-glib.h>
#include <stdlib.h>

#define do_indent(i) { gint z; for (z = 0; z < i; z++) g_print (" ");  }

static void
traverse (JsonNode* node)
{
	g_assert (node != NULL);
	static gint indent = -1;

	indent++;
	
	switch (JSON_NODE_TYPE(node))
	{
	case JSON_NODE_OBJECT:
	{
		g_print ("\n");
		do_indent (indent);
		g_print ("{\n");
		indent++;
		JsonObject* object = json_node_get_object (node);
		GList* members = json_object_get_members (object);
		GList* tmp = NULL; JsonNode* nextnode = NULL;
		for (tmp = members; tmp; tmp = tmp->next)
		{
			do_indent (indent);
			g_print ("%s: ", (gchar*) tmp->data);
			nextnode = json_object_get_member (object,
							   (gchar*) tmp->data);
			traverse (nextnode);
		}
		g_list_free (members);
		indent--;
		do_indent (indent);
		g_print ("}\n");
	}
		break;
	case JSON_NODE_ARRAY:
	{
		g_print ("\n");
		do_indent (indent);
		g_print ("[\n");
		indent++;
		JsonArray* arr = json_node_get_array (node);
		gint size = json_array_get_length (arr);
		gint i; JsonNode* nextnode;
		for (i = 0; i < size; i++)
		{
			nextnode = json_array_get_element (arr, i);
			traverse (nextnode);
		}
		indent--;
		do_indent (indent);
		g_print ("]\n");
	}
		break;
	case JSON_NODE_VALUE:
	{
		GType gtype = json_node_get_value_type (node);
		if (gtype == G_TYPE_STRING)
		{
			g_print ("%s\n", json_node_get_string (node));
		}
		else
		{
			g_print ("other type\n");
		}
	}
		break;
	case JSON_NODE_NULL:
		g_print ("= null\n");
		break;
	default:
		g_assert_not_reached ();
	}

	indent--;
	
	return;
}

static void
parse (gchar *filename)
{
	JsonParser* parser;
	GError* error;
		
	parser = json_parser_new ();

	if (!json_parser_load_from_file (parser, filename, &error))
	{
		g_print ("Error: %s\n", error->message);
		g_error_free (error);
		g_object_unref (parser);
		exit (1);
	}

	JsonNode* root;

	root = json_parser_get_root (parser);

	g_print ("root");
	traverse (root);

	return;
}

gint
main (gint argc, gchar **argv)
{
	if (argc != 2)
	{
		g_print ("json file argument missing");
		exit (1);
	}
	
	g_type_init ();
	parse (argv[1]);

	return 0;
}
