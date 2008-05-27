/* ytv-error.c
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
 * SECTION: ytv-error
 * @title: YtvError
 * @short_description: Ytv errors under GError structure
 *
 * Definitions of the Ytv errors for GError handling
 */

#include <ytv-error.h>

/**
 * ytv_error_get_message:
 * @err: a #GError
 *
 * Get the error's message
 *
 * returns: an error's message
 */
const gchar*
ytv_error_get_message (GError* err)
{
        g_assert (err != NULL);
        
	return err->message;
}

/**
 * ytv_error_get_code:
 * @err: a #GError
 *
 * Get the error's code
 *
 * returns: an error's code
 */
gint
ytv_error_get_code (GError* err)
{
        g_assert (err != NULL);

	return err->code;
}

/**
 * ytv_error_domain_get_type:
 *
 * GType system helper function
 *
 * returns: a #GType
 */
GType
ytv_error_domain_get_type (void)
{
	static GType type = 0;

	if (type == 0)
	{
		static const GEnumValue values[] = {
			{ 0, NULL, NULL }
		};

		type = g_enum_register_static ("YtvErrorDomain", values);
	}

	return type;
}

/**
 * ytv_error_get_type:
 *
 * GType system helper function
 *
 * returns: a #GType
 */
GType
ytv_error_get_type (void)
{
	static GType type = 0;

	if (type == 0)
	{
		static const GEnumValue values[] = {
			{ 0, NULL, NULL }
		};

		type = g_enum_register_static ("YtvError", values);
	}

	return type;
}
