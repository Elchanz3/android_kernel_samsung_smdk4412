#!/bin/sh

SPEN_INPUT_DEVICE=6

. /usr/local/share/acpi-fakekey/key-constants

/usr/local/bin/spen_helper $SPEN_INPUT_DEVICE $KEY_PROG1 $KEY_PROG2

