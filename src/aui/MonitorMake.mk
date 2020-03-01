APPNAME = MicMonitor
OFILES = $(BUILDDIR)/MicMonitor.o 
ALLDEP = Makefile

CC = g++
CCFLAGS = -g -Wall -pedantic -std=gnu++17
CXXFLAGS =
INCL = 
LDFLAGS =
LIBS =


# A makefile comment

ifndef COMPONENTNAME
	COMPONENTNAME := aui
endif

ifndef BASEDIR
        BASEDIR := ../../
endif


ifndef BUILDDIR
        BUILDDIR := $(BASEDIR)/build/$(COMPONENTNAME)
endif

all: $(BUILDDIR) $(BUILDDIR)/$(APPNAME)

$(BUILDDIR)/$(APPNAME): $(ALLDEP) $(OFILES)
	$(CC) $(CCFLAGS) $(CXXFLAGS) $(OFILES) -o $@  $(LDFLAGS) $(LIBS)

$(BUILDDIR)/%.o: src/%.cpp
	$(CC) $(CCFLAGS) $(CXXFLAGS) $(INCL) -c -o $@ $<

clean:
	rm -rf $(BASEDIR)build $(BUILDDIR)

.PHONY: bb

bb:
	mkdir -p $(BASEDIR)build

$(BUILDDIR): bb 
	mkdir -p $(BUILDDIR)

