# Who we are
COMPONENTNAME:=mfmtest

# Where's the top
BASEDIR:=../../..

# What we need to build
override INCLUDES += -I $(BASEDIR)/src/core/include -I $(BASEDIR)/src/elements/include -I $(BASEDIR)/src/sim/include -I $(BASEDIR)/src/gui/include -I $(BASEDIR)/src/aui/include -I $(BASEDIR)/src/test/include

# What we need to link
override LIBS += -L $(BASEDIR)/build/core/ -L $(BASEDIR)/build/test/ -L $(BASEDIR)/build/sim/ -L $(BASEDIR)/build/gui/ -L $(BASEDIR)/build/aui/
override LIBS += -lmfmtest -lmfmsim -lmfmcore -lmfmaui -lmfmgui -lSDL2 -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer


# Do the program thing
include $(BASEDIR)/config/Makeprog.mk
