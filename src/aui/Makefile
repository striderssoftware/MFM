# Who we are
COMPONENTNAME:=aui

# Where's the top
BASEDIR:=../..

# What we need from outside
override INCLUDES += $(shell sdl2-config --cflags) -I $(BASEDIR)/src/core/include 
override LIBS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image

# Do the library thing
include $(BASEDIR)/config/Makelib.mk
