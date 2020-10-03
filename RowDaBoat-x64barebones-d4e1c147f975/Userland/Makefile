include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule
gdb: sampleCodeModule_gdb

sampleCodeModule:
	cd SampleCodeModule; make

sampleCodeModule_gdb:
	cd SampleCodeModule; make gdb
	make bin

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	rm -rf *.bin *.o *.elf

bin:
	objcopy -O binary 0000-sampleCodeModule.elf 0000-sampleCodeModule.bin

.PHONY: sampleCodeModule all clean bin