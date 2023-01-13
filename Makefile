# SPDX-License-Identifier: MIT

override CFLAGS+= -Wall -Wextra
LDLIBS+= -lcrypt
DEPS = runCommand.h chkpw.h
OBJ = runCommand.o chkpw.o sudo.o
.PHONY: all suid clean
$.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
sudo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
all: sudo
suid: sudo
	chown root:root $^
	chmod +s $^
clean:
	rm -f *.o sudo
