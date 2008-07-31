/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-youtube-uri-builder.c - A type that defines URIs for the
 * youtube video repository
 * Copyright (C) 2008 Víctor Manuel Jáquez Leal <vjaquez@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * SECTION: ytv-youtube-uri-builder
 * @title: YtvYoutubeUriBuilder
 * @short_description: It is a #YtvUriBuilder implementation for the YouTube
 * video repository
 *
 * This is an implementation of the #YtvUriBuilder interface for the YouTube
 * service.
 */

/**
 * YtvYoutubeUriBuilder:
 *
 * It is a #YtvUriBuilder implementation for the YouTube video repository
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-youtube-uri-builder.h>

/**
 * YtvYoutubeOrder:
 *
 * Order to sort videos in the search result set
 *
 * @YTV_YOUTUBE_ORDER_RELEVANCE:
 * @YTV_YOUTUBE_ORDER_PUBLISHED:
 * @YTV_YOUTUBE_ORDER_VIEWCOUNT:
 * @YTV_YOUTUBE_ORDER_RATING:
 */

enum _YtvYoutubeUriBuilderProp
{
        PROP_0,
        PROP_ORDERBY,
        PROP_START_INDEX,
        PROP_MAX_RESULTS,
        PROP_AUTHOR,
        PROP_ALT,
        PROP_TIME
};

typedef struct _YtvYoutubeUriBuilderPriv YtvYoutubeUriBuilderPriv;
struct _YtvYoutubeUriBuilderPriv
{
        YtvUriBuilderReqFeedType reqtype;
        gchar* arg1;
        gchar* arg2;
        YtvYoutubeStdFeed arg3;

        YtvYoutubeOrder orderby;
        gint start_index;
        gint max_results;
        gchar* author;
        YtvYoutubeAlt alt;
        YtvYoutubeTime time;
};

#define YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE(obj)        \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), YTV_TYPE_YOUTUBE_URI_BUILDER, YtvYoutubeUriBuilderPriv))

#define BASEURL "http://gdata.youtube.com/feeds/api/"
#define IMGURL  "http://img.youtube.com/vi/%s/default.jpg"
#define RESERVEDCHARS "/:?&=-#@+"


typedef gchar* (*SetParam) (YtvYoutubeUriBuilder* self);

static gchar*
orderby_param (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;
        const gchar* orderby;
        gchar* retval;

        retval = NULL;
        
        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);
        
        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        switch (priv->orderby)
        {
        case YTV_YOUTUBE_ORDER_RELEVANCE:
                return NULL; /* this is by default */
                break;
        case YTV_YOUTUBE_ORDER_PUBLISHED:
                orderby = "published";
                break;
        case YTV_YOUTUBE_ORDER_VIEWCOUNT:
                orderby = "viewCount";
                break;
        case YTV_YOUTUBE_ORDER_RATING:
                orderby = "rating";
                break;
        default:
                g_warning ("Not valid order");
                return NULL;
        }

        retval = g_strdup_printf ("orderby=%s", orderby);
        return retval;
}

static gchar*
start_index_param (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;
        gchar* retval = NULL;
        
        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        if (priv->start_index == 0)
                return NULL; /* no explicity declared */

        retval = g_strdup_printf ("start-index=%d", priv->start_index);
        return retval;
}

static gchar*
max_results_param (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;
        gchar* retval = NULL;
        
        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        if (priv->max_results == 25)
                return NULL; /* default value  */

        retval = g_strdup_printf ("max-results=%d", priv->max_results);
        return retval;
}

static gchar*
author_param (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;
        gchar *retval = NULL;

        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        if (priv->author != NULL)
        {
                retval = g_strdup_printf ("author=%s", priv->author);
        }

        return retval;
}

static gchar*
alt_param (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;
        const gchar* alt;
        gchar* retval = NULL;

        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        switch (priv->alt)
        {
        case YTV_YOUTUBE_ALT_ATOM:
                return NULL; /* default value */
        case YTV_YOUTUBE_ALT_RSS:
                alt = "rss";
                break;
        case YTV_YOUTUBE_ALT_JSON:
                alt = "json";
                break;
        case YTV_YOUTUBE_ALT_JSON_IN_SCRIPT:
                alt = "json-in-script";
                break;
        default:
                g_warning ("Not valid alt");
                return NULL;
        }

        retval = g_strdup_printf ("alt=%s", alt);
        return retval;
}

