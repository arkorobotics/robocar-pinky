CC = $(CROSS)gcc
CCX = $(CROSS)g++
AR = $(CROSS)ar

csrc = $(wildcard src/*.c)
ccsrc = $(wildcard src/*.cpp)
obj = $(csrc:.c=.o) $(ccsrc:.cpp=.o)

ARFLAGS = rcv
CFLAGS = -Wall -O3 -g -fPIC -Isrc
CXXFLAGS = -Wall -O3 -g -fPIC -pthread -Isrc
LDFLAGS = -L -li2c -Wl,-R -Wl,.

TARGETS = ctrl

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
