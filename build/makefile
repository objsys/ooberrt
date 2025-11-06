# Build static run-time libraries

include ../platform.mk

OOROOTDIR = ..
OBJDIR = .
LIBDIR = ..$(PS)lib
RTXSRCDIR = $(OOROOTDIR)$(PS)rtxsrc
RTBERSRCDIR = $(OOROOTDIR)$(PS)rtbersrc

COMFLAGS = $(CVARS_) $(MCFLAGS) $(CBLDTYPE_)
CFLAGS	 = $(COMFLAGS) $(CFLAGS_)
IPATHS	 = -I$(OOROOTDIR) $(IPATHS_)
LPATHS	 = $(LPATHS_)

# START RTBUILD
all : common_libs

common_libs : $(LIBDIR)$(PS)$(LIBPFX)ooberrt$(A)

include ../rtxsrc/objects.mk
include ../rtbersrc/objects.mk

OBJECTS = $(RTXOBJECTS) $(RTBEROBJECTS)

$(LIBDIR)$(PS)$(LIBPFX)ooberrt$(A) : $(OBJECTS)
	$(LIBCMD) $(OBJECTS)

include ../rtxsrc/rules.mk
include ../rtbersrc/rules.mk

clean :
	$(RM) *$(OBJ)
	$(RM) $(LIBDIR)$(PS)$(LIBPFX)ooberrt$(A)
	$(RM) *.pdb
	$(RM) *~

