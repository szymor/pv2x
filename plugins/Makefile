include ../Makefile.setup

CFLAGS+=-I..

OBJ:=$(addsuffix .so,$(basename $(wildcard *.cpp)))

%.so:%.cpp
	$(CPP) -o $@ -Wall $(DEBUG) -shared -fPIC $(LDFLAGS) $(CFLAGS) $<

all:$(OBJ)

clean:
	rm -f $(OBJ)
