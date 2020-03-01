# Who we are
COMPONENTNAME:=mfmsim

# Where's the top
BASEDIR:=../../..

# What we need to build
override INCLUDES += -I $(BASEDIR)/src/core/include -I $(BASEDIR)/src/elements/include -I $(BASEDIR)/src/sim/include -I $(BASEDIR)/src/gui/include -I $(BASEDIR)/src/aui/include
override INCLUDES += $(shell sdl-config --cflags)

# What we need to link
override LIBS += -L $(BASEDIR)/build/core/ -L $(BASEDIR)/build/sim/ -L $(BASEDIR)/build/gui/ -L $(BASEDIR)/build/aui/
override LIBS += -lmfmgui -lmfmsim -lmfmcore -lmfmaui -lSDL -lSDL_ttf -lSDL_image -lpng -lm

# Do the program thing
include $(BASEDIR)/config/Makeprog.mk
