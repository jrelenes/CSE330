sysroot := "/home/jrelenes/linux-4.19"

obj-m += kernel_module.o

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C ${sysroot} M=${PWD} modules
clean:
	make -C ${sysroot} M=${PWD} clean