static gchar*
time_param (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;
        const gchar* time;
        gchar* retval = NULL;

        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);
        
        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        switch (priv->time)
        {
        case YTV_YOUTUBE_TIME_TODAY:
                time = "today";
                break;
        case YTV_YOUTUBE_TIME_THIS_WEEK:
                time = "this_week";
                break;
        case YTV_YOUTUBE_TIME_THIS_MONTH:
                time = "this_month";
                break;
        case YTV_YOUTUBE_TIME_ALL_TIME:
                return NULL; /* default value */
        default:
                g_warning ("Not valid time");
                return NULL;
        }

        retval = g_strdup_printf ("time=%s", time);
        return retval;
}

static gchar*
all_params (YtvYoutubeUriBuilder* self, gboolean time)
{
        YtvYoutubeUriBuilderPriv* priv;
        gchar *param, *retval = NULL;
        gint i;

        SetParam setparams[] = {
                orderby_param,
                start_index_param,
                max_results_param,
                author_param,
                alt_param,
                time_param,
                NULL
        };
        
        g_return_val_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (self), NULL);

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        for (i = 0; setparams[i] != NULL; i++)
        {
                if (setparams[i] == time_param && time == FALSE)
                {
                        continue;
                }

                param = setparams[i] (self);
                if (param != NULL)
                {
                        if (retval != NULL)
                        {
                                gchar* tmp;
                                tmp = g_strconcat (retval, "&", param, NULL);
                                g_free (retval);
                                retval = tmp;
                        }
                        else
                        {
                                retval = g_strdup (param);
                        }
                        g_free (param);
                }
        }

        return retval;
}

static gchar*
ytv_youtube_uri_builder_get_standard_feed_default (YtvUriBuilder* self,
                                                   guint type)
{
        const gchar* feedtype;
        gchar* retval;
        gchar* params;
        
        switch (type)
        {
        case YTV_YOUTUBE_STD_FEED_TOP_RATED:
                feedtype = "top_rated";
                break;
        case YTV_YOUTUBE_STD_FEED_TOP_FAVORITES:
                feedtype = "top_favorites";
                break;
        case YTV_YOUTUBE_STD_FEED_MOST_VIEWED:
                feedtype = "most_viewed";
                break;
        case YTV_YOUTUBE_STD_FEED_MOST_RECENT:
                feedtype = "most_recent";
                break;
        case YTV_YOUTUBE_STD_FEED_MOST_DISCUSSED:
                feedtype = "most_discussed";
                break;
        case YTV_YOUTUBE_STD_FEED_MOST_LINKED:
                feedtype = "most_linked";
                break;
        case YTV_YOUTUBE_STD_FEED_MOST_RESPONDED:
                feedtype = "most_responded";
                break;
        case YTV_YOUTUBE_STD_FEED_RECENTLY_FEATURED:
                feedtype = "recently_featured";
                break;
        case YTV_YOUTUBE_STD_FEED_WATCH_ON_MOBILE:
                feedtype = "watch_on_mobile";
                break;
        default:
                g_warning ("Not valid standard feed type");
                return NULL;
        }

        retval = g_strconcat (BASEURL, "standardfeeds/", feedtype, NULL);

        params = all_params (YTV_YOUTUBE_URI_BUILDER (self), TRUE);
        if (params != NULL)
        {
                gchar* tmp = g_strconcat (retval, "?", params, NULL);
                g_free (retval);
                retval = tmp;
                g_free (params);
        }

        {
                YtvYoutubeUriBuilderPriv* priv;
                priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);
                priv->reqtype = YTV_URI_BUILDER_REQ_FEED_TYPE_STANDARD;
                priv->arg3    = type;

                if (priv->arg1 != NULL)
                {
                        g_free (priv->arg1);
                        priv->arg1 = NULL;
                }

                if (priv->arg2 != NULL)
                {
                        g_free (priv->arg2);
                        priv->arg2 = NULL;
                }
        }
        
        return retval;
}

