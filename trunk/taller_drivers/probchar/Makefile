obj-m := probchar.o
KVERSION := $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules

clean:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean

ins:
	rmmod probchar.ko
	insmod probchar.ko

read:
	dd if=/dev/probchar count=1 bs=1 < text.out
	cat text.out -n 100

write:
	echo "4543434443" > /dev/probchar

