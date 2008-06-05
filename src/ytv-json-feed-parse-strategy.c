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

#include <errno.h>
#include <stdlib.h>

#include <json-glib/json-glib.h>

#include <ytv-entry.h>
#include <ytv-error.h>
#include <ytv-json-feed-parse-strategy.h>
#include <ytv-simple-list.h>

typedef struct _YtvJsonFeedParseStrategyPriv YtvJsonFeedParseStrategyPriv;

struct _YtvJsonFeedParseStrategyPriv
{
	JsonParser* parser;
	JsonNode* root;
};

#define YTV_JSON_FEED_PARSE_STRATEGY_GET_PRIVATE(o) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), YTV_TYPE_JSON_FEED_PARSE_STRATEGY, YtvJsonFeedParseStrategyPriv))

#define JSON_BAIL(err, obj, msg)                                \
        if (obj == NULL) {                                      \
                g_set_error (err, YTV_PARSE_ERROR,              \
                             YTV_PARSE_ERROR_BAD_FORMAT, msg);  \
                goto beach;                                     \
        }

#define JSON_GET_OBJECT(obj, node)                                      \
        obj = json_node_get_object ((node));                            \
        JSON_BAIL (err, obj, "Could not find the " #node " element")

#define JSON_GET_NODE(obj,node)                                         \
        node = json_object_get_member (obj, #node);                     \
        JSON_BAIL (err, node, "Could not find the " #node " element");

#define MIMETYPE "application/json"

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

/* extracts the video's id */
static gchar*
get_id (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        gchar* retval = NULL;
        const gchar* id = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (node), "$t"
                        )
                );

        if (id != NULL && g_utf8_validate (id, -1, NULL))
        {
                gchar* pos = g_strrstr (id, "/");
                if (pos != NULL && ++pos != NULL)
                {
                        retval = g_strdup (pos);
                }
        }

        return retval;
}

/* extracts the authors from the entry */
static gchar*
get_authors (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_ARRAY, NULL);

        gchar* authors = NULL;
        JsonArray* arr = json_node_get_array (node);
        gint i; gint size = json_array_get_length (arr);
        for (i = 0; i < size; i++)
        {
                const gchar* author = json_node_get_string (
                        json_object_get_member (
                                json_node_get_object (
                                        json_object_get_member (
                                                json_node_get_object (
                                                        json_array_get_element (
                                                                arr, i
                                                                )
                                                        ),
                                                "name"
                                                )
                                        ),
                                "$t"
                                )
                        );

                if (author != NULL)
                {
                        if (g_utf8_validate (author, -1, NULL))
                        {
                                if (authors == NULL)
                                        authors = g_strdup (author);
                                else
                                {
                                        gchar* tmp = g_strconcat (authors,
                                                                  " ", author);
                                        g_free (authors);
                                        authors = tmp;
                                }

                        }
                }
        }

        return authors;
}

/* extracts the title from the entry */
static gchar*
get_title (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        gchar* retval = NULL;
        const gchar* title = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (node), "$t"
                        )
                );

        if (title != NULL && g_utf8_validate (title, -1, NULL))
        {
                retval = g_strdup (title);
        }

        return retval;
}

/* extracts the video's duration */
static gint
get_duration (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, -1);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, -1);

        JsonObject* obj = json_node_get_object (node);

        gint retval = -1;
        const gchar* duration = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (
                                json_object_get_member (obj, "yt$duration")
                                ),
                        "seconds"
                        )
                );

        if (duration != NULL && g_utf8_validate (duration, -1, NULL))
        {
                errno = 0;
                gchar* tail;
                retval = strtol (duration, &tail, 0);
                if (errno != 0 || tail == duration)
                        return -1;
        }

        return retval;
}