static gchar*
ytv_youtube_uri_builder_search_feed_default (YtvUriBuilder* self,
                                             const gchar* query)
{
        /* @todo */
        GRegex* regex;
        gchar* retval;
        gchar* params;
        gchar *p;
        gchar *q = g_strstrip (g_strdup (query));

        regex = g_regex_new ("\\s+", 0, 0, NULL);

        if (regex != NULL)
        {
                p = g_regex_replace (regex, q, -1, 0, "+", 0, NULL);

                if (p == NULL)
                {
                        p = q;
                }
                else
                {
                        g_free (q);
                }

                g_regex_unref (regex);
        }
        else
        {
                p = q; /* good luck my friend */
        }
        
        retval = g_strconcat (BASEURL, "videos?vq=", p, NULL);

        g_free (p);

        params = all_params (YTV_YOUTUBE_URI_BUILDER (self), TRUE);
        if (params != NULL)
        {
                gchar* tmp = g_strconcat (retval, "&", params, NULL);
                g_free (retval);
                retval = tmp;
                g_free (params);
        }

        {
                YtvYoutubeUriBuilderPriv* priv;
                priv          = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);
                priv->reqtype = YTV_URI_BUILDER_REQ_FEED_TYPE_SEARCH;
                priv->arg3    = 0;

                if (priv->arg1 != NULL && priv->arg1 != query)
                {
                        g_free (priv->arg1);
                        priv->arg1 = NULL;
                }

                /* strdup when arg1 == NULL && arg1 != query */
                if (priv->arg1 != query) 
                {
                        priv->arg1 = g_strstrip (g_strdup (query));
                }

                if (priv->arg2 != NULL)
                {
                        g_free (priv->arg2);
                        priv->arg2 = NULL;
                }
        }

        return retval;
}

static gchar*
ytv_youtube_uri_builder_get_user_feed_default (YtvUriBuilder* self,
                                               const gchar* user)
{
        gchar* retval;
        gchar* params;
        gchar *u = g_strstrip (g_strdup (user));

        retval = g_strconcat (BASEURL, "users/", u, "/uploads", NULL);
        params = all_params (YTV_YOUTUBE_URI_BUILDER (self), FALSE);
        
        if (params != NULL)
        {
                gchar* tmp = g_strconcat (retval, "?", params, NULL);
                g_free (retval);
                retval = tmp;
                g_free (params);
        }

        {
                YtvYoutubeUriBuilderPriv* priv;
                priv          = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);
                priv->reqtype = YTV_URI_BUILDER_REQ_FEED_TYPE_USER;
                priv->arg3    = 0;

                if (priv->arg1 != NULL)
                {
                        g_free (priv->arg1);
                        priv->arg1 = NULL;
                }
                priv->arg1 = u;

                if (priv->arg2 != NULL)
                {
                        g_free (priv->arg2);
                        priv->arg2 = NULL;
                }
        }

        return retval;
}

static gchar*
ytv_youtube_uri_builder_get_keywords_feed_default (YtvUriBuilder* self,
                                                   const gchar* category,
                                                   const gchar* keywords)
{
        gchar* retval;
        gchar* params;
        gchar* c = g_strstrip (g_strdup (category));
        gchar* k = g_strstrip (g_strdup (keywords));

        retval = g_strconcat (BASEURL, "videos/-/", c, NULL);
        params = all_params (YTV_YOUTUBE_URI_BUILDER (self), FALSE);
        
        if (params != NULL)
        {
                gchar* tmp = g_strconcat (retval, "?", params, NULL);
                g_free (retval);
                retval = tmp;
                g_free (params);
        }
        
        {
                YtvYoutubeUriBuilderPriv* priv;
                priv          = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);
                priv->reqtype = YTV_URI_BUILDER_REQ_FEED_TYPE_KEYWORDS;
                priv->arg3    = 0;

                if (priv->arg1 != NULL)
                {
                        g_free (priv->arg1);
                        priv->arg1 = NULL;
                }
                priv->arg1 = c;

                if (priv->arg2 != NULL)
                {
                        g_free (priv->arg2);
                        priv->arg2 = NULL;
                }
                priv->arg2 = k;
        }

        return retval;
}

