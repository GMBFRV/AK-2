
obj-m += hello1.o
obj-m += hello2.o

ccflags-y := -I$(src)/inc
KDIR ?= /lib/modules/`uname -r`/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

