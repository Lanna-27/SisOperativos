ifneq($(KERNELRELEASE),)
obj-m := driver_rwTest.o
else
KDIR := /home/estudiante/linux-source-4.15.0/linux-source-4.15.0
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif

clean: 
	rm *.o