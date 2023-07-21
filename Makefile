obj-m += led_butt.o

all:
	make -C /lib/modules/5.10.21-200.fc33.armv7hl/build M=$(PWD) modules

clean:
	 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

