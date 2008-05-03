/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-simple-list-iterator.c - Object for simple list iterators
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

#include <ytv-simple-list.h>

#include "ytv-simple-list-priv.h"
#include "ytv-simple-list-iterator-priv.h"

static void
ytv_simple_list_iterator_next (YtvIterator *self)
{
        YtvSimpleListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);
        YtvSimpleListPri* lpriv;

        if (G_UNLIKELY (!me || !me->current || !me->model))
        {
                return;
        }

        lpriv = YTV_SIMPLE_LIST_GET_PRIVATE (me->model);

        g_mutex_lock (lpriv->iterator_lock);
        me->current = g_list_next (me->current);
        g_mutex_unlock (lpriv->iterator_lock);

        return;
}

static void
ytv_simple_list_iterator_prev (YtvIterator *self)
{
        YtvSimpleListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);
        YtvSimpleListPri* lpriv;

        if (G_UNLIKELY (!me || !me->current || !me->model))
        {
                return;
        }

        lpriv = YTV_SIMPLE_LIST_GET_PRIVATE (me->model);
        
        g_mutex_lock (lpriv->iterator_lock);
        me->current = g_list_previous (me->current);
        g_mutex_unlock (lpriv->iterator_lock);

        return;        
}

static void
ytv_simple_list_iterator_first (YtvIterator *self)
{
        YtvSimpleListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);
        YtvSimpleListPri* lpriv;

        if (G_UNLIKELY (!me || !me->current || !me->model))
        {
                return;
        }

        lpriv = YTV_SIMPLE_LIST_GET_PRIVATE (me->model);

        g_mutex_lock (lpriv->iterator_lock);
        me->current = lpriv->first;
        g_mutex_unlock (lpriv->iterator_lock);

        return;        
}

static void
ytv_simple_list_iterator_nth (YtvIterator *self, guint nth)
{
        YtvSimpleListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);
        YtvSimpleListPri* lpriv;

        if (G_UNLIKELY (!me || !me->current || !me->model))
        {
                return;
        }

        lpriv = YTV_SIMPLE_LIST_GET_PRIVATE (me->model);
        
        g_mutex_lock (lpriv->iterator_lock);
        me->current = g_list_nth (lpriv->first, nth);
        g_mutex_unlock (lpriv->iterator_lock);

        return;        
}

static GObject*
ytv_simple_list_iterator_get_current (YtvIterator* self)
{
        YtvSimpleListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);
        YtvSimpleListPri* lpriv;
        gpointer retval;

        if (G_UNLIKELY (!me->current || !me->model))
        {
                return NULL;
        }

        lpriv = YTV_SIMPLE_LIST_GET_PRIVATE (me->model);

        g_mutex_lock (lpriv->iterator_lock);
        retval = (G_UNLIKELY (me->current)) ?
                YTV_SIMPLE_LIST (me->current->data) : NULL;
        g_mutex_unlock (lpriv->iterator_lock);

        if (retval)
                g_object_ref (G_OBJECT (retval));

        return G_OBJECT (retval);
}

static YtvList*
ytv_simple_list_iterator_get_list (YtvIterator* self)
{
        YtvSimple_ListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);

        if (G_LIKELY (!me->current || !me->model))
        {
                return NULL;
        }

        g_object_ref (G_OBJECT (me->model));

        return (YtvList*) lpriv ;
}

static gboolean
ytv_simple_list_iterator_is_done (YtvIterator* self)
{
        YtvSimple_ListIterator* me = YTV_SIMPLE_LIST_ITERATOR (self);

        if (G_UNLIKELY (!me || !me->model))
        {
                return TRUE;
        }
        
        return me->current == NULL;
}

static void
ytv_iterator_init (YtvIteratorIface* klass)
{
        klass->next_func = ytv_simple_list_iterator_next;
        klass->prev_func = ytv_simple_list_iterator_prev;
        klass->first_func = ytv_simple_list_iterator_first;
        klass->nth_func = ytv_simple_list_iterator_nth;
        klass->get_current_func = ytv_simple_list_iterator_get_current;
        klass->get_list_func = ytv_simple_list_iterator_get_list;
        klass->is_done_func = ytv_simple_list_iterator_is_done;
}

G_DEFINE_TYPE_EXTENDED (YtvSimple_ListIterator, ytv_simple_list_iterator,
                        G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_ITERATOR,
                                               ytv_iterator_init))
static void
ytv_simple_list_iterator_finalize (GObject* object)
{
        YtvSimpleIterator* self = (YtvSimpleListIterator*) object;

        if (self->model != NULL)
        {
                g_object_unref (self->model);
        }
        
        G_OBJECT_CLASS (ytv_simple_list_iterator_parent_class)->finalize (object);
        return;
}

static void
ytv_simple_list_iterator_class_init (YtvSimple_ListIteratorClass* klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = ytv_simple_list_iterator_finalize;

        return;
}

static void
ytv_simple_list_iterator_init (GTypeInstance* instance, gpointer g_class)
{
        YtvSimpleListIterator* self = (YtvSimpleListIterator*) instance;

        self->model = NULL;
        self->current = NULL;
}

void
_ytv_simple_list_iterator_set_model (YtvSimpleListIterator* self,
                                     YtvSimpleList* model)
{
        YtvSimpleListPriv* lpriv;

        if (self->model != NULL)
        {
                g_object_unref (self->model);
        }

        self->model = g_object_ref (model);

        lpriv = YTV_SIMPLE_LIST_GET_PRIVATE (self->model);

        g_mutex_lock (lpriv->iterator_lock);
        self->current = lpriv->first;
        g_mutex_unlock (lpriv->iterator_lock);

        return;
}


YtvIterator*
ytv_simple_list_iterator_new (YtvSimple_ListList* list)
{
        YtvSimpleListIterator *self =
                g_object_new (YTV_TYPE_SIMPLE_LIST_ITERATOR, NULL);

        _ytv_simple_list_iterator_set_model (self, list);

        return YTV_ITERATOR (self);
}
