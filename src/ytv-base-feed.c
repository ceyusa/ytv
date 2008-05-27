/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-base-feed.c - It is a generic feed
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
 * SECTION: ytv-base-feed
 * @title: YtvBaseFeed
 * @short_description: It is a #YtvFeed implementation for a basic feed
 * extractor
 *
 * This is an implementation of the #YtvFeed interface for the a basic feed
 * extractor
 */

/**
 * YtvBaseFeed:
 *
 * It is a #YtvFeed implementation for the a basic feed extractor
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-error.h>
#include <ytv-base-feed.h>

#include <ytv-feed-parse-strategy.h>
#include <ytv-feed-fetch-strategy.h>
#include <ytv-uri-builder.h>

enum _YtvBaseFeedProp
{
        PROP_O,
        PROP_URI
};

typedef struct _YtvBaseFeedPriv YtvBaseFeedPriv;
struct _YtvBaseFeedPriv
{
        YtvGetEntriesCallback cb;
        gchar* uri;
        gpointer user_data;
};

#define YTV_BASE_FEED_GET_PRIVATE(obj)  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_BASE_FEED, YtvBaseFeedPriv))

static void
fetch_feed_cb (YtvFeedFetchStrategy* st, const gchar* mime,
               const gint8* response, gssize length, GError **err,
               gpointer user_data)
{
        g_return_if_fail (YTV_IS_BASE_FEED (user_data));

        YtvBaseFeed* self = YTV_BASE_FEED (user_data);
        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (self);
        YtvList *feed = NULL;
        GError *tmp_error = NULL;

        if (*err != NULL)
        {
                goto beach;
        }           

        if (g_strrstr
            (mime, ytv_feed_parse_strategy_get_mime (self->parsest)) != NULL)
        {
                feed = ytv_feed_parse_strategy_perform (self->parsest,
                                                        response,
                                                        length,
                                                        &tmp_error);

                if (tmp_error != NULL)
                {
                        g_propagate_error (err, tmp_error);
                }
                
        }
        else
        {
                g_set_error (err, YTV_PARSE_ERROR, YTV_PARSE_ERROR_BAD_MIME,
                             "Bad MIME type receibed - %s", mime);
        }


beach:
        if (priv->cb != NULL)
        {
                priv->cb (YTV_FEED (self), FALSE, feed, err, priv->user_data);
        }

        return;
}

static void
ytv_base_feed_set_fetch_strategy_default (YtvFeed* self,
                                          YtvFeedFetchStrategy* st)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        me->fetchst = g_object_ref (st);
        return;
}

static YtvFeedFetchStrategy*
ytv_base_feed_get_fetch_strategy_default (YtvFeed* self)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        return g_object_ref (me->fetchst);
}

static void
ytv_base_feed_set_parse_strategy_default (YtvFeed* self,
                                          YtvFeedParseStrategy* st)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        me->parsest = g_object_ref (st);

        return;
}

static YtvFeedParseStrategy*
ytv_base_feed_get_parse_strategy_default (YtvFeed *self)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        return g_object_ref (me->fetchst);
}

static YtvUriBuilder*
ytv_base_feed_get_uri_builder_default (YtvFeed* self)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        return g_object_ref (me->uribuild);
}

static void
ytv_base_feed_set_uri_builder_default (YtvFeed* self, YtvUriBuilder* ub)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        me->uribuild = g_object_ref (ub);

        return;
}

static void
ytv_base_feed_standard_default (YtvFeed* self, guint type)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        g_return_if_fail (me->uribuild != NULL);

        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (me);

        if (priv->uri != NULL)
        {
                g_free (priv->uri);
                priv->uri = NULL;
        }
        
        priv->uri = ytv_uri_builder_get_standard_feed (me->uribuild, type);

        g_object_notify (G_OBJECT (me), "uri");

        return;
}

static void
ytv_base_feed_search_default (YtvFeed* self, const gchar* query)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        g_return_if_fail (me->uribuild != NULL);

        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (me);

        if (priv->uri != NULL)
        {
                g_free (priv->uri);
                priv->uri = NULL;
        }
        
        priv->uri = ytv_uri_builder_search_feed (me->uribuild, query);

        g_object_notify (G_OBJECT (me), "uri");

        return;
}

static void
ytv_base_feed_user_default (YtvFeed* self, const gchar* user)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        g_return_if_fail (me->uribuild != NULL);

        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (me);

        if (priv->uri != NULL)
        {
                g_free (priv->uri);
                priv->uri = NULL;
        }
        
        priv->uri = ytv_uri_builder_get_user_feed (me->uribuild, user);

        g_object_notify (G_OBJECT (me), "uri");

        return;        
}

static void
ytv_base_feed_keywords_default (YtvFeed* self,
                                const gchar* category,
                                const gchar* keywords)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        g_return_if_fail (me->uribuild != NULL);

        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (me);

        if (priv->uri != NULL)
        {
                g_free (priv->uri);
                priv->uri = NULL;
        }
        
        priv->uri = ytv_uri_builder_get_keywords_feed (me->uribuild,
                                                       category, keywords);

        g_object_notify (G_OBJECT (me), "uri");

        return;        
}

static void
ytv_base_feed_related_default (YtvFeed* self, const gchar* vid)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);

        g_return_if_fail (me->uribuild != NULL);

        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (me);

        if (priv->uri != NULL)
        {
                g_free (priv->uri);
                priv->uri = NULL;
        }
        
        priv->uri = ytv_uri_builder_get_related_feed (me->uribuild, vid);

        g_object_notify (G_OBJECT (me), "uri");

        return;                
}

static void
ytv_base_feed_get_entries_async_default (YtvFeed* self,
                                         YtvGetEntriesCallback callback,
                                         gpointer user_data)
{
        YtvBaseFeed* me = YTV_BASE_FEED (self);
        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (me);

        g_return_if_fail (me->parsest != NULL);
        g_return_if_fail (me->fetchst != NULL);
        g_return_if_fail (priv->uri != NULL);

        priv->cb = callback;
        priv->user_data = user_data;
        
        ytv_feed_fetch_strategy_perform (me->fetchst, priv->uri,
                                         fetch_feed_cb, me);

        /* put this uri in a history ?? */
        g_free (priv->uri);
        priv->uri = NULL;
       
        return;
}

