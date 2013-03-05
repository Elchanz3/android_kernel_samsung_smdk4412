all: acpi_fakekey acpi_fakekeyd spen_helper

acpi_fakekey: acpi_fakekey.c
	$(TARGET)gcc$(CC_VERSION) -Wall $(shell dpkg-buildflags --get CFLAGS) $(shell dpkg-buildflags --get CPPFLAGS) $(shell dpkg-buildflags --get LDFLAGS) -o $@ $<

acpi_fakekeyd: acpi_fakekeyd.c
	$(TARGET)gcc$(CC_VERSION) -Wall $(shell dpkg-buildflags --get CFLAGS) $(shell dpkg-buildflags --get CPPFLAGS) $(shell dpkg-buildflags --get LDFLAGS) -o $@ $<

spen_helper: spen_helper.c
	$(TARGET)gcc$(CC_VERSION) -Wall $(shell dpkg-buildflags --get CFLAGS) $(shell dpkg-buildflags --get CPPFLAGS) $(shell dpkg-buildflags --get LDFLAGS) -o $@ $<

# Keep this manually generated so that it doesn't build-dep on
# 'linux-headers' and stays predictable.

key-constants: /usr/include/linux/input.h aliased-keys
	test -f $<
	echo -n '# Generated from $< dated ' > $@
	date -r $< >> $@
	awk '/#define  *KEY_/{if(strtonum($$3) <= 255) print $$2"="strtonum($$3)}' $< | sort -nt'=' -k2 >> $@
	test -f aliased-keys && cat aliased-keys >> $@

clean:
	$(RM) acpi_fakekey acpi_fakekeyd
