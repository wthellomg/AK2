
ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello.o
else
# normal makefile
#KDIR ?= /lib/modules/`uname -r`/build
KDIR ?= $(HOME)/repos/linux-stable
default:
	$(MAKE) -C $(KDIR) M=$$PWD
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif
