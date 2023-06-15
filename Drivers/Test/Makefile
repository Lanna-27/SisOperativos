ifneq($(KERNELRELEASE),)
obj-m := driver_test.o
else
KDIR := /home/estudiante/linux-source-4.15.0/linux-source-4.15.0
all:
	$(MAKE) -c $(KDIR) M=$$PWD
endif
	