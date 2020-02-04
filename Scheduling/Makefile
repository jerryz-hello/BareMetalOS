# 
# Team: 2-8
# Members: Jerry Zheng, Jocelynn Cheesebourough
#

all: shell floppya.img 

kernel.o: kernel.c kernel.h
	bcc -ansi -c -o kernel.o kernel.c

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o 

kernel: kernel.o kernel_asm.o
	ld86 -o kernel -d kernel.o kernel_asm.o 
	
bootload: bootload.asm
	nasm bootload.asm

shell.o: shell.c shell.h kernel.h
	bcc -ansi -c -o shell.o shell.c

lib_asm.o: lib.asm
	as86 lib.asm -o lib_asm.o 

shell: shell.o lib_asm.o
	ld86 -o shell -d shell.o lib_asm.o 

loadFile: loadFile.o
	gcc -o loadFile loadFile.o

loadFile.o: loadFile.c
	gcc -c loadFile.c

floppya.img: bootload kernel shell loadFile
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
	./loadFile bigmess.txt
	./loadFile message.txt
	./loadFile phello
	./loadFile shell
	
run: 
	qemu-system-i386 -fda floppya.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a &

clean: 
	rm -rf *.o kernel bootload floppya.img loadFile shell
