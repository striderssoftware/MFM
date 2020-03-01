# Who we are
COMPONENTNAME:=aui

# Where's the top
BASEDIR:=../..

# What we need from outside

# Build the MicMonitor exe and put it somewhere
# TODO VDT get this working so MicMonitor will be built as part of building the MFM
#include $(BASEDIR)/src/aui/MonitorMake.mk   

# Do the library thing
include $(BASEDIR)/config/Makelib.mk

.PHONY = MicMonitor

MicMonitor:
	make -f MonitorMake.mk

all: MicMonitor

