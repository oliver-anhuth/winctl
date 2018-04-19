.PHONY: all clean

CXXFLAGS = -std=c++14 -Wall -O3
CXXFLAGS += $(shell pkg-config --cflags libwnck-3.0)
CXXFLAGS += $(shell pkg-config --cflags gdk-3.0)
CXXFLAGS += $(shell pkg-config --cflags lua)
CXXFLAGS += $(shell pkg-config --cflags x11)

LDFLAGS =
LDFLAGS += $(shell pkg-config --libs libwnck-3.0)
LDFLAGS += $(shell pkg-config --libs gdk-3.0)
LDFLAGS += $(shell pkg-config --libs lua)
LDFLAGS += $(shell pkg-config --libs x11)

winctl: arg_parse.h lua.h winctl.h winctl.cpp main.cpp
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ main.cpp winctl.cpp

all: winctl

clean:
	-rm winctl
