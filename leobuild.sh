build -a X64 -p MdeModulePkg/MdeModulePkg.dsc
cp Build/MdeModule/DEBUG_GCC5/X64/HelloWorld.efi Build/MdeModule/DEBUG_GCC5/fun/HelloWorld.efi
cp Build/MdeModule/DEBUG_GCC5/X64/HelloWorld.debug Build/MdeModule/DEBUG_GCC5/fun/HelloWorld.debug

qemu-system-x86_64 -bios /usr/share/ovmf/x64/OVMF.fd -net none -hda fat:rw:Build/MdeModule/DEBUG_GCC5/fun/
