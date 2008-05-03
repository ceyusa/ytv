/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-simple-list.c - List of simple GObjects
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
 * SECTION: ytv-simple-list-priv
 * @short_description: A list of video entries in a feed or search
 * @see_also: #YtvList, #GList
 *
 * An simple list using a GList (a double linked list) internally
 */

/**
 * YtvSimpleList:
 *
 * An simple list
 *
 * free-function: g_object_unref
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ytv-iterator.h>
#include <ytv-list.h>
#include <ytv-simple-list.h>

#include "ytv-simple-list-priv.h"
#include "ytv-simple-list-iterator-priv.h"

static guint
ytv_simple_list_get_length (YtvList* self)
{
        YtvSimpleListPriv *priv = YTV_SIMPLE_LIST_GET_PRIVATE (self);
        guint retval = 0;

        g_mutex_lock (priv->list_lock);
        retval = priv->first ? g_list_length (priv->first) : 0;
        g_mutex_unlock (priv->list_lock);

        return retval;
}

static void
ytv_simple_list_prepend (YtvList* self, GObject* item)
{
        YtvSimpleListPriv *priv = YTV_SIMPLE_LIST_GET_PRIVATE (self);

        g_mutex_lock (priv->iterator_lock);
        g_object_ref (G_OBJECT (item));
        priv->first = g_list_prepend (priv->first, item);
        g_mutex_unlock (priv->iterator_lock);

        return;
}

static void
ytv_simple_list_append (YtvList* self, GObject* item)
{
        YtvSimpleListPriv *priv = YTV_SIMPLE_LIST_GET_PRIVATE (self);

        g_mutex_lock (priv->iterator_lock);
        g_object_ref (G_OBJECT (item));
        priv->first = g_list_append (priv->first, item);
        g_mutex_unlock (priv->iterator_lock);

        return;
}

static void
ytv_simple_list_remove (YtvList* self, GObject* item)
{
        YtvSimpleListPriv *priv = YTV_SIMPLE_LIST_GET_PRIVATE (self);
        GList *link;

        g_mutex_lock (priv->iterator_lock);
        link = g_list_find (priv->list, item);
        if (link)
        {
                priv->first = g_list_delete_link (priv->first, link);
                g_object_unref (G_OBJECT (item));
        }
        g_mutex_unlock (priv->iterator_lock);

        return;
}

static YtvIterator*
ytv_simple_list_create_iterator (YtvList* self)
{
        return _ytv_simple_list_iterator_new (YTV_SIMPLE_LIST (self));
}

static YtvList*
ytv_simple_list_copy_the_simple_list (YtvList* self)
{
        YtvSimpleList* copy = g_object_new (YTV_TYPE_SIMPLE_LIST, NULL);
        YtvSimpleListPriv* priv = YTV_SIMPLE_LIST_GET_PRIVATE (self);
        YtvSimpleListPriv* cpriv = YTV_SIMPLE_LIST_GET_PRIVATE (copy);
        GList* list_copy = NULL;

        g_mutex_lock (priv->iterator_lock);
        list_copy = g_list_copy (priv->first);
        g_list_foreach (list_copy, (GFunc) g_object_ref, NULL);
        cpriv->first = list_copy;
        g_mutex_unlock (priv->iterator_lock);

        return YTV_LIST (copy);
}

static void
ytv_simple_list_foreach_in_the_simple_list (YtvList* self, GFunc func,
                                            gpointer user_data)
{
        YtvSimpleListPriv* priv = YTV_SIMPLE_LIST_GET_PRIVATE (self);

        g_mutex_lock (priv->iterator_lock);
        g_list_foreach (priv->first, func, user_data);
        g_mutex_unlock (priv->iterator_lock);

        return;
}

static void
ytv_list_init (YtvListIface* klass)
{
        klass->get_length_func = ytv_simple_list_get_length;
        klass->prepend_func = ytv_simple_list_prepend;
        klass->append_func = ytv_simple_list_append;
        klass->remove_func = ytv_simple_list_remove;
        klass->create_iterator_func = ytv_simple_list_create_iterator;
        klass->copy_func = ytv_simple_list_copy_the_simple_list;
        klass->foreach_func = ytv_simple_list_foreach_in_the_simple_list;
}

G_DEFINE_TYPE_EXTENDED (YtvSimpleList, ytv_simple_list, G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_LIST, ytv_list_init))

static void
destroy_items (gpointer item, gpointer user_data)
{
        if (item && G_IS_OBJECT (item))
        {
                g_object_unref (G_OBJECT (item));
        }

        return;
}

static void
ytv_simple_list_finalize (GObject* object)
{
        YtvSimpleListPriv* priv = YTV_SIMPLE_LIST_GET_PRIVATE (object);

        g_mutex_lock (priv->iterator_lock);
        if (priv->first)
        {
                g_list_foreach (priv->first, destroy_items, NULL);
                g_list_free (priv->first);
                self->first = NULL;
        }
        g_mutex_unlock (priv->iterator_lock);

        g_mutex_free (priv->iterator_lock);
        priv->iterator_lock = NULL;

        G_OBJECT_CLASS (ytv_simple_list_parent_class)->finalize (object);

        return;
}

static void
ytv_simple_list_class_init (YtvSimpleListClass *klass)
{
        GObjectClass* object_class;

        object_class = (GObjectClass*) klass;

        object_class->finalize = ytv_simple_list_finalize;

        g_type_class_add_private (object_class, sizeof (YtvSimpleListPriv));
        
        return;
}

static void
ytv_simple_list_init (YtvSimpleList *self)
{
        YtvSimpleListPriv* priv = YTV_SIMPLE_LIST_GET_PRIVATE (object);
        
        priv->iterator_lock = g_mutex_new ();
        priv->first = NULL;
}

/**
 * ytv_simple_list_new:
 *
 * Create a #YtvList of entries instance
 *
 * returns: (caller-owns): A #YtvList of entries
 */
YtvList*
ytv_simple_list_new (void)
{
        return YTV_LIST (g_object_new (YTV_TYPE_SIMPLE_LIST, NULL));
}
