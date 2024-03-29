GCC=gcc
LD=ld
AR=ar
ASM=nasm

GCCFLAGS=-m64 -fno-exceptions -fno-asynchronous-unwind-tables -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -mno-red-zone -Wall -Wextra -ffreestanding -nostdlib -fno-common -std=c99 -g
ARFLAGS=rvs
ASMFLAGS=-f elf64 -g -F dwarf
LDFLAGS=--warn-common -z max-page-size=0x1000 -g
