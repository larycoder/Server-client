default: all

all:
	cd ./server; make to path=../bin
	cd ./client; make to path=../bin

test:
	make -f server/makefile