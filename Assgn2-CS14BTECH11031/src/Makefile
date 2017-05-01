obj-m += Assgn2Src-CS14BTECH11031.o
USER := $(shell uname -r)
PWD := $(shell pwd)

all:
	make -C /lib/modules/$(USER)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(USER)/build M=$(PWD) clean

install:
	make -C /lib/modules/$(USER)/build M=$(PWD) modules_install
