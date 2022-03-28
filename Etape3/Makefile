.PHONY: all, clean

# Disable implicit rules
.SUFFIXES:

# Keep intermediate files
#.PRECIOUS: %.o

CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS =

#LIBS += -lsocket -lnsl -lrt
LIBS += -lpthread

INCLUDE = csapp.h serveur_corps.h client_corps.h
OBJS = csapp.o serveur_corps.o client_corps.o
INCLDIR = -I.

PROGS = serveurFTP clientFTP

all: $(PROGS)

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<
	
%: %.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
	
clean:
	rm -f $(PROGS) *.o