static void
ytv_feed_init (YtvFeedIface* klass)
{
        klass->get_fetch_strategy = ytv_base_feed_get_fetch_strategy_default;
        klass->set_fetch_strategy = ytv_base_feed_set_fetch_strategy_default;
        klass->set_parse_strategy = ytv_base_feed_set_parse_strategy_default;
        klass->get_parse_strategy = ytv_base_feed_get_parse_strategy_default;
        klass->set_uri_builder = ytv_base_feed_set_uri_builder_default;
        klass->get_uri_builder = ytv_base_feed_get_uri_builder_default;

        klass->standard = ytv_base_feed_standard_default;
        klass->search = ytv_base_feed_search_default;
        klass->user = ytv_base_feed_user_default;
        klass->keywords = ytv_base_feed_keywords_default;
        klass->related = ytv_base_feed_related_default;

        klass->get_entries_async = ytv_base_feed_get_entries_async_default;

        return;
}

G_DEFINE_TYPE_EXTENDED (YtvBaseFeed, ytv_base_feed,
                        G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_FEED,
                                               ytv_feed_init))

static void
ytv_base_feed_get_property (GObject* object, guint prop_id,
                            GValue* value, GParamSpec* spec)
{
        g_return_if_fail (YTV_IS_BASE_FEED (object));

        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_URI:
                g_value_set_string (value, priv->uri);
                break;
        default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
		break;
        }

        return;
}

