/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-iterator.c - Interface for iterators
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-iterator.h>
#include <ytv-list.h>

/**
 * ytv_iterator_next:
 * @self: An #YtvIterator instance
 *
 * Moves the iterator to the next node
 *
 **/
void
ytv_iterator_next (YtvIterator *self)
{
        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->next_func != NULL);
        
        YTV_ITERATOR_GET_IFACE (self)->next_func (self);
        
        return;
}

/**
 * ytv_iterator_prev:
 * @self: An #YtvIterator instance
 *
 * Moves the iterator to the previous node
 *
 **/
void
ytv_iterator_prev (YtvIterator *self)
{
        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->prev_func != NULL);
        
        YTV_ITERATOR_GET_IFACE (self)->prev_func (self);
        
        return;        
}

/**
 * ytv_iterator_first:
 * @self: An #YtvIterator instance
 *
 * Moves the iterator to the first node
 *
 **/
void
ytv_iterator_first (YtvIterator *self)
{
        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->first_func != NULL);
        
        YTV_ITERATOR_GET_IFACE (self)->first_func (self);
        
        return;        
}

/**
 * ytv_iterator_nth:
 * @self: An #YtvIterator instance
 * @nth: The nth position
 *
 * Moves the iterator to the nth node
 *
 **/
void
ytv_iterator_nth (YtvIterator *self, guint nth)
{
        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->nth_func != NULL);
        
        YTV_ITERATOR_GET_IFACE (self)->nth_func (self, nth);
        
        return;        
}

/**
 * ytv_iterator_get_current:
 * @self: An #YtvIterator instance
 *
 * Does not move the iterator. Returns the objecto at the curren position. If
 * there's no current position, this method returns NULL. If not NULL, the
 * returned value must be unreferenced after use.
 *
 * Return value: the current object or NULL
 *
 **/
GObject*
ytv_iterator_get_current (YtvIterator *self)
{
        YtvObject *retval;

        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->get_current_func != NULL);

        retval = YTV_ITERATOR_GET_IFACE (self)->get_current_func (self);

        if (retval)
                g_assert (G_IS_OBJECT (retval));

        return retval;
}

/**
 * ytv_iterator_is_done:
 * @self: An #YtvIterator instance
 *
 * Does the iterator point to some valid list item? You can use this property
 * to make loops like:
 * 
 * Example:
 * <informalexample><programlisting>
 * YtvList *list = ytv_simple_list_new ();
 * YtvIterator *iter = ytv_list_create_iterator (list);
 * while (!ytv_iterator_is_done (iter))
 * {
 *    GObject *cur = ytv_iterator_get_current (iter);
 *    ...
 *    g_object_unref (cur);
 *    ytv_iterator_next (iter);
 * }
 * g_object_unref (G_OBJECT (iter));
 * g_object_unref (G_OBJECT (list));
 * </programlisting></informalexample>
 *
 * Return value: TRUE if it points to a valid list item, FALSE otherwise
 *
 **/
gboolean
ytv_iterator_is_done (YtvIterator *self)
{
        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->is_done_func != NULL);

        return YTV_ITERATOR_GET_IFACE (self)->is_done_func (self);
}

/**
 * ytv_iterator_get_list:
 * @self: An #YtvIterator instance
 *
 * Does not move the iterator. Returns the list of which this iterator is an
 * iterator. The returned list object should be unreferenced after use.
 * Remember when using this property that lists shouldn't change while
 * iterating them.
 *
 * Return value: The #YtvList instance being iterated
 *
 **/
YtvList*
ytv_iterator_get_list (YtvIterator *self)
{
        YtvList *retval = NULL;

        g_assert (YTV_IS_ITERATOR (self));
        g_assert (YTV_ITERATOR_GET_IFACE (self)->get_list_func != NULL);

        retval = YTV_ITERATOR_GET_IFACE (self)->get_list_func (self);

        g_assert (YTV_IS_LIST (retval));

        return retval;
}

static void
ytv_iterator_base_init (gpointer g_class)
{
        static gboolean initialized = FALSE;

        if (!initialized)
        {
                initialized = TRUE;
        }
}

GType
ytv_iterator_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY (type == 0))
        {
                static const GTypeInfo info =
                {
                        sizeof (YtvIteratorIface),
                        ytv_iterator_base_init,
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
                                               "YtvIterator", &info, 0);
        }

        return type;
}
