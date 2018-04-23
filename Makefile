
SOURCES = main.cpp logger.cpp winctl.cpp
TARGET = winctl

# Check prerequisites
ifeq ($(shell which ${CXX}),)
$(error No C++ compiler found)
endif

ifeq ($(shell which pkg-config),)
$(error No pkg-config found)
endif

ifeq ($(shell pkg-config --modversion libwnck-3.0),)
$(error No development package for libwnck-3.0 found)
endif

ifeq ($(shell pkg-config --modversion gdk-3.0),)
$(error No development package for gdk-3.0 found)
endif

ifeq ($(shell pkg-config --modversion lua),)
$(error No development package for lua found)
endif

# Set up
CXXFLAGS += -std=c++14 -Wall -O3
CXXFLAGS += $(shell pkg-config --cflags libwnck-3.0)
CXXFLAGS += $(shell pkg-config --cflags gdk-3.0)
CXXFLAGS += $(shell pkg-config --cflags lua)

LDFLAGS += $(shell pkg-config --libs libwnck-3.0)
LDFLAGS += $(shell pkg-config --libs gdk-3.0)
LDFLAGS += $(shell pkg-config --libs lua)

OBJS = $(patsubst %.cpp,%.o,${SOURCES})
DEPS = $(patsubst %.cpp,%.o.dep,${SOURCES})

# Build rules
.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJS}
	${CXX} -o $@ ${OBJS} ${LDFLAGS}

.cpp.o: $(SOURCES)
	${CXX} ${CXXFLAGS} -c $< -o $@
	@${CXX} ${CXXFLAGS} -MP -MT $@ -MF $@.dep -MM $<

-include $(DEPS)

clean:
	-rm -f ${TARGET} *.o *.o.dep

install:
	install --mode u=rwx,g=rx,o=rx --preserve-timestamps ${TARGET} /usr/local/bin/${TARGET}
