/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-list.c - Interface for lists
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
 * SECTION: ytv-list
 * @title: YtvList
 * @short_description: A list type
 * @see_also: #YtvIterator
 *
 * The #YtvList is an iterable list type
 */

/**
 * YtvList:
 *
 * An iterable list type
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-list.h>

/**
 * ytv_list_get_length:
 * @self: An #YtvList instance
 *
 * Obtains the number of elements contained in the list
 *
 * Return value: the length of the list
 *
 **/
guint
ytv_list_get_length (YtvList *self)
{
        g_assert (YTV_IS_LIST (self));
        g_assert (YTV_LIST_GET_IFACE (self)->get_length_func != NULL);

        return YTV_LIST_GET_IFACE (self)->get_length_func (self);
}

/**
 * ytv_list_prepend:
 * @self: An #YtvList instance
 * @item: the item to prepend
 *
 * Prepends an item to a list. You can only prepend items that inherit from the
 * GObject base item. That's because the ytv list infrastructure does 
 * reference counting. It effectively means that indeed you can't use non 
 * GObject types in a ytv list. But there's not a single situation where 
 * you must do that. If you must store a non GObject in a list, you shouldn't
 * use the ytv infrastructure for this. Consider using a doubly linked list
 * or a pointer array or any other list-type available on your development
 * platform.
 *
 * However, ytv lists can cope with any valid GObject. Not just the 
 * GObjects implemented by the ytv framework.
 *
 * All reference handling in ytv is reference neutral. Also the lists. This
 * means that if your plan is to reparent the item to the list, that you should
 * take care of that by, after prepending or appending it, unreferencing it to 
 * get rid of its initial reference. If you don't want to reparent, but you do
 * want to destroy your item once removed from the list, then you must 
 * unreference your items twice. Note that reparenting is highly recommended
 * in most such cases (because it's a much cleaner way). However, if reparented
 * and the list itself gets destroyed, then the item will also get unreferenced.
 *
 * Reparenting indeed means reparenting. Okay? Loosing your parent reference
 * means loosing your reason of existance. So you'll get destroyed.
 *
 * Implementers: if you have to choose, make this one the fast one
 *
 **/
void
ytv_list_prepend (YtvList *self, GObject* item)
{
        g_assert (YTV_IS_LIST (self));
        gint length = ytv_list_get_length (self);
        g_assert (item);
        g_assert (G_IS_OBJECT (item));
        g_assert (YTV_LIST_GET_IFACE (self)->prepend_func != NULL);

        YTV_LIST_GET_IFACE (self)->prepend_func (self, item);

        g_assert (ytv_list_get_length (self) == length + 1);

        return;
}

/**
 * ytv_list_append:
 * @self: An #YtvList instance
 * @item: the item to append
 *
 * Appends an item to a list. You can only append items that inherit from the
 * GObject base item. That's because the ytv list infrastructure does 
 * reference counting. It effectively means that indeed you can't use non 
 * GObject types in a ytv list. But there's not a single situation where 
 * you must do that. If you must store a non GObject in a list, you shouldn't
 * use the ytv infrastructure for this. Consider using a doubly linked list
 * or a pointer array or any other list-type available on your development
 * platform.
 *
 * However, ytv lists can cope with any valid GObject. Not just the 
 * GObjects implemented by the ytv framework.
 *
 * All reference handling in ytv is reference neutral. Also the lists. This
 * means that if your plan is to reparent the item to the list, that you should
 * take care of that by, after prepending or appending it, unreferencing it to 
 * get rid of its initial reference. If you don't want to reparent, but you do
 * want to destroy your item once removed from the list, then you must 
 * unreference your items twice. Note that reparenting is highly recommended
 * in most such cases (because it's a much cleaner way). However, if reparented
 * and the list itself gets destroyed, then the item will also get unreferenced.
 *
 * Reparenting indeed means reparenting. Okay? Loosing your parent reference
 * means loosing your reason of existance. So you'll get destroyed.
 *
 * Implementers: if you have to choose, make the prepend one the fast one
 *
 **/
