# Build static run-time libraries

include ../platform.mk

OOROOTDIR = ..
OBJDIR = .
RTXSRCDIR = $(OOROOTDIR)/rtxsrc
RTBERSRCDIR = $(OOROOTDIR)/rtbersrc

COMFLAGS = $(CVARS_) $(MCFLAGS) $(CBLDTYPE_)
CFLAGS	 = $(COMFLAGS) $(CFLAGS_)
IPATHS	 = -I$(OOROOTDIR) $(IPATHS_)
LPATHS	 = $(LPATHS_)

# START RTBUILD 
all : common_libs 

common_libs : ../lib/$(LIBPFX)ooberrt$(A)

include ../rtxsrc/objects.mk
include ../rtbersrc/objects.mk

OBJECTS = $(RTOBJECTS) $(RTXOBJECTS) $(RTBEROBJECTS)

../lib/$(LIBPFX)ooberrt$(A) : $(OBJECTS) 
	$(LIBCMD) $(OBJECTS) 

include ../rtxsrc/rules.mk
include ../rtbersrc/rules.mk

clean :
	$(RM) *$(OBJ) 
	$(RM) *.pdb 
	$(RM) *~ 
