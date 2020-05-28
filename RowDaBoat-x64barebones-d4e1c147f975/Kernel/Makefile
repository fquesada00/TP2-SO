include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_INT=$(wildcard Interrupciones/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_ASM_INT=$(wildcard Interrupciones/*.asm)
SOURCES_VIDEO=$(wildcard Video_Driver/*.c)
SOURCES_KEYBOARD=$(wildcard Keyboard_Driver/*.c)
SOURCES_ASM_KEYBOARD=$(wildcard Keyboard_Driver/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_INT=$(SOURCES_INT:.c=.o)
OBJECTS_ASM_INT=$(SOURCES_ASM_INT:.asm=.o)
OBJECTS_VIDEO=$(SOURCES_VIDEO:.c=.o)
OBJECTS_KEYBOARD=$(SOURCES_KEYBOARD:.c=.o)
OBJECTS_ASM_KEYBOARD=$(SOURCES_ASM_KEYBOARD:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_INT) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_ASM_INT) $(OBJECTS_VIDEO) $(OBJECTS_KEYBOARD) $(OBJECTS_ASM_KEYBOARD)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_INT) $(OBJECTS_ASM_INT) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_VIDEO) $(OBJECTS_KEYBOARD) $(OBJECTS_ASM_KEYBOARD)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin Interrupciones/*.o Keyboard_Driver/*.o Video_Driver/*.o

.PHONY: all clean
