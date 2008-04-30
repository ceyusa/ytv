/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-entry.c - Describes a youtube video entry
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
 * SECTION:ytv-entry
 * @short_description: Object which describes a youtube video entry.
 *
 * The #YtvEntry is a data transport object which contains the necessary
 * information to present and play a video in YouTube.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-entry.h>

enum _YtvEntryProperties
{
	PROP_0,
	PROP_ID,
	PROP_AUTHOR,
	PROP_TITLE,
	PROP_DURATION,
	PROP_RATING,
	PROP_PUBLISHED,
	PROP_VIEWS,
	PROP_CATEGORY,
	PROP_TAGS,
	PROP_DESCRIPTION
};

typedef struct _YtvEntryPriv YtvEntryPriv;
struct _YtvEntryPriv
{
	gchar* id;
	gchar* author;
	gchar* title;
	gint duration;
	gfloat rating;
	gchar* published;
	gint views;
	gchar* category;
	gchar* tags;
	gchar* description;
};

#define YTV_ENTRY_GET_PRIVATE(obj)					\
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_ENTRY, YtvEntryPriv))

G_DEFINE_TYPE (YtvEntry, ytv_entry, G_TYPE_OBJECT)

static void
ytv_entry_init (YtvEntry* self)
{
	YtvEntryPriv* priv = YTV_ENTRY_GET_PRIVATE (self);

	priv->id          = NULL;
	priv->author      = NULL;
	priv->title       = NULL;
	priv->duration    = -1;
	priv->rating      = -1;
	priv->published   = NULL;
	priv->views       = -1;
	priv->category    = NULL;
	priv->tags        = NULL;
	priv->description = NULL;

	return;
}

static void
ytv_entry_set_property (GObject* object, guint prop_id,
			const GValue* value, GParamSpec* spec)
{
	g_return_if_fail (YTV_IS_ENTRY (object));

	YtvEntryPriv* priv = YTV_ENTRY_GET_PRIVATE (object);

	switch (prop_id)
	{
	case PROP_ID:
                g_return_if_fail (priv->id == NULL);
                const gchar* id = g_value_get_string (value);
                g_return_if_fail (id != NULL);
                priv->id = g_strdup (id);
                break;
	case PROP_AUTHOR:
		g_return_if_fail (priv->author == NULL);
		const gchar* author = g_value_get_string (value);
		g_return_if_fail (author != NULL);
		priv->author = g_strdup (author);
		break;
	case PROP_TITLE:
		g_return_if_fail (priv->title == NULL);
		const gchar* title = g_value_get_string (value);
		g_return_if_fail (title != NULL);
		priv->title = g_strdup (title);
		break;
	case PROP_DURATION:
		g_return_if_fail (priv->duration == -1);
		gint duration = g_value_get_int (value);
		g_return_if_fail (duration > 0);
		priv->duration = duration;
		break;
	case PROP_RATING:
		g_return_if_fail (priv->rating == -1);
		gint rating = g_value_get_float (value);
		g_return_if_fail (rating >= 1);
		priv->rating = rating;
		break;
	case PROP_CATEGORY:
		g_return_if_fail (priv->category == NULL);
		const gchar* category = g_value_get_string (value);
		g_return_if_fail (category != NULL);
		priv->category = g_strdup (category);
		break;
	case PROP_PUBLISHED:
		g_return_if_fail (priv->published == NULL);
		const gchar* published = g_value_get_string (value);
		g_return_if_fail (published != NULL);
		priv->published = g_strdup (published);
	case PROP_VIEWS:
		g_return_if_fail (priv->views == -1);
		gint views = g_value_get_int (value);
		g_return_if_fail (views >= 0);
		priv->views = views;
		break;
	case PROP_TAGS:
		g_return_if_fail (priv->tags == NULL);
		const gchar* tags = g_value_get_string (value);
		g_return_if_fail (tags != NULL);
		priv->tags = g_strdup (tags);
		break;
	case PROP_DESCRIPTION:
		g_return_if_fail (priv->description == NULL);
		const gchar* description = g_value_get_string (value);
		g_return_if_fail (description != NULL);
		priv->description = g_strdup (description);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
		break;
	}

	return;
}