gchar*
ytv_youtube_uri_builder_get_related_feed_default (YtvUriBuilder* self,
                                                  const gchar* vid)
{
        gchar* retval;
        gchar* params;
        gchar* id = g_strstrip (g_strdup (vid));

        retval = g_strconcat (BASEURL, "videos/",  id, "/related", NULL);
        params = all_params (YTV_YOUTUBE_URI_BUILDER (self), FALSE);
        
        if (params != NULL)
        {
                gchar* tmp = g_strconcat (retval, "?", params, NULL);
                g_free (retval);
                retval = tmp;
                g_free (params);
        }

        {
                YtvYoutubeUriBuilderPriv* priv;
                priv          = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);
                priv->reqtype = YTV_URI_BUILDER_REQ_FEED_TYPE_RELATED;
                priv->arg3    = 0;

                if (priv->arg1 != NULL)
                {
                        g_free (priv->arg1);
                        priv->arg1 = NULL;
                }
                priv->arg1 = id;

                if (priv->arg2 != NULL)
                {
                        g_free (priv->arg2);
                        priv->arg2 = NULL;
                }
        }
        
        return retval;
}

static gchar*
ytv_youtube_uri_builder_get_thumbnail_default (YtvUriBuilder* self,
                                               const gchar* vid)
{
        gchar* retval = g_strdup_printf (IMGURL, vid);        
        return retval;
}

static gchar*
ytv_youtube_uri_builder_get_current_feed_default (YtvUriBuilder* self)
{
        gchar* retval = NULL;
        YtvYoutubeUriBuilderPriv* priv;
        
        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        switch (priv->reqtype)
        {
        case YTV_URI_BUILDER_REQ_FEED_TYPE_STANDARD:
                retval = ytv_uri_builder_get_standard_feed (self, priv->arg3);
                break;
        case YTV_URI_BUILDER_REQ_FEED_TYPE_SEARCH:
                retval = ytv_uri_builder_search_feed (self, priv->arg1);
                break;
        case YTV_URI_BUILDER_REQ_FEED_TYPE_USER:
                retval = ytv_uri_builder_get_user_feed (self, priv->arg1);
                break;
        case YTV_URI_BUILDER_REQ_FEED_TYPE_KEYWORDS:
                retval = ytv_uri_builder_get_keywords_feed (self,
                                                            priv->arg1,
                                                            priv->arg2);
                break;
        case YTV_URI_BUILDER_REQ_FEED_TYPE_RELATED:
                retval = ytv_uri_builder_get_related_feed (self, priv->arg1);
                break;
        default:
                g_warning ("Unknown request feed type");
        }

        return retval;
}

static void
ytv_uri_builder_init (YtvUriBuilderIface* klass)
{
        klass->get_standard_feed = ytv_youtube_uri_builder_get_standard_feed;
        klass->search_feed = ytv_youtube_uri_builder_search_feed;
        klass->get_user_feed = ytv_youtube_uri_builder_get_user_feed;
        klass->get_keywords_feed = ytv_youtube_uri_builder_get_keywords_feed;
        klass->get_related_feed = ytv_youtube_uri_builder_get_related_feed;
        klass->get_thumbnail = ytv_youtube_uri_builder_get_thumbnail;
        klass->get_current_feed = ytv_youtube_uri_builder_get_current_feed;

        return;
}

G_DEFINE_TYPE_EXTENDED (YtvYoutubeUriBuilder, ytv_youtube_uri_builder,
                        G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_URI_BUILDER,
                                               ytv_uri_builder_init))