/* extracts the rating */
static gfloat
get_rating (JsonNode* node)
{
        /* this could be possible in most_recent feed */
        if (node == NULL)
        {
                return 1;
        }
        
        /* g_return_val_if_fail (node != NULL, 1);  */
        
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, -1);

        gfloat retval = -1;

        JsonObject* obj = json_node_get_object (node);
        const gchar* rating = json_node_get_string (
                        json_object_get_member (obj, "average")
                );

        if (rating != NULL && g_utf8_validate (rating, -1, NULL))
        {
                errno = 0;
                gchar* tail;
                retval = (float) strtod (rating, &tail);
                if (errno != 0 || rating == tail)
                        return -1;
        }

        return retval;
}

/* extracts the published date */
static gchar*
get_published (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        gchar* retval = NULL;
        const gchar* published = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (node), "$t"
                        )
                );

        if (published != NULL && g_utf8_validate (published, -1, NULL))
        {
                retval = g_strdup (published);
        }

        return retval;
}

/* extracts the number of views */
static gint
get_views (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, -1);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, -1);

        JsonObject* obj = json_node_get_object (node);

        gint retval = -1;
        const gchar* views = json_node_get_string (
                json_object_get_member (obj, "viewCount")
                );

        if (views != NULL && g_utf8_validate (views, -1, NULL))
        {
                errno = 0;
                gchar* tail;
                retval = strtol (views, &tail, 0);
                if (errno != 0 || tail == views)
                        return -1;
        }

        return retval;
}

/* extracts the entry's category */
static gchar*
get_category (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        gchar* retval = NULL;

        JsonObject* obj = json_node_get_object (node);
        const gchar* category = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (
                                json_array_get_element (
                                        json_node_get_array (
                                                json_object_get_member
                                                (obj, "media$category")
                                                ),
                                        0
                                        )
                                ),
                        "$t"
                        )
                );

        if (category != NULL && g_utf8_validate (category, -1, NULL))
        {
                retval = g_strdup (category);
        }

        return retval;
}

/* extracts the entry's tags */
static gchar*
get_tags (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        gchar* retval = NULL;

        JsonObject* obj = json_node_get_object (node);

        /* only the first element */
        const gchar* tags = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (
                                json_object_get_member (obj, "media$keywords")
                                ),
                        "$t"
                        )
                );

        if (tags != NULL && g_utf8_validate (tags, -1, NULL))
        {
                retval = g_strdup (tags);
        }

        return retval;
}

/* extracts the video's description */
static gchar*
get_description (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        gchar* retval = NULL;

        JsonObject* obj = json_node_get_object (node);

        /* only the first element */
        const gchar* description = json_node_get_string (
                json_object_get_member (
                        json_node_get_object (
                                json_object_get_member (obj,
                                                        "media$description")
                                ),
                        "$t"
                        )
                );

        if (description != NULL && g_utf8_validate (description, -1, NULL))
        {
                retval = g_strdup (description);
        }

        return retval;
}

static YtvEntry*
parse_entry (JsonNode* node)
{
        g_return_val_if_fail (node != NULL, NULL);
        g_return_val_if_fail (JSON_NODE_TYPE (node) == JSON_NODE_OBJECT, NULL);

        YtvEntry* entry = NULL;
        JsonObject* obj = json_node_get_object (node);

        gchar* id = get_id (
                json_object_get_member (obj, "id")
                );

        gchar* authors = get_authors (
                json_object_get_member (obj, "author")
                );

        gchar* title = get_title (
                json_object_get_member (obj, "title")
                );

        gint duration = get_duration (
                json_object_get_member (obj, "media$group")
                );

        gfloat rating = get_rating (
                json_object_get_member (obj, "gd$rating")
                );

        gchar* published = get_published (
                json_object_get_member (obj, "published")
                );

        gint views = get_views (
                json_object_get_member (obj, "yt$statistics")
                );

        gchar* category = get_category (
                json_object_get_member (obj,"media$group")
                );

        gchar* tags = get_tags (
                json_object_get_member (obj,"media$group")
                );

        gchar* description = get_description (
                json_object_get_member (obj,"media$group")
                );

        if (id != NULL && authors != NULL && title != NULL && duration > 0 &&
            rating > -1 && published != NULL && views > 0 &&
            category != NULL && tags != NULL && description != NULL)
        {
                entry = g_object_new (YTV_TYPE_ENTRY,
                                      "id", id, "author", authors,
                                      "title", title, "duration", duration,
                                      "rating", rating, "published", published,
                                      "views", views, "category", category,
                                      "tags", tags, "description", description,
                                      NULL);
        }

        if (id != NULL)
                g_free (id);

        if (authors != NULL)
                g_free (authors);

        if (title != NULL)
                g_free (title);

        if (published != NULL)
                g_free (published);

        if (category != NULL)
                g_free (category);

        if (tags != NULL)
                g_free (tags);

        if (description != NULL)
                g_free (description);

        return entry;
}

