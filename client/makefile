CC			  = gcc
CFLAGS	  = -pthread
TARGET	  = client.o
T_TARGET	= ui.o
OBJFILES  = main.c staff.c terminal.c
T_OBJFILES= test.c terminal.c staff.c
LDFLAGS	  =

default: client

client: $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

to: $(OBJFILES)
	$(CC) $(CFLAGS) -o $(path)/$(TARGET) $(OBJFILES) $(LDFLAGS)

test: $(T_OBJFILES)
	$(CC) $(CFLAGS) -o $(T_TARGET) $(T_OBJFILES) $(LDFLAGS)