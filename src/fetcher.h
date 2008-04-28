#ifndef YTV_FETCHER_H
#define YTV_FETCHER_H

/* fetcher.h - network data fetcher interface
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

#include <glib-object.h>

G_BEGIN_DECLS

#define YTV_TYPE_FETCHER			\
	(ytv_account_get_type ())
#define YTV_FETCHER(obj) 			\
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_FETCHER, YtvFetcher))
#define YTV_IS_FETCHER(obj)			\
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_FETCHER))
#define YTV_FETCHER_GET_IFACE(inst)		\
	(G_TYPE_INSTANCE_GET_INTERFACE ((inst), YTV_TYPE_FETCHER, YtvFetcherIface))

typedef struct _YtvFetcher YtvFetcher;
typedef struct _YtvFetcherIface YtvFetcherIfacer;

#define YTV_TYPE_FEED_CAT (ytv_feed_cat_get_type ())

enum _YtvFeedCat
{
	YTV_FEED_CAT_TOP_RATED,
	YTV_FEED_CAT_TOP_FAVORITES,
	YTV_FEED_CAT_MOST_VIEWED,
	YTV_FEED_CAT_MOST_DISCUSSED,
	YTV_FEED_CAT_MOST_LINKED,
	YTV_FEED_CAT_MOST_RESPONDED,
	YTV_FEED_CAT_RECENTLY_FEATURED,
	YTV_FEED_CAT_WATCH_ON_MOBILE
};

typedef enum _YtvFeedCat YtvFeedCat;

struct _YtvFetcherIface
{
	GTypeInterface parent;

	void (*search) (YtvFetcher *self, const gchar* query);
	void (*get_feed) (YtvFetcher *self, YtvFeedCat category);
	void (*get_video) (YtvFetcher *self, const gchar* vid);

	/* signals */
	void (*result_fetched) (YtvFetcher *self, GError **error);

	
};

GType ytv_feed_cat_get_type ();
GType ytv_fetcher_get_type ();

void ytv_fetcher_search (const gchar* query);
void ytv_fetcher_get_feed (YtvFeedCat category);
void ytv_fetcher_get_video (const gchar* vid);
	
G_END_DECLS

#endif /* YTV_FETCHER_H */