static void
ytv_base_feed_dispose (GObject* object)
{
        g_return_if_fail (YTV_IS_BASE_FEED (object));

        YtvBaseFeed* self = YTV_BASE_FEED (object);

        (*G_OBJECT_CLASS (ytv_base_feed_parent_class)->dispose) (object);

        if (self->fetchst != NULL)
        {
                g_object_unref (self->fetchst);
        }

        if (self->parsest != NULL)
        {
                g_object_unref (self->parsest);
        }

        if (self->uribuild != NULL)
        {
                g_object_unref (self->uribuild);
        }

        return;
}

static void
ytv_base_feed_finalize (GObject* object)
{
        g_return_if_fail (YTV_IS_BASE_FEED (object));

        YtvBaseFeed* self = YTV_BASE_FEED (object);
        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (self);

        if (priv->uri != NULL)
        {
                g_free (priv->uri);
                priv->uri = NULL;
        }

        (*G_OBJECT_CLASS (ytv_base_feed_parent_class)->finalize) (object);

        return;
}

static void
ytv_base_feed_class_init (YtvBaseFeedClass* klass)
{
        GObjectClass* g_klass = G_OBJECT_CLASS (klass);

        g_type_class_add_private (g_klass, sizeof (YtvBaseFeedPriv));
        
        g_klass->get_property = ytv_base_feed_get_property;
        g_klass->dispose      = ytv_base_feed_dispose;
        g_klass->finalize     = ytv_base_feed_finalize;

        klass->get_fetch_strategy = ytv_base_feed_get_fetch_strategy;
        klass->set_fetch_strategy = ytv_base_feed_set_fetch_strategy;
        klass->set_parse_strategy = ytv_base_feed_set_parse_strategy;
        klass->get_parse_strategy = ytv_base_feed_get_parse_strategy;
        klass->set_uri_builder = ytv_base_feed_set_uri_builder;
        klass->get_uri_builder = ytv_base_feed_get_uri_builder;

        klass->standard = ytv_base_feed_standard;
        klass->search   = ytv_base_feed_search;
        klass->user     = ytv_base_feed_user;
        klass->keywords = ytv_base_feed_keywords;
        klass->related  = ytv_base_feed_related;

        klass->get_entries_async = ytv_base_feed_get_entries_async;

        g_object_class_install_property
                (g_klass, PROP_URI,
                 g_param_spec_string
                 ("uri", "URI", "Feed URI", NULL, G_PARAM_READABLE));
        
        return;
}

static void
ytv_base_feed_init (YtvBaseFeed* self)
{
        YtvBaseFeedPriv* priv = YTV_BASE_FEED_GET_PRIVATE (self);

        self->parsest = NULL;
        self->fetchst = NULL;

        priv->cb = NULL;
        priv->uri = NULL;
        priv->user_data = NULL;

        return;
}


void
ytv_base_feed_set_fetch_strategy (YtvFeed* self,
                                  YtvFeedFetchStrategy* st)
{
}

YtvFeedFetchStrategy*
ytv_base_feed_get_fetch_strategy (YtvFeed* self)
{
}

void
ytv_base_feed_set_parse_strategy (YtvFeed* self,
                                  YtvFeedParseStrategy* st)
{
}

YtvFeedParseStrategy*
ytv_base_feed_get_parse_strategy (YtvFeed *self)
{

}

YtvUriBuilder*
ytv_base_feed_get_uri_builder (YtvFeed* self)
{
}

void
ytv_base_feed_set_uri_builder (YtvFeed* self, YtvUriBuilder* ub)
{
}

void
ytv_base_feed_standard (YtvFeed* self, guint type)
{
}

void
ytv_base_feed_search (YtvFeed* self, const gchar* query)
{
}

void
ytv_base_feed_user (YtvFeed* self, const gchar* user)
{
}

void
ytv_base_feed_keywords (YtvFeed* self, const gchar* category,
                        const gchar* keywords)
{
}

void
ytv_base_feed_related (YtvFeed* self, const gchar* vid)
{
}

void
ytv_base_feed_get_entries_async (YtvFeed* self,
                                 YtvGetEntriesCallback callback,
                                 gpointer user_data)
{
}
