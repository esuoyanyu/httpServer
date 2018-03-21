CC     ?= gcc
export CC   

all:
	make -C ./src ta 

clean:
	make -C ./src clean