include ../../mk/flags.mk
include ../../mk/build.mk

all: term/term init/init test/test ls/ls

term/term:
	cd term && make all

init/init:
	cd init && make all

test/test:
	cd test && make all

ls/ls:
	cd ls && make all

clean:
	cd term && make clean
	cd init && make clean
	cd test && make clean
	cd ls && make clean

