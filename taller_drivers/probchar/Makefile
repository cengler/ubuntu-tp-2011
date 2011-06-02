obj-m := probchar.o
KVERSION := $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules

clean:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean
	rm -f text.out

ins:
	
	rmmod probchar.ko
	insmod probchar.ko

read:
	dd if=/dev/probchar count=1000 bs=1 > text.out
	cat text.out

write:
	echo "45" > /dev/probchar

