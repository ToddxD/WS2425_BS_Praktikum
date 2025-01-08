obj-m+=hello.o

all:
	make -C /lib/modules/6.4.0-150600.23.30-default/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/6.4.0-150600.23.30-default/build/ M=$(PWD) clean