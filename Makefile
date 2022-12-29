# Copyright © 2022 Jakub Wilk <jwilk@jwilk.net>
# SPDX-License-Identifier: MIT

CFLAGS ?= -g -O2
CFLAGS += -Wall -Wformat -Wextra -pedantic

.PHONY: all
all: reflink

reflink: reflink.c

.PHONY: clean
clean:
	rm -f reflink

# vim:ts=4 sts=4 sw=4 noet

