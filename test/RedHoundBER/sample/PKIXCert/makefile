# makefile to build generated files

include ../../../../platform.mk

# include directories
OSROOTDIR = ../../../..
RTLIBDIR  = $(OSROOTDIR)/lib
RTSRCDIR = $(OSROOTDIR)/rtsrc
RTXSRCDIR = $(OSROOTDIR)/rtxsrc
BERSRCDIR = $(OSROOTDIR)/rtbersrc
HFILES = $(RTXSRCDIR)/rtxCommon.h $(RTSRCDIR)/asn1CppTypes.h  \
$(RTSRCDIR)/asn1CSeqOfList.h $(RTSRCDIR)/asn1CBitStr.h \
$(BERSRCDIR)/asn1ber.h $(BERSRCDIR)/asn1BerCppTypes.h 

LIBDIR = ..\..\lib
SRCDIR = ..\..\src

LIBS = $(LIBDIR)/rhber_a.lib $(RTLIBDIR)/ooberrt_a.lib 
LLIBS = rhber_a.lib ooberrt_a.lib 
LPATHS = $(LPPFX)$(LIBDIR) $(LPPFX)$(RTLIBDIR)

CFLAGS = -c $(CVARS_) $(CFLAGS_) $(CBLDTYPE_)
CCFLAGS = $(CFLAGS)
IPATHS = -I$(SRCDIR) -I$(OSROOTDIR) -I$(RTSRCDIR) -I$(BERSRCDIR) $(IPATHS_)
LINKOPT = $(LINKOPT_)

all : writer$(EXE) reader$(EXE)

writer$(EXE) : writer$(OBJ) $(LIBS)
	$(LINK) writer$(OBJ) $(LINKOPT) $(LPATHS) $(LLIBS)

reader$(EXE) : reader$(OBJ) $(LIBS)
	$(LINK) reader$(OBJ) $(LINKOPT) $(LPATHS) $(LLIBS)

RWHFILES = $(SRCDIR)/ridgeback_ber.h $(HFILES)

writer$(OBJ) : writer.cpp $(RWHFILES)
	$(CCC) -c $(CCFLAGS) $(IPATHS) writer.cpp

reader$(OBJ) : reader.cpp $(RWHFILES)
	$(CCC) -c $(CCFLAGS) $(IPATHS) reader.cpp

clean:
	$(RM) *$(OBJ)
	$(RM) *.exe
	$(RM) *.exp
	$(RM) *.pdb
	$(RM) *.map
	$(RM) *.lib
	$(RM) *~
	$(RM) writer$(EXE)
	$(RM) reader$(EXE)