static void
ytv_youtube_uri_builder_set_property (GObject* object, guint prop_id,
                                      const GValue* value, GParamSpec* spec)
{
        YtvYoutubeUriBuilderPriv* priv;
        
        g_return_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (object));
        
        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ORDERBY:
                priv->orderby = g_value_get_enum (value);
                break;
        case PROP_START_INDEX:
                priv->start_index = g_value_get_int (value);
                break;
        case PROP_MAX_RESULTS:
                priv->max_results = g_value_get_int (value);
                break;
        case PROP_AUTHOR:
        {
                const gchar* author = g_value_get_string (value);
                g_return_if_fail (author != NULL);

                if (priv->author != NULL)
                        g_free (priv->author);

                priv->author = g_strdup (author);
        }
                break;
        case PROP_ALT:
                priv->alt = g_value_get_enum (value);
                break;
        case PROP_TIME:
                priv->time = g_value_get_enum (value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_youtube_uri_builder_get_property (GObject* object, guint prop_id,
                                      GValue* value, GParamSpec* spec)
{
        YtvYoutubeUriBuilderPriv* priv;
        
        g_return_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (object));

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (object);

        switch (prop_id)
        {
        case PROP_ORDERBY:
                g_value_set_enum (value, priv->orderby);
                break;
        case PROP_START_INDEX:
                g_value_set_int (value, priv->start_index);
                break;
        case PROP_MAX_RESULTS:
                g_value_set_int (value, priv->max_results);
                break;
        case PROP_AUTHOR:
                g_value_set_string (value, priv->author);
                break;
        case PROP_ALT:
                g_value_set_enum (value, priv->alt);
                break;
        case PROP_TIME:
                g_value_set_enum (value, priv->time);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, spec);
                break;
        }

        return;
}

static void
ytv_youtube_uri_builder_finalize (GObject* object)
{
        YtvYoutubeUriBuilderPriv* priv;
        
        g_return_if_fail (YTV_IS_YOUTUBE_URI_BUILDER (object));
        
        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (object);

        if (priv->author != NULL)
        {
                g_free (priv->author);
                priv->author = NULL;
        }

        if (priv->arg1 != NULL)
        {
                g_free (priv->arg1);
                priv->arg1 = NULL;
        }
        
        if (priv->arg2 != NULL)
        {
                g_free (priv->arg2);
                priv->arg2 = NULL;
        }

        (*G_OBJECT_CLASS (ytv_youtube_uri_builder_parent_class)->finalize) (object);

        return;
}

static void
ytv_youtube_uri_builder_class_init (YtvYoutubeUriBuilderClass* klass)
{
        GObjectClass* g_klass = G_OBJECT_CLASS (klass);

        g_type_class_add_private (g_klass, sizeof (YtvYoutubeUriBuilderPriv));
        g_klass->set_property = ytv_youtube_uri_builder_set_property;
        g_klass->get_property = ytv_youtube_uri_builder_get_property;
        g_klass->finalize = ytv_youtube_uri_builder_finalize;

        klass->get_standard_feed =
                ytv_youtube_uri_builder_get_standard_feed_default;
        klass->search_feed = ytv_youtube_uri_builder_search_feed_default;
        klass->get_user_feed = ytv_youtube_uri_builder_get_user_feed_default;
        klass->get_keywords_feed =
                ytv_youtube_uri_builder_get_keywords_feed_default;
        klass->get_related_feed =
                ytv_youtube_uri_builder_get_related_feed_default;
        klass->get_thumbnail = ytv_youtube_uri_builder_get_thumbnail_default;
        klass->get_current_feed =
                ytv_youtube_uri_builder_get_current_feed_default;

        g_object_class_install_property
                (g_klass, PROP_ORDERBY,
                 g_param_spec_enum
                 ("orderby", "orderby",
                  "Order to sort videos in the search result set",
                  YTV_TYPE_YOUTUBE_ORDER, YTV_YOUTUBE_ORDER_RELEVANCE,
                  G_PARAM_READWRITE));

        g_object_class_install_property
                (g_klass, PROP_START_INDEX,
                 g_param_spec_int
                 ("start-index", "startindex",
                  "Index of the first matching result "
                  "that should be included in the result set", 0, G_MAXINT, 0,
                  G_PARAM_READWRITE));

        g_object_class_install_property
                (g_klass, PROP_MAX_RESULTS,
                 g_param_spec_int
                 ("max-results", "maxresults", "The maximum number of results "
                  "that should be included in the result set", 0, 50, 25,
                  G_PARAM_READWRITE));

        g_object_class_install_property
                (g_klass, PROP_AUTHOR,
                 g_param_spec_string
                 ("author", "author", "Restricts the search to videos uploaded "
                  "by a particular YouTube user", NULL, G_PARAM_READWRITE));

        g_object_class_install_property
                (g_klass, PROP_ALT,
                 g_param_spec_enum
                 ("alt", "alt", "The format of the feed to be returned",
                  YTV_TYPE_YOUTUBE_ALT, YTV_YOUTUBE_ALT_JSON,
                  G_PARAM_READWRITE));

        g_object_class_install_property
                (g_klass, PROP_TIME,
                 g_param_spec_enum
                 ("time", "time", "Restricts the search to videos uploaded "
                  " within the specified time", YTV_TYPE_YOUTUBE_TIME,
                  YTV_YOUTUBE_TIME_ALL_TIME, G_PARAM_READWRITE));

        return;
}

