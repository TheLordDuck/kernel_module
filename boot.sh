#!/bin/bash

QEMU=qemu-system-i386

if which $QEMU
then
	$QEMU $* -boot c -kernel linux-4.19.206/arch/x86/boot/bzImage -hda ./image.img -append "root=/dev/hda clock=pit" -net nic,model=e1000 -net user -k es
else
	echo No he trobat executable qemu / No he encontrado ejecutable qemu
	echo Està instal.lat? / ¿Está instalado?
	echo Està actualitzada la variable PATH? / ¿Está actualizada la variable PATH?
fi
