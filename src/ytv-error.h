#ifndef _YTV_ERROR_H_
#define _YTV_ERROR_H_

/* ytv-error.h
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

#define YTV_TYPE_ERROR_DOMAIN (ytv_error_domain_get_type ())

enum _YtvErrorDomain
{
	YTV_HTTP_ERROR = 1,
	YTV_PARSE_ERROR = 2,
};

typedef enum _YtvErrorDomain YtvErrorDomain;

#define YTV_TYPE_ERROR (ytv_error_get_type ())

/**
 * YtvError:
 *
 * A GError error code.
 */
enum _YtvError
{
	YTV_NO_ERROR,

	YTV_HTTP_ERROR_CONNECTION,
	YTV_HTTP_ERROR_BAD_URI,

	YTV_PARSE_ERROR_BAD_FORMAT,
};

typedef GError YError;

typedef enum _YtvError YtvError;

const gchar* ytv_error_get_message (GError *err);
gint ytv_error_get_code (GError *err);

GType ytv_error_domain_get_type (void);
GType ytv_error_get_type (void);

#endif /* _YTV_ERROR_H_ */