static void
ytv_youtube_uri_builder_init (YtvYoutubeUriBuilder* self)
{
        YtvYoutubeUriBuilderPriv* priv;

        priv = YTV_YOUTUBE_URI_BUILDER_GET_PRIVATE (self);

        priv->reqtype = 0;
        priv->arg1    = NULL;
        priv->arg2    = NULL;
        priv->arg3    = 0;
        
        priv->orderby     = YTV_YOUTUBE_ORDER_RELEVANCE;
        priv->start_index = 0;
        priv->max_results = 25;
        priv->author      = NULL;
        priv->alt         = YTV_YOUTUBE_ALT_JSON;
        priv->time        = YTV_YOUTUBE_TIME_ALL_TIME;

        return;
}

/* public methods */

/**
 * ytv_youtube_uri_builder_new:
 *
 * Creates a new instance of the #YtvYoutubeUriBuilder which
 * implements the #YtvUriBuilder interface
 *
 * returns: (not-null): a new youtube implementation of the
 * #YtvUriBuilder interface
 */
YtvUriBuilder*
ytv_youtube_uri_builder_new (void)
{
        YtvYoutubeUriBuilder* self;

        self = g_object_new (YTV_TYPE_YOUTUBE_URI_BUILDER, NULL);

        return YTV_URI_BUILDER (self);
}

/**
 * ytv_youtube_uri_builder_get_standard_feed:
 * @self: a #YtvUriBuilder
 * @type: a type of standard of feed
 *
 * Constructs an URI for the standard feed indicated by @type
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_youtube_uri_builder_get_standard_feed (YtvUriBuilder* self, guint type)
{
        gchar* retval;

        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_standard_feed != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_standard_feed
                (self, type);

        return retval;
}

/**
 * ytv_youtube_uri_builder_search_feed:
 * @self: a #YtvUriBuilder
 * @query: (not-null): a string to search
 *
 * Constructs an URI for the feed with the results of the search
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_youtube_uri_builder_search_feed (YtvUriBuilder* self, const gchar* query)
{
        gchar* retval;

        g_assert (query != NULL);
        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_standard_feed != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->search_feed
                (self, query);

        return retval;
}

/**
 * ytv_youtube_uri_builder_get_user_feed:
 * @self: a #YtvUriBuilder
 * @user: (not-null): the username who uploaded the videos to look up
 *
 * Constructs an URI for the feed with the entries uploaded by the specified user.
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_youtube_uri_builder_get_user_feed (YtvUriBuilder* self, const gchar* user)
{
        gchar* retval;

        g_assert (user != NULL);
        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_user_feed != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_user_feed
                (self, user);

        return retval;
}

/**
 * ytv_youtube_uri_builder_get_keywords_feed:
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
ytv_youtube_uri_builder_get_keywords_feed (YtvUriBuilder* self, const gchar* category,
                                           const gchar* keywords)
{
        gchar* retval;

        g_assert (category != NULL || keywords != NULL);
        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_keywords_feed != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_keywords_feed
                (self, category, keywords);

        return retval;
}

/**
 * ytv_youtube_uri_builder_get_related_feed:
 * @self: a #YtvUriBuilder
 * @vid: (not-ok): the video id
 *
 * Constructs an URI for the feed with the entries related to the @vid
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_youtube_uri_builder_get_related_feed (YtvUriBuilder* self, const gchar* vid)
{
        gchar* retval;

        g_assert (vid != NULL);
        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_related_feed != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_related_feed
                (self, vid);

        return retval;
}

/**
 * ytv_youtube_uri_builder_get_thumbnail:
 * @self: a #YtvUriBuilder
 * @vid: (not-ok): the video id
 *
 * Constructs an URI for the @vid entry's thumbnail
 *
 * returns: (null-ok): (caller-owns): the URI string for the thumbnail.
 * The string must be freed after use.
 */
