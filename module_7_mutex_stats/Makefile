obj-m+=mutex-stats.o
CC:=colorgcc
CFLAGS_mutex-stats.o := -DDEBUG


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko *~
	rm .tmp_versions -fr ; rm Module.symvers -fr
	rm -fr Module.markers ; rm -fr modules.order
