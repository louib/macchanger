AM_CPPFLAGS = -DLISTDIR="\"$(datadir)/$(PACKAGE)\""
AM_LDFLAGS = -lgtk+-3.0

bin_PROGRAMS = macchanger macchangergtk

macchanger_SOURCES = \
mac.h mac.c \
maclist.h maclist.c \
netinfo.h netinfo.c \
main.c

macchangergtk_SOURCES = \
mac.h mac.c \
maclist.h maclist.c \
netinfo.h netinfo.c \
gtk-main.c

macchangergtk_CPPFLAGS = @GTK_CFLAGS@
macchangergtk_LDADD = @GTK_LIBS@
