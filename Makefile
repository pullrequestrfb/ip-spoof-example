.PHONY: all get-deps build clean install uninstall

MACHINE = ""
PKG_MGR = ""
HAS_GYP = 0
HAS_CURL = 0
HAS_AUTOMAKE = 0
HAS_LIBTOOL = 0
SRC_FILES = ./src/ip_spoof.c ./src/eventloop/loop.c ./src/srv/srv.c ./src/spoof/socket.c

ifeq ("$(shell uname -s)","Linux")
	DISTRO = $(shell source /etc/os-release && echo $NAME)
ifneq ("$(DISTRO)","RHEL")
	MACHINE = "debian"
else
	MACHINE ="rhel"
endif
else
	MACHINE ="darwin"
endif

ifeq ($(MACHINE),"darwin")
	PKG_MGR = brew
else
ifeq ($(MACHINE),"debian")
	PKG_MGR = apt-get
else
	PKG_MGR = yum
endif
endif


ifneq ("$(shell which curl)","")
	HAS_CURL = 1
else
	HAS_CURL = 0
endif

ifneq ("$(shell which gyp)","")
	HAS_GYP = 1
else
	HAS_GYP = 0
endif

ifneq ("$(shell which automake)","")
	HAS_AUTOMAKE = 1
else
	HAS_AUTOMAKE = 0
endif

ifneq ("$(shell which libtoolize)", "")
	HAS_LIBTOOL = 1
else
	HAS_LIBTOOL = 0
endif

all: get-deps build

get-deps:
	mkdir -p ./tmp/
	# gyp
	if [ $(HAS_GYP) -eq 0 ]; then \
		$(PKG_MGR) install -y gyp; \
	fi
	# automake
	if [ $(HAS_AUTOMAKE) -eq 0 ]; then \
		$(PKG_MGR) install -y automake; \
	fi
	# libtool
	if [ $(HAS_LIBTOOL) -eq 0 ]; then \
		$(PKG_MGR) install -y libtool; \
	fi
	# libuv
	gpg --keyserver pool.sks-keyservers.net --recv-keys AE9BC059
	if [ $(HAS_CURL) -eq 1 ]; then \
		curl -o ./tmp/libuv-v1.10.1.tar.gz http://dist.libuv.org/dist/v1.10.1/libuv-v1.10.1.tar.gz; \
	else \
		wget -O ./tmp/libuv-v1.10.1.tar.gz http://dist.libuv.org/dist/v1.10.1/libuv-v1.10.1.tar.gz; \
	fi
	ORIG_DIR=`pwd`
	cd ./tmp/ && \
	tar xzvf ./libuv-v1.10.1.tar.gz && \
	cd ./libuv-v1.10.1/ && \
	sh autogen.sh && \
	./configure && \
	make && \
	make check && \
	make install

build:
	mkdir -p dist/
	$(CC) -pthread -o dist/ipspoof $(SRC_FILES) /usr/local/lib/libuv.a

clean:
	rm -rf dist/

install:
	cp -r dist/ /usr/local/bin/

uninstall:
	rm /usr/local/bin/ipspoof

docker:
	docker build -t ipspoof .
