DIST_SUBDIRS = \
	acpi \
	udisks \
	eee \
	hddtemp \
        i2c-proc \
        i2c-sys \
	i8k \
	ibm-acpi \
        libsensors \
        nvidia \
	omnibook \
	pmu-sys \
	smu-sys \
	sonypi \
	aticonfig

SUBDIRS = \
	acpi \
	eee \
	hddtemp \
	i8k \
	ibm-acpi \
	omnibook \
	pmu-sys \
	smu-sys \
	sonypi

if UDISKS
SUBDIRS	     += udisks
endif

if LIBSENSORS
SUBDIRS      += libsensors
else
SUBDIRS      += i2c-proc i2c-sys
endif

if NVIDIA
SUBDIRS      += nvidia
endif

if ATICONFIG
SUBDIRS      += aticonfig
endif
