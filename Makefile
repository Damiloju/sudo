# SPDX-License-Identifier: MIT

override CFLAGS+= -Wall -Wextra
LDLIBS+= -lcrypt
.PHONY: all suid clean
all: sudo
suid: sudo
	chown root:root $^
	chmod +s $^
clean:
	rm -f *.o sudo
