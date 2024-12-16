# Kernel module
obj-m += chardev.o

# Default build target for the kernel module
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o test_app test_app.c

# Clean build artifacts
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test_app


