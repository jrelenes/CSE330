sysroot := "/home/chinmay/Documents/ASU/sp20/cse330/project3/linux-4.19"
obj-m += walker.o

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C ${sysroot} \
		M=${PWD} modules

clean:
	make -C ${sysroot} M=${PWD} clean