gchar*
ytv_youtube_uri_builder_get_thumbnail (YtvUriBuilder* self, const gchar* vid)
{
        gchar* retval;

        g_assert (vid != NULL);
        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_thumbnail != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_thumbnail
                (self, vid);

        return retval;
}

/**
 * ytv_youtube_uri_builder_get_current_feed:
 * @self: a #YtvUriBuilder
 *
 * Reconstructs the URI for the previous requested feed
 *
 * returns: (null-ok): (caller-owns): the URI string representing the resource.
 * The string must be freed after use.
 */
gchar*
ytv_youtube_uri_builder_get_current_feed (YtvUriBuilder* self)
{
        gchar* retval;

        g_assert (YTV_IS_YOUTUBE_URI_BUILDER (self));
        g_assert (YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_current_feed != NULL);

        retval = YTV_YOUTUBE_URI_BUILDER_GET_CLASS (self)->get_current_feed
                (self);

        return retval;
}

/**
 * ytv_youtube_order_get_type:
 *
 * GType system helper function
 *
 * returns: a #GType
 */
GType
ytv_youtube_order_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GEnumValue values[] = {
                        { YTV_YOUTUBE_ORDER_RELEVANCE,
                          "YTV_YOUTUBE_ORDER_RELEVANCE", "relevance" },
                        { YTV_YOUTUBE_ORDER_PUBLISHED,
                          "YTV_YOUTUBE_ORDER_PUBLISHED", "published" },
                        { YTV_YOUTUBE_ORDER_VIEWCOUNT,
                          "YTV_YOUTUBE_ORDER_VIEWCOUNT", "viewcount" },
                        { YTV_YOUTUBE_ORDER_RATING,
                          "YTV_YOUTUBE_ORDER_RATING", "rating" },
                        { 0, NULL, NULL }
                };

                type = g_enum_register_static ("YtvYoutubeOrderType", values);
        }

        return type;
}

/**
 * ytv_youtube_alt_get_type:
 *
 * GType system helper function
 *
 * returns: a #GType
 */
GType
ytv_youtube_alt_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GEnumValue values[] = {
                        { YTV_YOUTUBE_ALT_ATOM,
                          "YTV_YOUTUBE_ALT_ATOM", "atom" },
                        { YTV_YOUTUBE_ALT_RSS,
                          "YTV_YOUTUBE_ALT_RSS", "rss" },
                        { YTV_YOUTUBE_ALT_JSON,
                          "YTV_YOUTUBE_ALT_JSON", "json" },
                        { YTV_YOUTUBE_ALT_JSON_IN_SCRIPT,
                          "YTV_YOUTUBE_ALT_JSON_IN_SCRIPT", "json-in-script" },
                        { 0, NULL, NULL }
                };

                type = g_enum_register_static ("YtvYoutubeAltType", values);
        }

        return type;
}

/**
 * ytv_youtube_time_get_type:
 *
 * GType system helper function
 *
 * returns: a #GType
 */
GType
ytv_youtube_time_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GEnumValue values[] = {
                        { YTV_YOUTUBE_TIME_TODAY,
                          "YTV_YOUTUBE_TIME_TODAY", "today" },
                        { YTV_YOUTUBE_TIME_THIS_WEEK,
                          "YTV_YOUTUBE_TIME_THIS_WEEK", "this_week" },
                        { YTV_YOUTUBE_TIME_THIS_MONTH,
                          "YTV_YOUTUBE_TIME_THIS_MONTH", "this_month" },
                        { YTV_YOUTUBE_TIME_ALL_TIME,
                          "YTV_YOUTUBE_TIME_ALL_TIME", "all_time" },
                        { 0, NULL, NULL }
                };

                type = g_enum_register_static ("YtvYoutubeTimeType", values);
        }

        return type;
}
