SHELL	= /bin/sh
CC	= gcc

FLAGS	= -ansi -Iinclude
CFLAGS	= -pedantic -Wall -Wextra

TARGET	= ipass
SOURCES	= $(shell echo src/*.c)
HEADERS = $(shell echo include/*.h)
OBJECTS	= $(SOURCES:.c=.o)

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET):	$(OBJECTS) $(COMMON)
		$(CC) $(FLAGS) $(CFLAGS) -o $(TARGET) $(OBJECTS)

install:	$(TARGET)
		install -D $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
		-rm $(BINDIR)/$(TARGET)

clean:
		-rm -f $(OBJECTS)
 
distclean:	clean
		-rm -f $(TARGET)

%.o: %.c $(HEADERS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -c -o $@ $<

.PHONY : all install uninstall clean distclean
