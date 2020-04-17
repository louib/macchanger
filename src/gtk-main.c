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


static void
random_seed (void)
{
	int            fd;
	struct timeval tv;
	unsigned int   seed;

	if ((fd = open("/dev/hwrng", O_RDONLY)) >= 0 ||
	    (fd = open("/dev/random", O_RDONLY)) >= 0 ||
	    (fd = open("/dev/urandom", O_RDONLY)) >= 0)
	{
		read (fd, &seed, sizeof(seed));
		close (fd);
	} else {
		gettimeofday (&tv, NULL);
		seed = (getpid() << 16) ^ tv.tv_sec ^ tv.tv_usec;
	}

	srandom(seed);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget* window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "macchanger");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication* app;
  int status;

  app = gtk_application_new ("org.gnu.macchanger", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
