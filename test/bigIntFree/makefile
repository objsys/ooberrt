.SUFFIXES : .c .cc .cpp .obj

.cpp.obj:
	$(CCC) $(CCFLAGS) -c $(IPATHS) $<

CCC       = cl
CCFLAGS   = -I ../../src -GX -W3 -Zi -D_DEBUG -MLd
LINK      = cl
LINKOPT0  = /nologo /link /OUT:$@ /OPT:REF  
LINKDBG_  = /MAP /DEBUG:FULL /DEBUGTYPE:BOTH
LINKOPT   = $(LINKOPT0) $(LINKDBG_)
LPATHS    = -LIBPATH:..\..\lib
OBJ       = .obj

bigIntTest.exe : bigIntTest.obj ../../lib/ooberrt_a.lib
	$(LINK) bigIntTest.obj $(LINKOPT) $(LPATHS) ooberrt_a.lib

bigIntTest.obj : bigIntTest.cpp

clean :
	$(RM) *.exe
	$(RM) *.obj
	$(RM) *.pdb
	$(RM) *~
