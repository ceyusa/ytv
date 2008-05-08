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
 * SECTION:ytv-soup-feed-fetch-strategy
 * @short_description: Object for HTTP communication using libsoup
 *
 * It is a #YtvFeedFetchStrategy implementation using the libsoup
 * library for the HTTP client communications.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libsoup/soup.h>
#include <gconf/gconf-client.h>

#include <ytv-error.h>
#include <ytv-soup-feed-fetch-strategy.h>

typedef struct _YtvSoupFeedFetchStrategyPriv YtvSoupFeedFetchStrategyPriv;

struct _YtvSoupFeedFetchStrategyPriv
{
	SoupSession* session;
};

/* helper for the session_async queue */
typedef struct _YtvCbWrapper YtvCbWrapper;
struct _YtvCbWrapper
{
        YtvFeedFetchStrategy*  st;
        YtvGetResponseCallback cb;
};

#define YTV_SOUP_FEED_FETCH_STRATEGY_GET_PRIVATE(o) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), YTV_TYPE_SOUP_FEED_FETCH_STRATEGY, YtvSoupFeedFetchStrategyPriv))

/* creates a HTTP session and set the proxy */
static void
create_session (YtvSoupFeedFetchStrategy* self)
{
        YtvSoupFeedFetchStrategyPriv* priv =
                YTV_SOUP_FEED_FETCH_STRATEGY_GET_PRIVATE (self);
        GConfClient* conf_client;

        if (priv->session != NULL)
        {
                return; /* no need to have another */
        }

        priv->session = soup_session_async_new_with_options
                (SOUP_SESSION_USER_AGENT, "youtube-viewer/" VERSION, NULL);

        conf_client = gconf_client_get_default ();

        if (gconf_client_get_bool (conf_client,
                                   "/system/http_proxy/use_http_proxy", NULL))
        {
                gchar *server, *proxy_uri;
                gint port;

                server = gconf_client_get_string (conf_client,
                                                  "/system/http_proxy/host",
                                                  NULL);
                port = gconf_client_get_int (conf_client,
                                             "/system/http_proxy/port", NULL);

                if (server && server[0])
                {
                        SoupURI *suri;

                        if (gconf_client_get_bool
                            (conf_client,
                             "/system/http_proxy/use_authentication",
                             NULL))
                        {
                                gchar *user, *password;

                                user = gconf_client_get_string
                                        (conf_client,
                                         "/system/http_proxy/authentication_user",
                                         NULL);
                                
                                password = gconf_client_get_string
                                        (conf_client,
                                         "/system/http_proxy/authentication_password",
                                         NULL);

                                proxy_uri = g_strdup_printf ("http://%s:%s@%s:%d",
                                                             user, password,
                                                             server, port);

                                g_free (user);
                                g_free (password);
                        }
                        else
                        {
                                proxy_uri = g_strdup_printf ("http://%s:%d",
                                                             server, port);
                        }

                        suri = soup_uri_new (proxy_uri);
                        g_object_set (G_OBJECT (priv->session),
                                      SOUP_SESSION_PROXY_URI, suri, NULL);

                        soup_uri_free (suri);

                        g_free (server);
                        g_free (proxy_uri);
                }
        }

        g_object_unref (conf_client);

        return;
}

static void
retrieval_done (SoupSession* session, SoupMessage* message, gpointer user_data)
{
        g_assert (user_data != NULL);

        YtvCbWrapper* cbw = (YtvCbWrapper*) user_data;

        YtvSoupFeedFetchStrategy* self =
                YTV_SOUP_FEED_FETCH_STRATEGY (cbw->st);
        YtvSoupFeedFetchStrategyPriv* priv =
                YTV_SOUP_FEED_FETCH_STRATEGY_GET_PRIVATE (self);
        GError *err = NULL;
                
        if (!SOUP_STATUS_IS_SUCCESSFUL (message->status_code))
        {
                g_set_error (&err, YTV_HTTP_ERROR, YTV_HTTP_ERROR_CONNECTION,
                             "HTTP error - HTTP/1.%d %d %s",
                             soup_message_get_http_version (message),
                             message->status_code, message->reason_phrase);

                if (cbw->cb != NULL)
                {
                        cbw->cb (cbw->st, NULL, NULL, 0, err);
                }
                goto done;
        }

        const gchar* mimetype = soup_message_headers_get
                (message->response_headers, "Content-Type");

        if (cbw->cb != NULL)
        {
                cbw->cb (cbw->st, mimetype, message->response_body->data,
                         message->response_body->length, err);
        }
        
done:
        g_slice_free (YtvCbWrapper, cbw);
        return;
}

