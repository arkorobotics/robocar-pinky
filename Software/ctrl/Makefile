CC = $(CROSS)gcc
CCX = $(CROSS)g++
AR = $(CROSS)ar

csrc = $(wildcard src/*.c)
ccsrc = $(wildcard src/*.cpp)
obj = $(csrc:.c=.o) $(ccsrc:.cpp=.o)

ARFLAGS = rcv
CFLAGS = -Wall -g -fPIC -Isrc
CXXFLAGS = -Wall -g -fPIC -lpthread -Isrc
LDFLAGS = -L -li2c -Wl,-R -Wl,.

TARGETS = ctrl #libi2c.a libi2c.so ctrl

.PHONY: all clean obj
.SILENT: clean

all: obj $(TARGETS)

clean:
	find . -name "*.o" | xargs rm -f
	$(RM) *.o *.so *~ a.out depend $(TARGETS) -rf

ctrl:$(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

depend:$(csrc) $(ccsrc)
	$(CC) $(CFLAGS) -MM $^ > $@

-include depend



#CC		= $(CROSS)gcc
#AR		= $(CROSS)ar
#CFLAGS		= -Wall -g -fPIC -lpthread
#LDSHFLAGS	= -rdynamic -shared
#ARFLAGS		= rcv
#CODE_STYLE	= astyle --align-pointer=name --align-reference=name --suffix=none --break-blocks --pad-oper --pad-header --break-blocks --keep-one-line-blocks --indent-switches --indent=spaces
#CFLAGS		+= -Isrc
#LDFLAGS		= -L$(OBJDIR) -li2c -Wl,-R -Wl,.

#OBJDIR=build
#SRCDIR=src
#SOURCES=$(wildcard src/*.c)
#HEADERS=$(wildcard src/*.h)
#OBJECTS=$(SOURCES:.c=.o)
#TARGETS = libi2c.a libi2c.so ctrl

#.PHONY:all clean objdir
#.SILENT: clean

#all:objdir $(TARGETS)

#objdir:
#	@mkdir -p $(OBJDIR)

#clean:
#	find . -name "*.o" | xargs rm -f
#	$(RM) *.o *.so *~ a.out depend $(TARGETS) $(OBJDIR) build -rf

#libi2c.a:$(OBJECTS)
#	$(AR) $(ARFLAGS) $(OBJDIR)/$@ $^

#libi2c.so:$(OBJECTS)
#	$(CC) $(LDSHFLAGS) -o $(OBJDIR)/$@ $^

#ctrl:$(OBJECTS)
#	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ $^ $(LDFLAGS)

#depend:$(SOURCES) $(HEADERS)
#	$(CC) $(CFLAGS) -MM $^ > $@

#-include depend
