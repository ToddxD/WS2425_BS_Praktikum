obj-m+=hello.o

all:
	make -C /lib/modules/linux-6.4.0-150600.23.30/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/linux-6.4.0-150600.23.30/build/ M=$(PWD) clean