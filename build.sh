#!/bin/sh

set -xe

CFLAGS="-Wall -Werror -Wextra -Wshadow"

gcc $CFLAGS -o lang-checker main.c