void 
ytv_list_append (YtvList *self, GObject* item)
{
        g_assert (YTV_IS_LIST (self));
        gint length = ytv_list_get_length (self);
        g_assert (item);
        g_assert (G_IS_OBJECT (item));
        g_assert (YTV_LIST_GET_IFACE (self)->append_func != NULL);

        YTV_LIST_GET_IFACE (self)->append_func (self, item); 

        g_assert (ytv_list_get_length (self) == length + 1);

        return;
}

/**
 * ytv_list_remove:
 * @self: An #YtvList instance
 * @item: the item to remove
 *
 * Removes an item from a list.  Removing a item might invalidate all existing
 * iterators or put them in an unknown and unspecified state. You'll need to 
 * recreate the iterator(s) if you remove an item to be certain.
 *
 * If you want to clear a list, consider using the ytv_list_foreach or simply
 * destroy the list instance and construct a new one. If you want to remove
 * specific items from a list, consider using a second list. You should not
 * attempt to remove items from a list while an (any) iterator is active on the
 * same list.
 *
 * Example (removing even items):
 * <informalexample><programlisting>
 * YtvList *toremovefrom = ...
 * YtvList *removethese = ytv_simple_list_new ();
 * YtvIterator *iter = ytv_list_create_iterator (toremovefrom);
 * int i = 0;
 * while (!ytv_iterator_is_done (iter))
 * {
 *      if (i % 2 == 0)
 *      {
 *           GObject *obj = ytv_iterator_get_current (iter);
 *           ytv_list_prepend (removethese, obj);
 *           g_object_unref (G_OBJECT (obj));
 *      }
 *      i++;
 *      ytv_iterator_next (iter);
 * }
 * g_object_unref (G_OBJECT (iter));
 * iter = ytv_list_create_iterator (removethese);
 * while (!ytv_iterator_is_done (iter))
 * {
 *      GObject *obj = ytv_iterator_get_current (iter);
 *      ytv_list_remove (toremovefrom, obj);
 *      g_object_unref (G_OBJECT (obj));
 *      ytv_iterator_next (iter);
 * }
 * g_object_unref (G_OBJECT (iter));
 * g_object_unref (G_OBJECT (removethese));
 * g_object_unref (G_OBJECT (toremovefrom));
 * </programlisting></informalexample>
 *
 * There's no guarantee whatsoever that existing iterators of @self will be
 * valid after this method returned. 
 *
 * Note that if you didn't remove the initial reference when putting the item
 * in the list, this remove will not take of that initial reference either. 
 *
 **/
void 
ytv_list_remove (YtvList *self, GObject* item)
{
        g_assert (YTV_IS_LIST (self));
        gint nl, length = ytv_list_get_length (self);
        g_assert (item);
        g_assert (G_IS_OBJECT (item));
        g_assert (YTV_LIST_GET_IFACE (self)->remove_func != NULL);

        YTV_LIST_GET_IFACE (self)->remove_func (self, item);

        nl = ytv_list_get_length (self);
        g_assert (nl == length || nl == length - 1);

        return;
}

/**
 * ytv_list_create_iterator:
 * @self: An #YtvList instance
 *
 * Creates a new iterator instance for the list. The initial position
 * of the iterator is the first element.
 *
 * An iterator is a position indicator for a list. It keeps the position
 * state of a list iteration. The list itself does not keep any position 
 * information. Consuming multiple iterator instances makes it possible to
 * have multiple list iterations simultanously (i.e. multiple threads or in
 * in a loop that simultanously works with multiple position states in a
 * single list).
 *
 * Example:
 * <informalexample><programlisting>
 * YtvList *list = ytv_simple_list_new ();
 * YtvIterator *iter1 = ytv_list_create_iterator (list);
 * YtvIterator *iter2 = ytv_list_create_iterator (list);
 * while (!ytv_iterator_is_done (iter1))
 * {
 *      while (!ytv_iterator_is_done (iter2))
 *            ytv_iterator_next (iter2);
 *      ytv_iterator_next (iter1);
 * }
 * g_object_unref (G_OBJECT (iter1));
 * g_object_unref (G_OBJECT (iter2));
 * g_object_unref (G_OBJECT (list));
 * </programlisting></informalexample>
 *
 * The reason why the method isn't called get_iterator is because it's a
 * object creation method (a factory method). It's not a property. It
 * effectively creates a new instance of an iterator. The returned iterator
 * object should (therefore) be unreferenced after use.
 * 
 * Implementers: For custom lists you must create a private iterator type and
 * return a new instance of it. You shouldn't make the internal API of that
 * type public.
 *
 * The developer will always only use the YtvIterator interface API on
 * instances of your type. You must therefore return your private iterator
 * type, that implements YtvIterator, here.
 *
 * Return value: A new iterator for the list @self
 *
 **/