static void
ytv_entry_get_property (GObject* object, guint prop_id,
			GValue* value, GParamSpec* spec)
{
	g_return_if_fail (YTV_IS_ENTRY (object));

	YtvEntryPriv* priv = YTV_ENTRY_GET_PRIVATE (object);

	switch (prop_id)
	{
	case PROP_ID:
		g_value_set_string (value, priv->id);
		break;
	case PROP_AUTHOR:
		g_value_set_string (value, priv->author);
		break;
	case PROP_TITLE:
		g_value_set_string (value, priv->title);
		break;
	case PROP_DURATION:
		g_value_set_int (value, priv->duration);
		break;
	case PROP_RATING:
		g_value_set_float (value, priv->rating);
		break;
	case PROP_CATEGORY:
		g_value_set_string (value, priv->category);
		break;
	case PROP_VIEWS:
		g_value_set_int (value, priv->views);
		break;
	case PROP_TAGS:
		g_value_set_string (value, priv->tags);
		break;
	case PROP_DESCRIPTION:
		g_value_set_string (value, priv->description);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
		break;
	}

	return;
}

static void
ytv_entry_finalize (GObject* object)
{
	g_return_if_fail (YTV_IS_ENTRY (object));

	YtvEntryPriv* priv = YTV_ENTRY_GET_PRIVATE (object);

	if (priv->id != NULL)
	{
		g_free (priv->id);
		priv->id = NULL;
	}

	if (priv->author != NULL)
	{
		g_free (priv->author);
		priv->author = NULL;
	}

	if (priv->title != NULL)
	{
		g_free (priv->title);
		priv->title = NULL;
	}

	if (priv->published != NULL)
	{
		g_free (priv->published);
		priv->published = NULL;
	}

	if (priv->category != NULL)
	{
		g_free (priv->category);
		priv->category = NULL;
	}

	if (priv->tags != NULL)
	{
		g_free (priv->tags);
		priv->tags = NULL;
	}

	if (priv->description != NULL)
	{
		g_free (priv->description);
		priv->description = NULL;
	}

	(*G_OBJECT_CLASS (ytv_entry_parent_class)->finalize) (object);
	
	return;
}

static void
ytv_entry_class_init (YtvEntryClass* klass)
{
	GObjectClass* g_klass = G_OBJECT_CLASS (klass);

	g_type_class_add_private (g_klass, sizeof (YtvEntryPriv));
	g_klass->set_property = ytv_entry_set_property;
	g_klass->get_property = ytv_entry_get_property;
	g_klass->finalize = ytv_entry_finalize;

	g_object_class_install_property
		(g_klass, PROP_ID,
		 g_param_spec_string
		 ("id", "vid", "Video identificator string", NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_AUTHOR,
		 g_param_spec_string
		 ("author", "author", "Username of the video's owner", NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_TITLE,
		 g_param_spec_string
		 ("title", "title", "Video's title", NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_DURATION,
		 g_param_spec_int
		 ("duration", "duration", "Length of the video in seconds",
		  0, G_MAXINT, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_RATING,
		 g_param_spec_float
		 ("rating", "rating", "Average user rating for the video",
		  1.0, 5.0, 1.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_PUBLISHED,
		 g_param_spec_string
		 ("published", "timestamp",
		  "Timestamp when the video was uploaded", NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_VIEWS,
		 g_param_spec_int
		 ("views", "viewcount",
		  "Number of times the video has been viewed", 0, G_MAXINT, 0,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_CATEGORY,
		 g_param_spec_string
		 ("category", "label", "Category what the video belongs",
		  NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
		  
	g_object_class_install_property
		(g_klass, PROP_TAGS,
		 g_param_spec_string
		 ("tags", "keywords", "Keywords associated to the video", NULL,
		  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property
		(g_klass, PROP_DESCRIPTION,
		 g_param_spec_string
		 ("description", "description",
		  "Additional information about the video", NULL,
		 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
	
		 return;
}


/** Public methods **/


