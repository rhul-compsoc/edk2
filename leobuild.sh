export CCFLAGS=-Wno-error

source edksetup.sh BaseTools

build -a X64 -p Wordle/Wordle.dsc

qemu-system-x86_64 -bios /usr/share/ovmf/x64/OVMF.fd -net none -usb -device usb-tablet -vga cirrus -hda fat:rw:Build/Wordle/DEBUG_GCC5/X64/
