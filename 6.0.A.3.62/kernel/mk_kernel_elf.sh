#!/bin/sh
python ./mkelf.py -o kernel.elf ./arch/arm/boot/zImage@0x40208000 ramdisk.gz@0x41200000,ramdisk rpm.bin@0x20000,rpm 
