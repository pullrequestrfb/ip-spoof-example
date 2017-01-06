.PHONY: all build clean install uninstall

all: build

build:
	mkdir -p dist/
	$CC -o dist/ipspoof ip_spoof.c

clean:
	rm -rf dist/

install:
	cp -r dist/ /usr/local/bin/

uninstall:
	rm /usr/local/bin/ipspoof