YtvIterator* 
ytv_list_create_iterator (YtvList *self)
{
        YtvIterator *retval = NULL;

        g_assert (YTV_IS_LIST (self));
        g_assert (YTV_LIST_GET_IFACE (self)->create_iterator_func != NULL);

        retval = YTV_LIST_GET_IFACE (self)->create_iterator_func (self);

        g_assert (YTV_IS_ITERATOR (retval));

        return retval;
}

/**
 * ytv_list_foreach:
 * @self: An #YtvList instance
 * @func: the function to call with each element's data.
 * @user_data: user data to pass to the function.
 *
 * Calls a function for each element in a #YtvList. It will use an internal
 * iteration which you don't have to worry about. 
 *
 * Example:
 * <informalexample><programlisting>
 * static void
 * list_foreach_item (YtvHeader *header, gpointer user_data)
 * {
 *      g_print ("%s\n", ytv_header_get_subject (header));
 * }
 * </programlisting></informalexample>
 *
 * <informalexample><programlisting>
 * YtvFolder *folder = ...
 * YtvList *headers = ytv_simple_list_new ();
 * ytv_folder_get_headers (folder, headers, FALSE);
 * ytv_list_foreach (headers, list_foreach_item, NULL);
 * g_object_unref (G_OBJECT (list));
 * </programlisting></informalexample>
 *
 * The purpose of this method is to have a fast foreach iteration. Using this
 * is faster than inventing your own foreach loop using the is_done and next
 * methods. The order is guaranteed to be the first element first, the last 
 * element last. If during the iteration you don't remove items, it's guaranteed
 * that all current items will be iterated.
 *
 * In the func implementation and during the foreach operation you shouldn't
 * append, remove nor prepend items to the list. In multithreaded environments
 * it's advisable to introduce a lock when using this functionality. 
 *
 **/
void 
ytv_list_foreach (YtvList *self, GFunc func, gpointer user_data)
{
        g_assert (YTV_IS_LIST (self));
        g_assert (func);
        g_assert (YTV_LIST_GET_IFACE (self)->foreach_func != NULL);

        YTV_LIST_GET_IFACE (self)->foreach_func (self, func, user_data);
        return;
}


/**
 * ytv_list_copy:
 * @self: An #YtvList instance
 *
 * Creates a shallow copy of the list. It doesn't copy the items. It,
 * however, creates a new list with new references to the same
 * items. The items will get an extra reference added for the new list
 * being their second parent, setting their reference count to for
 * example two. Which means that both lists (the original and the
 * copy) must be unreferenced after use.
 *
 * Return value: A copy of this list
 *
 **/
YtvList*
ytv_list_copy (YtvList *self)
{
        YtvList *retval = NULL;

        g_assert (YTV_IS_LIST (self));
        g_assert (YTV_LIST_GET_IFACE (self)->copy_func != NULL);

        retval = YTV_LIST_GET_IFACE (self)->copy_func (self);

        g_assert (YTV_IS_LIST (retval));

        return retval;
}

static void
ytv_list_base_init (gpointer g_class)
{
        static gboolean initialized = FALSE;

        if (!initialized)
        {
                /* create interface signals here. */
                initialized = TRUE;
        }
}

GType
ytv_list_get_type (void)
{
        static GType type = 0;

        if (G_UNLIKELY(type == 0))
        {
                static const GTypeInfo info = 
                {
                  sizeof (YtvListIface),
                  ytv_list_base_init,   /* base_init */
                  NULL,   /* base_finalize */
                  NULL,   /* class_init */
                  NULL,   /* class_finalize */
                  NULL,   /* class_data */
                  0,
                  0,      /* n_preallocs */
                  NULL,   /* instance_init */
                  NULL
                };
                type = g_type_register_static (G_TYPE_INTERFACE, 
                                               "YtvList", &info, 0);
        }

        return type;
}
