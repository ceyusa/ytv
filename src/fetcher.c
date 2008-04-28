/* fetcher.c - network data fetcher interface
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

#include <config.h>

#include "fetcher.h"

/* signals */
enum
{
	RESULT_FETCHED,
	LAST_SIGNAL
};

gint signals[LAST_SIGNAL];

static void
ytv_fetcher_base_init (gpointer g_class)
{
	static gboolean initialized = FALSE;

	if (!initialized)
	{
/**
 * YtvFetcher::result-fetched
 * @self: the object on which the signal is emitted
 * @user_data: (null-ok): user data set when the signal handler was connected.
 *
 * Emitted when the fetcher has received the data from the network
 *
 **/
	}
}

/**
 * ytv_fetcher_get_type:
 *
 * GType system herlper function
 *
 * returns: a #Gtype
 **/
GType
ytv_fetcher_get_type (void)
{
	static GType type = 0;

	if (G_UNLIKELY (type == 0))
	{
		static const GTypeInfo info =
		{
			ytv_fetcher_base_init,
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL,
			NULL
		};

		type = g_type_register_static (G_TYPE_INTERFACE,
					       "Fetcher", info, 0);

		g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
	}

	return type;
}

/**
 * ytv_feed_cat_get_type:
 *
 * GType system herlper function
 *
 * returns: a #Gtype
 **/
GType
ytv_feed_cat_get_type (void)
{
	static GType type = 0;

	if (G_UNLIKELY (type == 0))
	{
		static const GEnumValue values[] = {
			{ YTV_FEED_CAT_TOP_RATED, "Top Rated",
			  "Top rated videos" },
			{ YTV_FEED_CAT_TOP_FAVORITES, "Top Favorites",
			  "Top favorites videos" },
			{ YTV_FEED_CAT_MOST_VIEWED, "Most viewed",
			  "Most viewed videos" },
			{ YTV_FEED_CAT_MOST_DISCUSSED, "Must discussed",
			  "Most discussed videos" },
			{ YTV_FEED_CAT_MOST_LINKED, "Most linked",
			  "Most linked videos" },
			{ YTV_FEED_CAT_MOST_RESPONDED, "Most Responded",
			  "Most responded videos" },
			{ YTV_FEED_CAT_RECENTLY_FEATURED, "Recently featured",
			  "Recently featured videos" },
			{ YTV_FEED_CAT_WATCH_ON_MOBILE, "Watch on mobile",
			  "Watch on mobile" }
		};
		type = g_enum_register_static ("YtvFeedCat", values);
	}

	return type;
}
