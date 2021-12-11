
ifneq ($(KERNELRELEASE),)
	obj-m := hw.o 
else
	include ../Makefile
	KERNELDIR := ../../$(KERNEL_VERSION)
	PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.[oas] .*.flags *.ko .*.cmd .*.d .*.tmp *.mod.c .tmp_versions Module.symvers
