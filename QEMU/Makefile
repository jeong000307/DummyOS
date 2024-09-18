all: prepare Boot Kernel QEMU

prepare:
	mkdir -p Image/EFI/Boot

Boot:
	make -C ../소스/Boot

Kernel:
	make -C ../소스/Kernel

QEMU:
	cp ../소스/Boot/Build/BootX64.efi Image/EFI/Boot
	cp ../소스/Kernel/Build/Kernel.exe Image

	qemu-system-x86_64 -M q35 -drive file=fat:rw:Image,format=raw,media=disk -smp 2 -m 1G -bios OVMF.fd

clean:
	make -C ../소스/Boot clean
	make -C ../소스/Kernel clean
	rm -rf Image