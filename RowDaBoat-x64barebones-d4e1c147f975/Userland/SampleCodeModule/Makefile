include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_LIB =$(wildcard ../*.c)
SOURCES_LIB_ASM=$(wildcard ../*.asm)
OBJECTS_LIB_ASM=$(SOURCES_LIB_ASM:.asm=.o)
all: $(ASM) $(MODULE)

$(ASM):
	nasm -f elf64 $(SOURCES_LIB_ASM)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) $(SOURCES_LIB) $(OBJECTS_LIB_ASM) -I ../include -o ../$(MODULE)


clean:
	rm -rf *.o

.PHONY: all clean print
