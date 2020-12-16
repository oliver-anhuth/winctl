
SOURCES = main.cpp logger.cpp winctl.cpp
TARGET = winctl

VERSION_MAJOR = 0
VERSION_MINOR = 8
VERSION_PATCH = 1

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
BUILD_TYPE=Release

CXXFLAGS += -std=c++14 -Wall
CXXFLAGS += -DVERSION_MAJOR=${VERSION_MAJOR} -DVERSION_MINOR=${VERSION_MINOR} -DVERSION_PATCH=${VERSION_PATCH}
CXXFLAGS += $(shell pkg-config --cflags libwnck-3.0)
CXXFLAGS += $(shell pkg-config --cflags gdk-3.0)
CXXFLAGS += $(shell pkg-config --cflags lua)

ifeq (${BUILD_TYPE},$(filter ${BUILD_TYPE},Release rel Optimized opt))
CXXFLAGS += -O3
endif
ifeq (${BUILD_TYPE},$(filter ${BUILD_TYPE},Debug dbg))
CXXFLAGS += -g
endif

LDFLAGS += $(shell pkg-config --libs libwnck-3.0)
LDFLAGS += $(shell pkg-config --libs gdk-3.0)
LDFLAGS += $(shell pkg-config --libs lua)

OBJECT_FILES = $(patsubst %.cpp,%.o,${SOURCES})
DEPENDENCIES = $(patsubst %.cpp,%.o.dep,${SOURCES})

# Install to /usr/local/bin/
INSTALL_DIR = /usr/local

# Build rules
.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJECT_FILES}
	${CXX} -o $@ ${OBJECT_FILES} ${LDFLAGS}

${OBJECT_FILES}: ${SOURCES}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@
	@${CXX} ${CXXFLAGS} -MP -MT $@ -MF $@.dep -MM $<

-include $(DEPENDENCIES)

clean:
	-rm -f ${TARGET} *.o *.o.dep

install:
	install --mode u=rwx,g=rx,o=rx --preserve-timestamps ${TARGET} ${INSTALL_DIR}/bin/${TARGET}
