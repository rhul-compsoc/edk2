build -a X64 -p Wordle/Wordle.dsc

qemu-system-x86_64 -bios /usr/share/ovmf/x64/OVMF.fd -net none -hda fat:rw:Build/Wordle/DEBUG_GCC5/X64/
