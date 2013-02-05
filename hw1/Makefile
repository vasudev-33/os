obj-m += sys_xcrypt.o

all: xcipher xintegrity

xhw1: xhw1.c
	gcc -Wall -Werror -I/lib/modules/$(shell uname -r)/build/arch/x86/include xhw1.c -o xhw1

xintegrity:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f xcipher
