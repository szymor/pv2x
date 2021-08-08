include Makefile.setup

LDFLAGS+=-rdynamic -ldl

SDMOUNTPOINT=/media/sd
INSTALLDIR=dev

TARGETGP2X=	pv2x.gpe
TARGETLINUX=	pv2x
TARGETMIYOO=	pv2x-miyoo

OBJ=	filelistrandomizer.o \
	pv2xplugin.o \
	plugins.o \
	sdltools.o \
	scaledimage.o \
	filelist.o \
	sdlttf.o \
	misctools.o \
	projectfunctions.o \
	main.o

%.o:%.cpp
	$(CPP) -o $@ -Wall $(DEBUG) $(CFLAGS) -c $<

all:$(OBJ) allplugins
	$(CPP) -o $(TARGET) $(OBJ) $(LDFLAGS)

allplugins:
	$(MAKE) -C plugins

clean:
	$(MAKE) -C plugins clean
	rm -f $(OBJ)
	rm -f $(TARGETGP2X) $(TARGETLINUX)

distclean:clean
	rm -rf dist
	rm -f pv2x.cfg

dist:all
	mkdir -p dist/pv2x-$(VERSION)/plugins
	mkdir -p dist/pv2x-$(VERSION)/doc
	cp $(TARGETGP2X) dist/pv2x-$(VERSION)
	cp Vera.ttf dist/pv2x-$(VERSION)
	cp pv2x.png dist/pv2x-$(VERSION)
	cp README dist/pv2x-$(VERSION)
	cp COPYING dist/pv2x-$(VERSION)
	cp plugins/*.so dist/pv2x-$(VERSION)/plugins
	cp doc/* dist/pv2x-$(VERSION)/doc
	cd dist && tar cjvf pv2x-$(VERSION).tar.bz2 pv2x-$(VERSION)
	cd dist && zip -r pv2x-$(VERSION).zip pv2x-$(VERSION)

install:dist
	mount $(SDMOUNTPOINT)
	cd dist/pv2x-$(VERSION) && cp * $(SDMOUNTPOINT)/$(INSTALLDIR)
	umount $(SDMOUNTPOINT)
