#!/bin/bash

QEMU=qemu-system-i386

if which $QEMU
then
	$QEMU $* -boot c -kernel linux-4.19.206/arch/x86/boot/bzImage -hda ./image.img -append "root=/dev/hda clock=pit" -net nic,model=e1000 -net user -k es
else
	echo qemu not found
	echo Is it installed ?
	echo check your PATH
fi
