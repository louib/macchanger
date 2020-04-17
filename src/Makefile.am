bin_PROGRAMS = macchanger macchangergtk

macchanger_SOURCES = \
mac.h mac.c \
maclist.h maclist.c \
netinfo.h netinfo.c \
main.c

AM_CPPFLAGS = -DLISTDIR="\"$(datadir)/$(PACKAGE)\""

macchangergtk_SOURCES = \
mac.h mac.c \
maclist.h maclist.c \
netinfo.h netinfo.c \
gtk-main.c

macchangergtk_CPPFLAGS = @GTK_CFLAGS@ -DLISTDIR="\"$(datadir)/$(PACKAGE)\""
macchangergtk_LDADD = @GTK_LIBS@
