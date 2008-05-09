/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-soup-feed-fetch-strategy.h - An object which implements the strategy
 *                                  for HTTP request using libsoup
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
 * SECTION: ytv-json-feed-parse-strategy
 * @short_description: JSON format implementation for #YtvFeedParseStrategy
 *
 * Implementation of the #YtvFeedParseStrategy intefaces, parsing JSON format
 * feeds with the json-glib library.
 */

/**
 * YtvJsonFeedParseStrategy:
 *
 * Object that represent the feed parsing strategy in JSON format using
 * the json-glib library.
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <json-glib/json-glib.h>

#include <ytv-error.h>
#include <ytv-json-feed-parse-strategy.h>

typedef struct _YtvJsonFeedParseStrategyPriv YtvJsonFeedParseStrategyPriv;

struct _YtvJsonFeedParseStrategyPriv
{
	JsonParser* parser;
	JsonNode* root;
};

#define YTV_JSON_FEED_PARSE_STRATEGY_GET_PRIVATE(o) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), YTV_TYPE_JSON_FEED_PARSE_STRATEGY, YtvJsonFeedParseStrategyPriv))

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

/**
 * ytv_json_feed_parse_strategy_perform:
 * @self: a #YtvFeedParseStrategy implementation instance
 * @data: (null-ok): the string to parse
 * @length: the length of the string to parse
 * @err: the error to propagates if something goes wrong.
 *
 * Parse a JSON format feed and extract the entries available.
 *
 * returns: (null-ok) (caller-own): a #YtvList of #YtvEntry
 */
YtvList*
ytv_json_feed_parse_strategy_perform (YtvFeedParseStrategy* self,
				      const gchar* data, gssize length,
				      GError **err)
{
        g_assert (self != NULL);
        g_assert (YTV_IS_JSON_FEED_PARSE_STRATEGY (self));

        return YTV_JSON_FEED_PARSE_STRATEGY_GET_CLASS (self)->perform
                (self, data, length, err);
}

static YtvList*
ytv_json_feed_parse_strategy_perform_default (YtvFeedParseStrategy* self,
                                              const gchar* data, gssize length,
                                              GError **err)
{
        g_return_val_if_fail (err == NULL || *err == NULL, NULL);
        g_return_val_if_fail (data != NULL, NULL);
        g_return_val_if_fail (lenght != 0, NULL);

        GError* tmp_error;
        JsonParser* parser;

        parser = json_parser_new ();

        if (!json_parser_load_from_data (parser, data, length, &tmp_error))
        {
                if (tmp_error != NULL)
                {
                        g_propagate_error (err, tmp_error);
                        goto beach;
                }
        }

        JsonNode* root;

        root = json_parser_get_root (parser);
        
        traverse (root);

beach:            
        g_object_unref (parser);
        
        return NULL;
}

static void
ytv_feed_parse_strategy_init (YtvFeedParseStrategyIface* klass)
{
	klass->perform = ytv_json_feed_parse_strategy_perform;

	return;
}

G_DEFINE_TYPE_EXTENDED (YtvJsonFeedParseStrategy, ytv_json_feed_parse_strategy,
			G_TYPE_OBJECT, 0,
			G_IMPLEMENT_INTERFACE (YTV_TYPE_FEED_PARSE_STRATEGY,
					       ytv_feed_parse_strategy_init))

static void
ytv_json_feed_parse_strategy_class_init (YtvJsonFeedParseStrategyClass* klass)
{
        klass->perform = ytv_json_feed_parse_strategy_perform_default;
        
        return;
}

static void
ytv_json_feed_parse_strategy_init (YtvJsonFeedParseStrategy* self)
{
        return;
}

/**
 * ytv_json_feed_parse_strategy_new:
 *
 * Creates a new instance of the #YtvJsonFeedParseStrategy which
 * implements the #YtvFeedParseStrategy interface
 *
 * returns: (not-null): a new json-glib implementation of the
 * #YtvFeedParseStrategy interface
 */
YtvFeedParseStrategy*
ytv_json_feed_parse_strategy_new (void)
{
        YtvJsonFeedParseStrategy* self = g_object_new
                (YTV_TYPE_JSON_FEED_PARSE_STRATEGY, NULL);

        return YTV_FEED_PARSE_STRATEGY (self);
}
