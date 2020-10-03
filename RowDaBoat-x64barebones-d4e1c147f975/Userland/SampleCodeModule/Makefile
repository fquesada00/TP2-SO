include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
MODULE_GDB=0000-sampleCodeModule.elf
SOURCES=$(wildcard [^_]*.c)
SOURCES_LIB =$(wildcard ../*.c)
SOURCES_LIB_ASM=$(wildcard ../*.asm)
OBJECTS_LIB_ASM=$(SOURCES_LIB_ASM:.asm=.o)
all: $(ASM) $(MODULE)
gdb: $(ASM) $(MODULE_GDB)

$(ASM):
	nasm -g -f elf64 -F dwarf $(SOURCES_LIB_ASM)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) $(SOURCES_LIB) $(OBJECTS_LIB_ASM) -I ../include -o ../$(MODULE)

$(MODULE_GDB): $(SOURCES)
	$(GCC) $(GCCFLAGS) -g -T sampleCodeModule_gdb.ld _loader.c $(SOURCES) $(SOURCES_LIB) $(OBJECTS_LIB_ASM) -I ../include -o ../$(MODULE_GDB)

clean:
	rm -rf *.o

.PHONY: gdb all clean print