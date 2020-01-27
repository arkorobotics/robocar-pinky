PYTHON		= python
CC		= $(CROSS)gcc
AR		= $(CROSS)ar
CFLAGS		= -Wall -g -fPIC
LDSHFLAGS	= -rdynamic -shared 
ARFLAGS		= rcv
CODE_STYLE	= astyle --align-pointer=name --align-reference=name --suffix=none --break-blocks --pad-oper --pad-header --break-blocks --keep-one-line-blocks --indent-switches --indent=spaces

SOURCES=$(wildcard src/*.c)
HEADERS=$(wildcard src/*.h)
OBJECTS=$(SOURCES:.c=.o)
TARGETS = libi2c.a libi2c.so

.PHONY:all clean example
.SILENT: clean

all:$(TARGETS) example

clean:
	make -C example clean
	find . -name "*.o" | xargs rm -f 
	$(RM) *.o *.so *~ a.out depend $(TARGETS) build -rf

example:$(TARGETS)
	make -C $@

libi2c.a:$(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

libi2c.so:$(OBJECTS)
	$(CC) $(LDSHFLAGS) -o $@ $^

depend:$(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -MM $^ > $@

-include depend
