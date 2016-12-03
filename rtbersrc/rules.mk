# C/C++ to object file compilation rules

$(OBJDIR)$(PS)asn1BerCppTypes$(OBJ): $(RTBERSRCDIR)$(PS)asn1BerCppTypes.cpp \
 $(RTBERSRCDIR)$(PS)asn1BerCppTypes.h $(RTSRCDIR)$(PS)asn1CppTypes.h \
 $(RTXSRCDIR)$(PS)OSRTMsgBuf.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h \
 $(RTXSRCDIR)$(PS)OSRTMsgBufIF.h $(RTXSRCDIR)$(PS)OSRTContext.h $(RTSRCDIR)$(PS)ASN1TOctStr.h \
 $(RTSRCDIR)$(PS)ASN1TObjId.h $(RTBERSRCDIR)$(PS)asn1ber.h
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTBERSRCDIR)$(PS)asn1BerCppTypes.cpp

$(OBJDIR)$(PS)decode$(OBJ): $(RTBERSRCDIR)$(PS)decode.c $(RTBERSRCDIR)$(PS)asn1ber.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTBERSRCDIR)$(PS)decode.c

$(OBJDIR)$(PS)encode$(OBJ): $(RTBERSRCDIR)$(PS)encode.c $(RTBERSRCDIR)$(PS)asn1ber.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTBERSRCDIR)$(PS)encode.c
