# makefile to build generated files

include ../../platform.mk

OOROOTDIR = ..$(PS)..
BERSRCDIR = $(OOROOTDIR)$(PS)rtbersrc
RTXSRCDIR = $(OOROOTDIR)$(PS)rtxsrc
SRCDIR = .
HFILESDIR = .

# This must be set to path of ASN1C compiler
ASN1C = ..$(PS)..$(PS)..$(PS)bin$(PS)asn1c$(EXE)

CFLAGS = $(CBLDTYPE_) $(CVARS_) $(MCFLAGS) $(CFLAGS_)
IPATHS = -I. -I$(OOROOTDIR)

OOBERRTLIBNAME = $(LIBPFX)ooberrt$(A)

OBJECTS = AllTypes$(OBJ)
ENCOBJECTS = AllTypesEnc$(OBJ)
DECOBJECTS = AllTypesDec$(OBJ)
PRINTOBJECTS = AllTypesPrint$(OBJ)

LIBDIR2 = $(OOROOTDIR)$(PS)lib
LPATHS = $(LPPFX)$(LIBDIR2) $(LPATHS_)

HFILES = $(RTXSRCDIR)$(PS)rtxCommon.h $(BERSRCDIR)$(PS)asn1ber.h

all : $(OBJECTS) $(ENCOBJECTS) $(DECOBJECTS) $(PRINTOBJECTS) \
writer$(EXE) reader$(EXE)

WRITEROBJ = writer$(OBJ) $(OBJECTS) $(ENCOBJECTS)

writer$(EXE) : $(WRITEROBJ) $(LIBDIR2)$(PS)$(OOBERRTLIBNAME)
	$(LINK) $(WRITEROBJ) $(LINKOPT_) $(LPATHS) $(LLOOBERRT) $(LLSYS)

READEROBJ = reader$(OBJ) $(OBJECTS) $(DECOBJECTS) $(PRINTOBJECTS)

reader$(EXE) : $(READEROBJ) $(LIBS)
	$(LINK) $(READEROBJ) $(LINKOPT_) $(LPATHS) $(LLOOBERRT) $(LLSYS)

RWHFILES = $(HFILESDIR)$(PS)AllTypes.h \
$(HFILESDIR)$(PS)rtkey.h \
$(HFILES)

writer$(OBJ) : $(SRCDIR)$(PS)writer.c $(RWHFILES)
	$(CC) -c $(CFLAGS) $(IPATHS) $(SRCDIR)$(PS)writer.c

reader$(OBJ) : $(SRCDIR)$(PS)reader.c $(RWHFILES)
	$(CC) -c $(CFLAGS) $(IPATHS) $(SRCDIR)$(PS)reader.c

# .c -> .obj rules
# common file rules
AllTypes$(OBJ) : $(SRCDIR)$(PS)AllTypes.c \
$(HFILESDIR)$(PS)AllTypes.h \
$(HFILESDIR)$(PS)rtkey.h \
$(HFILES)
	$(CC) $(CFLAGS) -c $(IPATHS) $(SRCDIR)$(PS)AllTypes.c

# decode file rules
AllTypesDec$(OBJ) : $(SRCDIR)$(PS)AllTypesDec.c \
$(HFILESDIR)$(PS)AllTypes.h \
$(HFILESDIR)$(PS)rtkey.h $(HFILES)
	$(CC) $(CFLAGS) -c $(IPATHS) $(SRCDIR)$(PS)AllTypesDec.c

# encode file rules
AllTypesEnc$(OBJ) : $(SRCDIR)$(PS)AllTypesEnc.c \
$(HFILESDIR)$(PS)AllTypes.h \
$(HFILESDIR)$(PS)rtkey.h $(HFILES)
	$(CC) $(CFLAGS) -c $(IPATHS) $(SRCDIR)$(PS)AllTypesEnc.c

# print file rules
AllTypesPrint$(OBJ) : $(SRCDIR)$(PS)AllTypesPrint.c \
$(HFILESDIR)$(PS)AllTypes.h $(HFILES)
	$(CC) $(CFLAGS) -c $(IPATHS) $(SRCDIR)$(PS)AllTypesPrint.c

# .asn -> .c .h rules
$(SRCDIR)$(PS)AllTypesEnc.c \
$(SRCDIR)$(PS)AllTypesDec.c \
$(SRCDIR)$(PS)AllTypesPrint.c \
$(SRCDIR)$(PS)AllTypes.c \
$(HFILESDIR)$(PS)AllTypes.h \
$(HFILESDIR)$(PS)rtkey.h \
: allTypes.asn $(ASN1C)
	$(ASN1C) allTypes.asn -c -ber -print

clean:
	$(RM) AllTypesEnc.c
	$(RM) AllTypesDec.c
	$(RM) AllTypesPrint.c
	$(RM) AllTypes.c
	$(RM) $(HFILESDIR)$(PS)AllTypes.h
	$(RM) $(HFILESDIR)$(PS)rtkey.h
	$(RM) $(HFILESDIR)$(PS)rtkey.h
	$(RM) *$(OBJ)
	$(RM) *~
	$(RM) reader$(EXE)
	$(RM) writer$(EXE)
	$(RM) message.dat
	$(RM_MSVC_FILES)
