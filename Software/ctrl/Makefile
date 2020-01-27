CC		= $(CROSS)gcc
AR		= $(CROSS)ar
CFLAGS		= -Wall -g -fPIC
LDSHFLAGS	= -rdynamic -shared
ARFLAGS		= rcv
CODE_STYLE	= astyle --align-pointer=name --align-reference=name --suffix=none --break-blocks --pad-oper --pad-header --break-blocks --keep-one-line-blocks --indent-switches --indent=spaces
CFLAGS		+= -Isrc
LDFLAGS		= -L. -li2c -Wl,-R -Wl,.

OBJDIR=build
SOURCES=$(wildcard src/*.c)
HEADERS=$(wildcard src/*.h)
OBJECTS=$(SOURCES:.c=.o)
TARGETS = libi2c.a libi2c.so rc_ctrl

.PHONY:all clean objdir
.SILENT: clean

all:objdir $(TARGETS)

objdir:
	@mkdir -p $(OBJDIR)

clean:
	find . -name "*.o" | xargs rm -f
	$(RM) *.o *.so *~ a.out depend $(TARGETS) $(OBJDIR) build -rf

libi2c.a:$(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

libi2c.so:$(OBJECTS)
	$(CC) $(LDSHFLAGS) -o $@ $^

rc_ctrl: rc_ctrl.o
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ src/$^ $(LDFLAGS)

depend:$(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -MM $^ > $@

-include depend