/**
 * ytv_soup_feed_fetch_strategy_perform:
 * @self: a #YtvSoupFeedFetchStrategy instance
 * @uri: the URI to fetch
 * @callback: a #YtvGetResponseCallback to execute when the response arrives
 *
 * Performs the async fetch of a feed through HTTP using
 * libsoup.
 */
void
ytv_soup_feed_fetch_strategy_perform (YtvFeedFetchStrategy *self, gchar* uri,
                                      YtvGetResponseCallback callback)
{
        g_assert (self != NULL);
        g_assert (YTV_IS_SOUP_FEED_FETCH_STRATEGY (self));
        g_assert (uri != NULL);
        
	YTV_SOUP_FEED_FETCH_STRATEGY_GET_CLASS (self)->perform (self, uri,
                                                                callback);

	return;
}

static void
ytv_soup_feed_fetch_strategy_perform_default (YtvFeedFetchStrategy* self,
					      gchar* uri,
                                              YtvGetResponseCallback callback)
{
        g_assert (YTV_IS_SOUP_FEED_FETCH_STRATEGY (self));
        
        YtvSoupFeedFetchStrategy* me = YTV_SOUP_FEED_FETCH_STRATEGY (self);
        YtvSoupFeedFetchStrategyPriv* priv =
                YTV_SOUP_FEED_FETCH_STRATEGY_GET_PRIVATE (me);
        SoupMessage* message;

        create_session (me);
        
        message = soup_message_new (SOUP_METHOD_GET, uri);

        if (message == NULL)
        {
                /* could not parse uri error */
                GError *err = NULL;
                g_set_error (&err, YTV_HTTP_ERROR, YTV_HTTP_ERROR_BAD_URI,
                             "Could not parse URI - %s", uri);
                return;
        }
        
        soup_message_set_flags (message, SOUP_MESSAGE_NO_REDIRECT);

        YtvCbWrapper* cbw = g_slice_new (YtvCbWrapper);
        cbw->st = self;
        cbw->cb = callback;

        soup_session_queue_message (priv->session, message,
                                    (SoupSessionCallback) retrieval_done,
                                    cbw);

        return;
}

static void
ytv_feed_fetch_strategy_init (YtvFeedFetchStrategyIface* klass)
{
	klass->perform = ytv_soup_feed_fetch_strategy_perform;

	return;
}

G_DEFINE_TYPE_EXTENDED (YtvSoupFeedFetchStrategy, ytv_soup_feed_fetch_strategy,
			G_TYPE_OBJECT, 0,
			G_IMPLEMENT_INTERFACE (YTV_TYPE_FEED_FETCH_STRATEGY,
					       ytv_feed_fetch_strategy_init))

static void
ytv_soup_feed_fetch_strategy_finalize (GObject *object)
{
        g_assert (YTV_IS_SOUP_FEED_FETCH_STRATEGY (object));

        YtvSoupFeedFetchStrategy* self = YTV_SOUP_FEED_FETCH_STRATEGY (object);
        YtvSoupFeedFetchStrategyPriv* priv =
                YTV_SOUP_FEED_FETCH_STRATEGY_GET_PRIVATE (self);

        if (priv->session != NULL)
        {
                soup_session_abort (priv->session);
                g_object_unref (priv->session);
                priv->session = NULL;
        }

        (*G_OBJECT_CLASS (ytv_soup_feed_fetch_strategy_parent_class)->finalize) (object);
        
        return;
}

static void
ytv_soup_feed_fetch_strategy_class_init (YtvSoupFeedFetchStrategyClass* klass)
{
	GObjectClass *g_klass;

	g_klass = G_OBJECT_CLASS (klass);
	
	klass->perform = ytv_soup_feed_fetch_strategy_perform_default;
	g_klass->finalize = ytv_soup_feed_fetch_strategy_finalize;

        g_type_class_add_private (klass, sizeof (YtvSoupFeedFetchStrategyPriv));
        
	return;
}

static void
ytv_soup_feed_fetch_strategy_init (YtvSoupFeedFetchStrategy* self)
{
        YtvSoupFeedFetchStrategyPriv* priv =
                YTV_SOUP_FEED_FETCH_STRATEGY_GET_PRIVATE (self);

        priv->session = NULL;
        
	return;
}

/**
 * ytv_soup_feed_fetch_strategy_new:
 *
 * Creates a new instance of the #YtvSoupFeedFetchStrategy which
 * implements the #YtvFeedFetchStrategy interface
 */
YtvFeedFetchStrategy*
ytv_soup_feed_fetch_strategy_new (void)
{
	YtvSoupFeedFetchStrategy* self = g_object_new
		(YTV_TYPE_SOUP_FEED_FETCH_STRATEGY, NULL);

	return YTV_FEED_FETCH_STRATEGY (self);
}
