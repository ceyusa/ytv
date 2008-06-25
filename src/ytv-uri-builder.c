/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-uri-builder.c - A type that defines a uri for a feed in the repository
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
 * SECTION: ytv-uri-builder
 * @title: YtvUriBuilder
 * @short_description: A type that defines a resource in the respository
 *
 * An abstract type that defines URI strings which represents a resource in
 * the video repository
 */

/**
 * YtvUriBuilder:
 *
 * Constructs URI strings which represent a resource in a repository.
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-uri-builder.h>

/**
 * ytv_uri_builder_get_standard_feed:
 * @self: a #YtvUriBuilder
 * @type: a type of standard of feed
 *
 * Constructs an URI for the standard feed indicated by @type
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_uri_builder_get_standard_feed (YtvUriBuilder* self, guint type)
{
        gchar* retval;

        g_assert (YTV_IS_URI_BUILDER (self));
        g_assert (YTV_URI_BUILDER_GET_IFACE (self)->get_standard_feed != NULL);

        retval = YTV_URI_BUILDER_GET_IFACE (self)->get_standard_feed (self, type);

        return retval;
}

/**
 * ytv_uri_builder_search_feed:
 * @self: a #YtvUriBuilder
 * @query: (not-null): a string to search
 *
 * Constructs an URI for the feed with the results of the search
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_uri_builder_search_feed (YtvUriBuilder* self, const gchar* query)
{
        gchar* retval;

        g_assert (query != NULL);
        g_assert (YTV_IS_URI_BUILDER (self));
        g_assert (YTV_URI_BUILDER_GET_IFACE (self)->search_feed != NULL);

        retval = YTV_URI_BUILDER_GET_IFACE (self)->search_feed (self, query);

        return retval;
}

/**
 * ytv_uri_builder_get_user_feed:
 * @self: a #YtvUriBuilder
 * @user: (not-null): the username who uploaded the videos to look up
 *
 * Constructs an URI for the feed with the entries uploaded by the specified user.
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_uri_builder_get_user_feed (YtvUriBuilder* self, const gchar* user)
{
        gchar* retval;

        g_assert (user != NULL);
        g_assert (YTV_IS_URI_BUILDER (self));
        g_assert (YTV_URI_BUILDER_GET_IFACE (self)->get_user_feed != NULL);

        retval = YTV_URI_BUILDER_GET_IFACE (self)->get_user_feed (self, user);

        return retval;
}

/**
 * ytv_uri_builder_get_keywords_feed:
 * @self: a #YtvUriBuilder
 * @category: (null-ok): the category to browse
 * @keywords: (null-ok): the space separated keywords list to browse
 *
 * Constructs an URI for the feed with the entries in the category or tagged
 * by the keywords string.
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_uri_builder_get_keywords_feed (YtvUriBuilder* self, const gchar* category,
                                   const gchar* keywords)
{
        gchar* retval;

        g_assert (category != NULL || keywords != NULL);
        g_assert (YTV_IS_URI_BUILDER (self));
        g_assert (YTV_URI_BUILDER_GET_IFACE (self)->get_keywords_feed != NULL);

        retval = YTV_URI_BUILDER_GET_IFACE (self)->get_keywords_feed (self,
                                                                      category,
                                                                      keywords);

        return retval;
}

/**
 * ytv_uri_builder_get_related_feed:
 * @self: a #YtvUriBuilder
 * @vid: (not-null): the entry id 
 *
 * Constructs an URI for the feed with the entries related to the @vid
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_uri_builder_get_related_feed (YtvUriBuilder* self, const gchar* vid)
{
        gchar* retval;

        g_assert (vid != NULL);
        g_assert (YTV_IS_URI_BUILDER (self));
        g_assert (YTV_URI_BUILDER_GET_IFACE (self)->get_related_feed != NULL);

        retval = YTV_URI_BUILDER_GET_IFACE (self)->get_related_feed (self, vid);

        return retval;
}

/**
 * ytv_uri_builder_get_thumbnail:
 * @self: a #YtvUriBuilder
 * @vid: (not-null): the entry id
 *
 * Construct an URI for the @vid thumbnail to retrieve
 *
 * returns: (null-ok): (caller-owns): the URI string to the @vid thumbnail.
 * The string must be free after user.
 */
gchar*
ytv_uri_builder_get_thumbnail (YtvUriBuilder* self, const gchar* vid)
{
        gchar* retval;

        g_assert (vid != NULL);
        g_assert (YTV_IS_URI_BUILDER (self));
        g_assert (YTV_URI_BUILDER_GET_IFACE (self)->get_thumbnail != NULL);

        retval = YTV_URI_BUILDER_GET_IFACE (self)->get_thumbnail (self, vid);

        return retval;        
}
 
/**
 * ytv_uri_builder_get_current_feed:
 * @self: a #YtvUriBuilder
 *
 * Reconstructs the URI for the previous requested feed
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_uri_builder_get_current_feed (YtvUriBuilder* self)
{
        gchar* retval;

        g_assert (YTV_IS_URI_BUILDER (self));
        if (YTV_URI_BUILDER_GET_IFACE (self)->get_current_feed == NULL)
        {
                g_critical ("You must implement ytv_uri_builder_get_current_feed\n");
        }

        retval = YTV_URI_BUILDER_GET_IFACE (self)->get_current_feed (self);

        return retval;
}

static void
ytv_uri_builder_base_init (gpointer g_class)
{
        static gboolean initialized = FALSE;

        if (!initialized)
        {
                initialized = TRUE;
        }

        return;
}

GType
ytv_uri_builder_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GTypeInfo info =
                {
                        sizeof (YtvUriBuilderIface),
                        ytv_uri_builder_base_init,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        0,
                        0,
                        NULL,
                        NULL
                };

                type = g_type_register_static (G_TYPE_INTERFACE, "YtvUriBuilder",
                                               &info, 0);

                g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
        }

        return type;
}
