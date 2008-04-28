/* fetch-soup.c - proof of concept for a HTTP client for YT using
 *                libsoup
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

#include <libsoup/soup.h>

typedef struct _Http Http;
struct _Http
{
	SoupSession *session;
	SoupURI *uri;
	GMainLoop *loop;
};
	
Http*
http_init ()
{
	Http* me = g_new0 (Http, 1);
	gchar* base = "http://gdata.youtube.com/feeds/api/videos?max-results=3&vq=muse+invincible&alt=json";
	me->uri = soup_uri_new (base);

	if (!me->uri)
	{
		g_warning ("Could not parse '%s' as an URL\n", base);
		g_free (me);
		return NULL;
	}

	me->session = soup_session_async_new ();

	me->loop = NULL;

	return me;
}

void
http_destroy (Http* self)
{
	g_assert (self != NULL);

	if (G_LIKELY (self->uri))
	{
		soup_uri_free (self->uri);
		self->uri = NULL;
	}

	if (G_LIKELY (self->session))
	{
		soup_session_abort (self->session);
		g_object_unref (self->session);
		self->session = NULL;
	}

	g_free (self);
}

void
retrieval_done (SoupSession* session, SoupMessage* msg, Http* self)
{
	g_assert (self != NULL);
	
	/* debug */
	{
		const gchar *name, *value;
		gchar *path;
		SoupMessageHeadersIter iter;
		
		g_print ("Current path = %s\n",
			 soup_message_get_uri (msg)->path);
		path = soup_uri_to_string (soup_message_get_uri (msg), TRUE);
		g_print ("%s %s HTTP/1.%d\n\n", SOUP_METHOD_GET, path,
			 soup_message_get_http_version (msg));
		g_print ("HTTP/1.%d %d %s\n",
			 soup_message_get_http_version (msg),
			 msg->status_code, msg->reason_phrase);
		
		soup_message_headers_iter_init (&iter, msg->response_headers);
		while (soup_message_headers_iter_next (&iter, &name, &value))
			g_print ("%s: %s\r\n", name, value);
		g_print ("\n");
	}

	if (!SOUP_STATUS_IS_SUCCESSFUL (msg->status_code))
		return;

	g_print ("%s", msg->response_body->data);

	g_main_loop_quit (self->loop);

	return;
}

gboolean
retrieval_begin (Http* self)
{
	g_assert (self != NULL);

	SoupMessage *msg;

	msg = soup_message_new_from_uri (SOUP_METHOD_GET, self->uri);
	soup_message_set_flags (msg, SOUP_MESSAGE_NO_REDIRECT);

	soup_session_queue_message (self->session, msg,
				    (SoupSessionCallback) retrieval_done,
				    self);

	return FALSE;
}

gint
main (gint argc, gchar **argv)
{
	g_thread_init (NULL);
	g_type_init ();

	Http *http = http_init ();

	http->loop = g_main_loop_new (NULL, TRUE);

	g_idle_add ((GSourceFunc) retrieval_begin, http);

	g_main_loop_run (http->loop);
	
	g_main_loop_unref (http->loop);

	http_destroy (http);

	return 0;
}
