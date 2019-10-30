CC      = gcc
CFLAGS  =
TARGET  = server.o
OBJFILES= main.c staff.c talker.c header.h
LDFLAGS =

default: server

server: $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

to: $(OBJFILES)
	$(CC) $(CFLAGS) -o $(path)/$(TARGET) $(OBJFILES) $(LDFLAGS)