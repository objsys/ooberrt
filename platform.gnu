.SUFFIXES : .c .cpp .o

.cpp.o:
	$(CCC) $(CCFLAGS) -c $(IPATHS) $<

.c.o:
	$(CC) $(CFLAGS) -c $(IPATHS) $<

.y.c:

# Platform specific definitions for Linux/GNU. Assumes 
# GNU GCC 3.x or 2.95.x compiler.

# The following variables are platform specific and may need
# to be edited if moving to a different system.

# compiler defs
CC        = gcc
CCC       = g++
CVARS_    = -DASN1RT -DGNU -D_GNU_SOURCE -DHAVE_STDINT_H -DHAVE_VA_COPY -fPIC
LINKDBG_  = -g
CELF_     = -fPIC
CUSEELF_  = -fpic
CEXCEPT_  = -fexceptions -fpermissive
CVARS0_   = $(CVARS_)
CVARSR_   = $(CVARS_)
CVARSMT_  = $(CVARS_)
CVARSMTR_ = $(CVARS_)
CVARSMTD_ = $(CVARS_)
CVARSMD_  = $(CVARS_)
CVARSMDR_ = $(CVARS_)
CVARSMDD_ = $(CVARS_)
CVARSMTR  = $(CVARS_)
CDEV_     = -D_TRACE
COPTIMIZE0_ = -O2
CSPACEOPT_ = -Os
GCCWARN_ = -Wall -Wpointer-arith -Wextra -Wundef -Wno-unused-parameter -Wshadow -Wcast-align -Wcomments -Wredundant-decls
WERROR = -Werror
# START RELEASE
#BLDSUBDIR  = release
#CFLAGS_    = $(GCCWARN_) -Wnested-externs
#CCFLAGS_   = $(GCCWARN_) -Wno-reorder $(CEXCEPT_)
#COPTIMIZE_ = $(COPTIMIZE0_) -D_OPTIMIZED  
#CDEBUG_    = -g
#LINKOPT_   = -Wl,-Bstatic -o $@
#LINKOPTRLM_ = $(LINKOPT_)
#CBLDTYPE_  = $(COPTIMIZE_)
# END RELEASE

# START DEBUG
BLDSUBDIR  = debug
CDEBUG_    = -g -D_DEBUG -D_TRACE
RTCPPFLAGS = -Wno-non-virtual-dtor
CFLAGS_    = $(CDEBUG_) $(GCCWARN_) -Wnested-externs
CCFLAGS_   = $(CDEBUG_) $(GCCWARN_) -Wno-reorder $(CEXCEPT_)
COPTIMIZE_ = 
LINKOPT_   = $(LINKDBG_) -o $@ -Wl,-Bstatic
LINKOPTRLM_ = $(LINKOPT_)
CBLDTYPE_  = $(CDEBUG_)
# END DEBUG

CCDEBUG_  = $(CDEBUG_)

LIBCMD    = ar r $@
LIBADD    = $(LIBCMD)
LINK      = $(CCC)
LINKSO    = $(LINK)
LINKOPT2  = $(LINKOPT_)
LINKELF_  = -shared $(CELF_) -o $@
LINKELF2_ = $(LINKELF_)
LINKUELF_ = $(CUSEELF_) -o $@
LINKOPTDYN_ = $(LINKUELF_)
COMPACT   = -Os -D_COMPACT
OBJOUT    = -o $@
OUTFILE   = -o

# File extensions
EXE     = 
OBJ     = .o
SO      = .so

# Run-time library
LIBPFX  = lib
LIBEXT  = a
LPPFX   = -L
LLPFX   = -l
LLEXT   =
LLAEXT  = 

A       = .$(LIBEXT)
MTA     = $(A)
MDA     = $(A)
IMP     = $(SO)
DLL     = $(SO)
RTDIRSFX =

# Include and library paths
PS      = /
FS      = :
IPATHS_ = 
CSC     = dmcs

# O/S commands
COPY     = cp -f
MOVE     = mv -f
MV       = $(MOVE)
RM       = rm -f
STRIP    = strip
MAKE     = make
RMDIR    = rm -rf
MKDIR    = mkdir -p

LLOOBERRT = -looberrt
LLSYS   = -Wl,-Bdynamic -lm -lpthread

OOBERRTLIBNAME = libooberrt.a
