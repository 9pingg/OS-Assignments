obj-m = new_device.o

KBUILD_CFLAGS += -w

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules $(KBUILD_CFLAGS)
	sudo insmod new_device.ko buffer_size=200
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean