# fmon - simple file monitor
# See LICENSE file for copyright and license details
.POSIX:

include config.mk

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: fmon

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OBJ): config.mk

fmon: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f fmon $(OBJ) fmon-$(VERSION).tar.gz

dist: clean
	mkdir -p fmon-$(VERSION)
	cp -R LICENSE Makefile config.h config.mk\
		$(OBJ:.o=.h) $(SRC) fmon.1 fmon-$(VERSION)
	tar -czf fmon-$(VERSION).tar.gz fmon-$(VERSION)
	rm -rf fmon-$(VERSION)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f fmon $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/fmon
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < fmon.1 > $(DESTDIR)$(MANPREFIX)/man1/fmon.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/fmon.1

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/fmon"
	rm -f "$(DESTDIR)$(MANPREFIX)/man1/fmon.1"

.PHONY: all options clean dist install uninstall
