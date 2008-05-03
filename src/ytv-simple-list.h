#ifndef _YTV_SIMPLE_LIST_H_
#define _YTV_SIMPLE_LIST_H_

/* ytv-simple-list.h - An simple gobject list object
 * Copyright (C) 2006-2007 Philip Van Hoof <pvanhoof@gnome.org>
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

#define YTV_TYPE_SIMPLE_LIST             (ytv_simple_list_get_type ())
#define YTV_SIMPLE_LIST(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_SIMPLE_LIST, YtvSimpleList))
#define YTV_SIMPLE_LIST_CLASS(vtable)    (G_TYPE_CHECK_CLASS_CAST ((vtable), YTV_TYPE_SIMPLE_LIST, YtvSimpleListClass))
#define YTV_IS_SIMPLE_LIST(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_SIMPLE_LIST))
#define YTV_IS_SIMPLE_LIST_CLASS(vtable) (G_TYPE_CHECK_CLASS_TYPE ((vtable), YTV_TYPE_SIMPLE_LIST))
#define YTV_SIMPLE_LIST_GET_CLASS(inst)  (G_TYPE_INSTANCE_GET_CLASS ((inst), YTV_TYPE_SIMPLE_LIST, YtvSimpleListClass))

struct _YtvSimpleList 
{
	GObject parent;
};

struct _YtvSimpleListClass 
{
	GObjectClass parent;
};

GType ytv_simple_list_get_type (void);
YtvList* ytv_simple_list_new (void);

G_END_DECLS

#endif /* _YTV_SIMPLE_LIST_H_ */
