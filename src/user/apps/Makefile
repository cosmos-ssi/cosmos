include ../../mk/flags.mk
include ../../mk/build.mk

all: term/term init/init

term/term:
	cd term && make all

init/init:
	cd init && make all
	
clean:
	cd term && make clean
	cd init && make clean
