SOURCES = main.cpp winctl.cpp
TARGET = winctl

CXXFLAGS = -std=c++14 -Wall -O3
CXXFLAGS += ${shell pkg-config --cflags libwnck-3.0}
CXXFLAGS += ${shell pkg-config --cflags gdk-3.0}
CXXFLAGS += ${shell pkg-config --cflags lua}
CXXFLAGS += ${shell pkg-config --cflags x11}

LDFLAGS =
LDFLAGS += ${shell pkg-config --libs libwnck-3.0}
LDFLAGS += ${shell pkg-config --libs gdk-3.0}
LDFLAGS += ${shell pkg-config --libs lua}
LDFLAGS += ${shell pkg-config --libs x11}

OBJS = ${patsubst %.cpp,%.o,${SOURCES}}
DEPS = ${patsubst %.cpp,%.o.dep,${SOURCES}}

${TARGET}: ${OBJS}
	${CXX} -o $@ ${OBJS} ${LDFLAGS}

.cpp.o: $(SOURCES)
	${CXX} ${CXXFLAGS} -c $< -o $@
	@${CXX} ${CXXFLAGS} -MP -MT $@ -MF $@.dep -MM $<

-include $(DEPS)

.PHONY: all clean

all: winctl

clean:
	-rm -f ${TARGET} *.o *.o.dep
