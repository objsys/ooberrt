# C to object file compilation rules

$(OBJDIR)$(PS)base64$(OBJ): $(RTXSRCDIR)$(PS)base64.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)base64.c

$(OBJDIR)$(PS)context$(OBJ): $(RTXSRCDIR)$(PS)context.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)context.c

$(OBJDIR)$(PS)datetime$(OBJ): $(RTXSRCDIR)$(PS)datetime.c $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)datetime.c

$(OBJDIR)$(PS)dlist$(OBJ): $(RTXSRCDIR)$(PS)dlist.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)dlist.c

$(OBJDIR)$(PS)errmgmt$(OBJ): $(RTXSRCDIR)$(PS)errmgmt.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)errmgmt.c

$(OBJDIR)$(PS)memmgmt$(OBJ): $(RTXSRCDIR)$(PS)memmgmt.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)memmgmt.c

$(OBJDIR)$(PS)print$(OBJ): $(RTXSRCDIR)$(PS)print.c ../rtsrc/rtPrint.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)systypes.h $(RTXSRCDIR)$(PS)rtxPrint.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)print.c

$(OBJDIR)$(PS)utf8str$(OBJ): $(RTXSRCDIR)$(PS)utf8str.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)utf8str.c

$(OBJDIR)$(PS)utils$(OBJ): $(RTXSRCDIR)$(PS)utils.c $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)systypes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)utils.c
