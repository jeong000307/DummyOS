AS = nasm
CC = clang
LD = lld-link

ASMFLAGS = -f win64
CFLAGS = -Werror -target x86_64-unknown-windows -std=c11 -ffreestanding -fshort-wchar -mno-red-zone -O
LDFLAGS = -nologo -entry:Main -subsystem:efi_application

ASMSOURCES = $(wildcard *.asm)
ASMOBJECTS = $(patsubst %.asm, Build/Object/%.o, $(ASMSOURCES))
CSOURCES = $(wildcard *.c)
COBJECTS = $(patsubst %.c, Build/Object/%.o, $(CSOURCES))

all: prepare Build/BootX64.efi

prepare: 
	mkdir -p Build/Object

Build/Object/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

Build/Object/%.o: %.asm
	$(AS) $(ASMFLAGS) -o $@ $^

Build/BootX64.efi: $(COBJECTS) $(ASMOBJECTS)
	$(LD) $(LDFLAGS) -out:Build/BootX64.efi $^

clean:
	rm -rf Build