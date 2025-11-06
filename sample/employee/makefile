# makefile to build sample program

include ../../platform.mk

OOROOTDIR = ..$(PS)..
BERSRCDIR = $(OOROOTDIR)$(PS)rtbersrc
RTXSRCDIR = $(OOROOTDIR)$(PS)rtxsrc

# This must be set to path of ASN1C compiler
ASN1C = ..$(PS)..$(PS)..$(PS)bin$(PS)asn1c$(EXE)

CFLAGS = $(CBLDTYPE_) $(CVARS_) $(MCFLAGS) $(CFLAGS_)
IPATHS = -I. -I$(OOROOTDIR)

OOBERRTLIBNAME = $(LIBPFX)ooberrt$(A)

all : writer$(EXE) reader$(EXE)

HFILES = $(RTXSRCDIR)$(PS)rtxCommon.h $(BERSRCDIR)$(PS)asn1ber.h

LIBDIR2 = $(OOROOTDIR)$(PS)lib
LPATHS = $(LPPFX)$(LIBDIR2) $(LPATHS_)

writer$(EXE) : writer$(OBJ) Employee$(OBJ) EmployeeEnc$(OBJ) \
$(LIBDIR2)$(PS)$(OOBERRTLIBNAME)
	$(LINK) writer$(OBJ) Employee$(OBJ) EmployeeEnc$(OBJ) $(LINKOPT_) \
	  $(LPATHS) $(LLOOBERRT) $(LLSYS)

reader$(EXE) : reader$(OBJ) Employee$(OBJ) EmployeeDec$(OBJ) \
EmployeePrint$(OBJ) $(LIBDIR2)$(PS)$(OOBERRTLIBNAME)
	$(LINK) reader$(OBJ) Employee$(OBJ) EmployeeDec$(OBJ) \
	  EmployeePrint$(OBJ) $(LINKOPT_) $(LPATHS) $(LLOOBERRT) $(LLSYS)

writer$(OBJ) : writer.c Employee.h $(HFILES)
reader$(OBJ) : reader.c Employee.h $(HFILES)

Employee$(OBJ) : Employee.c Employee.h $(HFILES)
EmployeeEnc$(OBJ) : EmployeeEnc.c Employee.h $(HFILES)
EmployeeDec$(OBJ)  : EmployeeDec.c Employee.h $(HFILES)
EmployeePrint$(OBJ)  : EmployeePrint.c Employee.h $(HFILES)

Employee.c EmployeeEnc.c EmployeeDec.c EmployeePrint.c Employee.h : \
employee.asn $(ASN1C)
	$(ASN1C) employee.asn -c -ber -print

clean:
	$(RM) Employee*.c
	$(RM) Employee.h
	$(RM) *.dat
	$(RM) *$(OBJ)
	$(RM) writer$(EXE)
	$(RM) reader$(EXE)
	$(RM) *.exp
	$(RM) *.pdb
	$(RM) *.map
	$(RM) *.lib
	$(RM) *~
	$(RM) rtkey.h

