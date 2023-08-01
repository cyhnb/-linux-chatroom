KERNELDIR := /home/alientek/alpha/alientek-zzk/kernel-zzk/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
CC = $(CROSS_COMPILE)gcc
CURRENT_PATH := $(shell pwd)
LDFLAGS = -pthread
build: kernel_modules

kernel_modules:
	#$(CROSS_COMPILE)arm-linux-gnueabihf-gcc -o server server.c
	$(CC) $(LDFLAGS) $(LDFLAGS) -o server server.c
	$(CC) $(LDFLAGS) $(LDFLAGS) -o client client.c
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
