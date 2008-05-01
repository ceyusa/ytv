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
#include <ytv-list.h>
#include <ytv-entry-iterator.h>
#include <ytv-entry-list.h>

static guint
ytv_entry_list_get_length (YtvList* self)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        YtvEntryList* me = YTV_ENTRY_LIST (self);
        guint retval = 0;

        g_mutex_lock (me->list_lock);
        retval = me->first ? g_list_length (me->first) : 0;
        g_mutex_unlock (me->list_lock);

        return retval;
}

static void
ytv_entry_list_prepend (YtvList* self, YtvObject* item)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        g_assert (YTV_IS_ENTRY (item));
        YtvEntryList* me = YTV_ENTRY_LIST (self);
        YtvEntry *entry = YTV_ENTRY (item);

        g_mutex_lock (me->list_lock);
        g_object_ref (G_OBJECT (entry));
        me->first = g_list_prepend (me->first, entry);
        g_mutex_unlock (me->list_lock);

        return;
}

static void
ytv_entry_list_append (YtvList* self, YtvObject* item)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        g_assert (YTV_IS_ENTRY (item));
        YtvEntryList* me = YTV_ENTRY_LIST (self);
        YtvEntry *entry = YTV_ENTRY (item);

        g_mutex_lock (me->list_lock);
        g_object_ref (G_OBJECT (entry));
        me->first = g_list_append (me->first, entry);
        g_mutex_unlock (me->list_lock);

        return;
}

static void
ytv_entry_list_remove (YtvList* self, YtvObject* item)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        g_assert (YTV_IS_ENTRY (item));
        YtvEntryList* me = YTV_ENTRY_LIST (self);
        YtvEntry *entry = YTV_ENTRY (item);

        g_mutex_lock (me->list_lock);
        GList *l = NULL;
        for (l = me->first; l != NULL; l = l->next)
        {
                if (entry == YTV_ENTRY (l->data))
                {
                        g_object_unref (G_OBJECT (entry));
                        break;
                }               
        }
        g_mutex_unlock (me->list_lock);
}

static YtvIterator*
ytv_entry_list_create_iterator (YtvList* self)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        YtvEntryList* me = YTV_ENTRY_LIST (self);

        return YTV_ITERATOR (ytv_entry_iterator_new (me));
}

static YtvList*
ytv_entry_list_copy (YtvList* self)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        YtvEntryList* me = YTV_ENTRY_LIST (self);
        YtvEntryList* copy = g_object_new (YTV_TYPE_ENTRY_LIST, NULL);

        g_mutex_lock (me->list_lock);
        GList *list_copy = g_list_copy (me->first);
        g_list_foreach (list_copy, (GFunc) g_object_ref, NULL);
        copy->first = list_copy;
        g_mutex_unlock (me->list_lock);

        return YTV_LIST (copy);
}

static void
ytv_entry_list_foreach (YtvList* self, GFunc func, gpointer user_data)
{
        g_assert (YTV_IS_ENTRY_LIST (self));
        YtvEntryList* me = YTV_ENTRY_LIST (self);

        g_mutex_lock (me->list_lock);
        g_list_foreach (me->first, func, user_data);
        g_mutex_unlock (me->list_lock);

        return;
}

static void
ytv_list_init (YtvListIface* klass)
{
        klass->get_length_func = ytv_entry_list_get_length;
        klass->prepend_func = ytv_entry_list_prepend;
        klass->append_func = ytv_entry_list_append;
        klass->remove_func = ytv_entry_list_remove;
        klass->create_iterator_func = ytv_entry_list_create_iterator;
        klass->copy_func = ytv_entry_list_copy;
        klass->foreach_func = ytv_entry_list_foreach;
}

G_DEFINE_TYPE_EXTENDED (YtvEntryList, ytv_entry_list, G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE (YTV_TYPE_LIST, ytv_list_init))

static void
destroy_entrys (gpointer entry, gpointer user_data)
{
        if (entry && G_IS_OBJECT (entry))
        {
                g_object_unref (G_OBJECT (entry));
        }

        return;
}

static void
ytv_entry_list_finalize (GObject* object)
{
        YtvEntryList* self = YTV_ENTRY_LIST (object);

        g_mutex_lock (self->list_lock);
        if (self->first)
        {
                g_list_foreach (self->first, destroy_entrys, NULL);
                g_list_free (self->first);
                self->first = NULL;
        }
        g_mutex_unlock (self->list_lock);

        g_mutex_free (self->list_lock);
        self->list_lock = NULL;

        G_OBJECT_CLASS (ytv_entry_list_parent_class)->finalize (object);

        return;
}

static void
ytv_entry_list_class_init (YtvEntryListClass *klass)
{
        GObjectClass* object_class;

        object_class = (GObjectClass*) klass;

        object_class->finalize = ytv_entry_list_finalize;

        return;
}

static void
ytv_entry_list_init (YtvEntryList *self)
{
        self->list_lock = g_mutex_new ();
        self->first = NULL;
}

YtvList*
goo_entry_list_new (void)
{
        return GOO_LIST (g_object_new (GOO_TYPE_ENTRY_LIST, NULL));
}
