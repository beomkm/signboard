obj-m := hellomodule.o
KDIR := /home/byk/xc/linux-rpi-4.1.19
PWD := $(shell pwd)

default:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
