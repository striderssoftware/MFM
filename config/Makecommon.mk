############ Nothing below here should need to be changed ############ 

include $(BASEDIR)/config/Makevars.mk

#####
# COMPONENT DIRECTORY LAYOUT  
#
# For a COMPONENTNAME foo, we expect to find the following structure:
# foo/Makefile        Makefile to build component foo
# foo/src/            All foo-specific source codes (which other components can't see)
# foo/include/        All foo-specific header files (which other components can see)
#
INCLUDES+=-I include

### TOOL STUFF

LDFLAGS:=

ALLDEP:=$(wildcard $(BASEDIR)/config/*.mk) Makefile   # If config or local makefile changes, nuke it from orbit

ifndef ($(TARGET),cross)
  CFLAGS:=$(NATIVE_GCC_CFLAGS)
  CPPFLAGS:=$(NATIVE_GCC_CPPFLAGS)
  LDFLAGS:=$(NATIVE_GCC_LDFLAGS)
  DEFINES:=$(NATIVE_GCC_DEFINES)
  OPTS:=$(NATIVE_GCC_OPTS)
  DEBUGS:=$(NATIVE_GCC_DEBUGS)
  GCC:=$(NATIVE_GCC)
  GPP:=$(NATIVE_GPP)
  LD:=$(NATIVE_LD)
  OBJCOPY:=$(NATIVE_OBJCOPY)
  ARCH_CODE:=$(NATIVE_ARCH_CODE)
else                    
  CFLAGS:=$(CROSS_GCC_CFLAGS)
  CPPFLAGS:=$(CROSS_GCC_CPPFLAGS)
  LDFLAGS:=$(CROSS_GCC_LDFLAGS)
  DEFINES:=$(CROSS_GCC_DEFINES)
  OPTS:=$(CROSS_GCC_OPTS)
  DEBUGS:=$(CROSS_GCC_DEBUGS)
  GCC:=$(CROSS_GCC)
  GPP:=$(CROSS_GPP)
  LD:=$(CROSS_LD)
  OBJCOPY:=$(CROSS_OBJCOPY)
  ARCH_CODE:=$(CROSS_ARCH_CODE)
endif
CPPFLAGS+=$(INCLUDES) 
CFLAGS+=$(INCLUDES) 

### COMPONENT-SPECIFIC STUFF

BUILDDIR := $(BASEDIR)/build/$(COMPONENTNAME)
ARCHIVEBASENAME := mfm$(COMPONENTNAME)
ARCHIVENAME := lib$(ARCHIVEBASENAME).a
ARCHIVEPATH := $(BUILDDIR)/$(ARCHIVENAME)

### PATTERN RULES	

# Pattern rule for c files
$(BUILDDIR)/%.o:	src/%.c $(ALLDEP) $(BUILDDIR)/%.d
	$(GCC) $(OPTS) $(DEBUGS) $(CFLAGS) $(DEFINES) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"

# Pattern rule for cpp files
$(BUILDDIR)/%.o:	src/%.cpp $(ALLDEP) $(BUILDDIR)/%.d
	$(GPP) $(OPTS) $(DEBUGS) $(CPPFLAGS) $(DEFINES) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"

# Pattern rule for S files
$(BUILDDIR)/%.o:	src/%.S $(ALLDEP) $(BUILDDIR)/%.d
	$(GCC) $(OPTS) $(DEBUGS) $(CFLAGS) $(DEFINES) -c -o"$@" "$<"

