
todo: clean all

all:  bootloader kernel userland image permissions
gdb:  bootloader kernel_gdb userland_gdb permissions

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all MM=$(MM)
kernel_gdb:
	cd Kernel; make gdb MM=$(MM)

userland:
	cd Userland; make all
userland_gdb:
	cd Userland; make gdb

image: kernel bootloader userland
	cd Image; make all

permissions: image
	chmod -R 777 *

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

test:
	cd Kernel; make test MM=$(MM)

.PHONY: bootloader image collections kernel kernel_gdb userland userland_gdb permissions all clean