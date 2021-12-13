#!/bin/sh

set -xe

CFLAGS="-g -Wall -Werror -Wextra -Wshadow"

gcc $CFLAGS -o lang-checker main.c
