/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

/* ytv-entry-iterator.c - Object for entry iterators
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

#include <ytv-entry.h>
#include <ytv-iterator.h>
#include <ytv-entry-iterator.h>

static void
ytv_entry_iterator_next (YtvIterator *self)
{
        g_assert (YTV_IS_ENTRY_ITERATOR (self));
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);

        if (G_UNLIKELY (!me || !me->current || !me->list))
        {
                return;
        }

        g_mutex_lock (me->list->list_lock);
        me->current = g_list_next (me->current);
        g_mutex_unlock (me->list->list_lock);

        return;
}

static void
ytv_entry_iterator_prev (YtvIterator *self)
{
        g_assert (YTV_IS_ENTRY_ITERATOR (self));
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);

        if (G_UNLIKELY (!me || !me->current || !me->list))
        {
                return;
        }

        g_mutex_lock (me->list->list_lock);
        me->current = g_list_previous (me->current);
        g_mutex_unlock (me->list->list_lock);

        return;        
}

static void
ytv_entry_iterator_first (YtvIterator *self)
{
        g_assert (YTV_IS_ENTRY_ITERATOR (self));
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);

        if (G_UNLIKELY (!me || !me->current || !me->list))
        {
                return;
        }

        g_mutex_lock (me->list->list_lock);
        me->current = me->list->first;
        g_mutex_unlock (me->list->list_lock);

        return;        
}

static void
ytv_entry_iterator_nth (YtvIterator *self, guint nth)
{
        g_assert (YTV_IS_ENTRY_ITERATOR (self));
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);

        if (G_UNLIKELY (!me || !me->current || !me->list))
        {
                return;
        }

        g_mutex_lock (me->list->list_lock);
        me->current = g_list_nth (me->list->first, nth);
        g_mutex_unlock (me->list->list_lock);

        return;        
}

static YtvObject*
ytv_entry_iterator_get_current (YtvIterator* self)
{
        g_assert (YTV_IS_ENTRY_ITERATOR (self));
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);
        YtvEntry *entry = NULL;

        if (G_UNLIKELY (!me->current || !me->list))
        {
                return NULL;
        }

        g_mutex_lock (me->list->list_lock);
        entry = (G_UNLIKELY (me->current)) ?
                YTV_ENTRY (me->current->data) : NULL;
        g_mutex_unlock (me->list->list_lock);

        if (entry)
                g_object_ref (G_OBJECT (entry));

        return YTV_OBJECT (entry);
}

static YtvList*
ytv_entry_iterator_get_list (YtvIterator* self)
{
        g_assert (YTV_IS_ENTRY_ITERATOR (self));
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);

        if (G_LIKELY (!me->current || !me->list))
        {
                return NULL;
        }

        g_object_ref (G_OBJECT (me->list));

        return (YtvList*) me->list ;
}

static gboolean
ytv_entry_iterator_is_done (YtvIterator* self)
{
        YtvEntryIterator* me = YTV_ENTRY_ITERATOR (self);

        if (G_UNLIKELY (!me || !me->list))
        {
                return TRUE;
        }
        
        return me->current == NULL;
}

static void
ytv_iterator_init (YtvIteratorIface* klass)
{
        klass->next_func = ytv_entry_iterator_next;
        klass->prev_func = ytv_entry_iterator_prev;
        klass->first_func = ytv_entry_iterator_first;
        klass->nth_func = ytv_entry_iterator_nth;
        klass->get_current_func = ytv_entry_iterator_get_current;
        klass->get_list_func = ytv_entry_iterator_get_list;
        klass->is_done_func = ytv_entry_iterator_is_done;
}

G_DEFINE_TYPE_EXTENDED (YtvEntryIterator, ytv_entry_iterator,
                        G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_ITERATOR,
                                               ytv_iterator_init))
static void
ytv_entry_iterator_finalize (GObject* object)
{
        G_OBJECT_CLASS (ytv_entry_iterator_parent_class)->finalize (object);
        return;
}

static void
ytv_entry_iterator_class_init (YtvEntryIteratorClass* klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = ytv_entry_iterator_finalize;

        return;
}

static void
ytv_entry_iterator_init (YtvEntryIterator* self)
{
        self->list = NULL;
        self->current = NULL;
}

void
ytv_entry_iterator_set_list (YtvEntryIterator* self, YtvEntryList* list)
{
        self->list = list;
        self->current = list->first;
}

YtvIterator*
ytv_entry_iterator_new (YtvEntryList* list)
{
        YtvEntryIterator *self = g_object_new (YTV_TYPE_ENTRY_ITERATOR, NULL);

        ytv_entry_iterator_set_list (self, list);

        return YTV_ITERATOR (self);
}