static YtvList*
ytv_json_feed_parse_strategy_perform_default (YtvFeedParseStrategy* self,
                                              const guchar* data, gssize length,
                                              GError **err)
{
        g_return_val_if_fail (err == NULL || *err == NULL, NULL);
        g_return_val_if_fail (data != NULL, NULL);
        g_return_val_if_fail (length != 0, NULL);

        GError* tmp_error;
        JsonParser* parser;
        YtvList* fl = NULL;

        parser = json_parser_new ();

        if (!json_parser_load_from_data (parser, data, length, &tmp_error))
        {
                if (tmp_error != NULL)
                {
                        g_propagate_error (err, tmp_error);
                        goto beach;
                }
        }

        JsonNode* root; JsonObject* object_root;
        root = json_parser_get_root (parser);
        JSON_BAIL (err, root, "Could not find the root element");
        JSON_GET_OBJECT (object_root, root);

        JsonNode* feed; JsonObject* object_feed;
        JSON_GET_NODE (object_root, feed);
        JSON_GET_OBJECT (object_feed, feed);

        JsonNode* entry;
        JSON_GET_NODE (object_feed, entry);

        JsonArray* entries_arr = json_node_get_array (entry);
        JSON_BAIL (err, entries_arr, "Could not find the entry array");

        fl = ytv_simple_list_new (); /* feed list */
        gint i; gint size = json_array_get_length (entries_arr);
        for (i = 0; i < size; i++)
        {
                entry = json_array_get_element (entries_arr, i);
                if (entry != NULL)
                {
                        /* traverse (entry); */
                        YtvEntry* e = parse_entry (entry);
                        if (e != NULL)
                        {
                                ytv_list_append (fl, e);
                                g_object_unref (e); /* we don't want the ref */
                        }
                }
        }

        g_debug ("number of entries = %d", ytv_list_get_length (fl));

beach:
        g_object_unref (parser);

        return fl;
}

static const gchar*
ytv_json_feed_parse_strategy_get_mime_default (YtvFeedParseStrategy* self)
{
        return MIMETYPE;
}

static void
ytv_feed_parse_strategy_init (YtvFeedParseStrategyIface* klass)
{
	klass->perform = ytv_json_feed_parse_strategy_perform;
        klass->get_mime = ytv_json_feed_parse_strategy_get_mime;

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
        klass->get_mime = ytv_json_feed_parse_strategy_get_mime_default;

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
				      const guchar* data, gssize length,
				      GError **err)
{
        g_assert (self != NULL);
        g_assert (YTV_IS_JSON_FEED_PARSE_STRATEGY (self));

        return YTV_JSON_FEED_PARSE_STRATEGY_GET_CLASS (self)->perform
                (self, data, length, err);
}


/**
 * ytv_json_feed_parse_strategy_get_mime:
 * @self: a #YtvFeedParseStrategy implementation instance
 *
 * Retrieves the MIME type that this parser can handle: application/json
 *
 * returns: (not-null): a string with the MIME type. Do not modify the internal
 * string.
 */
const gchar*
ytv_json_feed_parse_strategy_get_mime (YtvFeedParseStrategy* self)
{
        g_assert (self != NULL);
        g_assert (YTV_IS_JSON_FEED_PARSE_STRATEGY (self));

        return YTV_JSON_FEED_PARSE_STRATEGY_GET_CLASS (self)->get_mime (self);
}
