/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; coding: utf-8 -*- */

#ifndef _YTV_SHELL_H_
#define _YTV_SHELL_H_

/* ytv-shell.h - GTK+ widget with the whole graphic shell
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

#include <gtk/gtknotebook.h>

G_BEGIN_DECLS

#define YTV_TYPE_SHELL                          \
        (ytv_shell_get_type ())
#define YTV_SHELL(obj)                          \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), YTV_TYPE_SHELL, YtvShell))
#define YTV_SHELL_CLASS(klass)                  \
        (G_TYPE_CHECK_CLASS_CAST ((klass), YTV_TYPE_SHELL, YtvShellClass))
#define YTV_IS_SHELL(obj)                       \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), YTV_TYPE_SHELL))
#define YTV_IS_SHELL_CLASS(klass)               \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), YTV_TYPE_SHELL))
#define YTV_SHELL_GET_CLASS(obj)                \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), YTV_TYPE_SHELL, YtvShellClass))

typedef struct _YtvShell YtvShell;
typedef struct _YtvShellClass YtvShellClass;

/**
 * YtvShell:
 *
 * GTK+ widget representing the graphical shell
 */
struct _YtvShell
{
        GtkNotebook parent;
};

struct _YtvShellClass
{
        GtkNotebookClass parent_class;

        /* signals */
        void (*error_raised) (YtvShell* self, GError* error);
};

GType ytv_shell_get_type (void);

GtkWidget* ytv_shell_new (void);

G_END_DECLS

#endif /* _YTV_SHELL_H_ */
