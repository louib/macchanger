/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/* MAC Changer
 *
 * Authors:
 *      Alvaro Lopez Ortega <alvaro@alobbs.com>
 *
 * Copyright (C) 2002,2013 Alvaro Lopez Ortega
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include <gtk/gtk.h>
// This is required to acknowledge that we are using an unstable
// version of the handy library.
#define HANDY_USE_UNSTABLE_API
#include <handy.h>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Required to query the interfaces with if_nameindex()
#include <net/if.h>

#include "mac.h"
#include "maclist.h"
#include "netinfo.h"

#define EXIT_OK    0
#define EXIT_ERROR 1

static void
print_help (void)
{
	printf ("GNU MAC Changer\n"
		"Usage: macchanger [options] device\n\n"
		"  -h,  --help                   Print this help\n"
		"  -V,  --version                Print version and exit\n"
		"  -s,  --show                   Print the MAC address and exit\n"
		"  -e,  --ending                 Don't change the vendor bytes\n"
		"  -a,  --another                Set random vendor MAC of the same kind\n"
		"  -A                            Set random vendor MAC of any kind\n"
		"  -p,  --permanent              Reset to original, permanent hardware MAC\n"
		"  -r,  --random                 Set fully random MAC\n"
		"  -l,  --list[=keyword]         Print known vendors\n"
		"  -b,  --bia                    Pretend to be a burned-in-address\n"
		"  -m,  --mac=XX:XX:XX:XX:XX:XX  Set the MAC XX:XX:XX:XX:XX:XX\n\n"
		"Report bugs to https://github.com/alobbs/macchanger/issues\n");
}


static void
print_usage (void)
{
	printf ("GNU MAC Changer\n"
		"Usage: macchanger [options] device\n\n"
		"Try `macchanger --help' for more options.\n");
}


static void
print_mac (const char *s, const mac_t *mac)
{
	char string[18];
	int  is_wireless;

	is_wireless = mc_maclist_is_wireless(mac);
	mc_mac_into_string (mac, string);
	printf ("%s%s%s (%s)\n", s,
		string,
		is_wireless ? " [wireless]": "",
		CARD_NAME(mac));
}

static void interface_changed(GtkWidget* widget, gpointer data) {
  printf("interface changed\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
  GtkWidget* window;
  GtkComboBoxText* combo;
  GtkBuilder* builder;
  GError* error = NULL;


  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "src/main-window.ui", &error) == 0) {
    g_printerr ("Error loading file: %s\n", error->message);
    g_clear_error (&error);
    return;
  }

  window = (GtkWidget*) gtk_builder_get_object (builder, "main_window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  combo = (GtkComboBoxText*) gtk_builder_get_object (builder, "interface_combo_box");

  struct if_nameindex *if_nidxs, *intf;

  // Populating the interface names.
  if_nidxs = if_nameindex();
  if (if_nidxs == NULL) {
     perror("could not get network interface names.");
     exit(EXIT_FAILURE);
  }
  for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++) {
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), NULL, intf->if_name);

    // TODO set to active if it's a wireless interface.
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 1);
  }
  if_freenameindex(if_nidxs);

  g_signal_connect (combo, "changed", G_CALLBACK (interface_changed), NULL);

  // window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "macchanger");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all (window);
}

int main (int argc, char** argv) {
  GtkApplication* app;
  int status;

  app = gtk_application_new ("org.gnu.macchanger", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  gtk_main ();
  return status;
}
