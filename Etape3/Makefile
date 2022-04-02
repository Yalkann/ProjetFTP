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

INCLUDE = csapp.h slaves_body.h master_body.h client_body.h
OBJS = csapp.o slaves_body.o master_body.o client_body.o
INCLDIR = -I.

PROGS = slaves_FTP master_FTP client_FTP

all: $(PROGS)

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<
	
%: %.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)
	
clean:
	rm -f $(PROGS) *.o
