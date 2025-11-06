/**
 * Copyright (c) 1997-2025 by Objective Systems, Inc.
 * https://obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   https://obj-sys.com/open/lgpl2.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

#ifndef _RTXCHARSTR_H_
#define _RTXCHARSTR_H_

#include "rtxsrc/rtxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

char* rtxStrcat (char* dest, size_t bufsiz, const char* src);
char* rtxStrncat(char* dest, size_t bufsiz, const char* src, size_t nchars);
char* rtxStrcpy (char* dest, size_t bufsiz, const char* src);
char* rtxStrncpy(char* dest, size_t bufsiz, const char* src, size_t nchars);
char* rtxStrdup (OSCTXT* pctxt, const char* src);

const char* rtxStrJoin
(char* dest, size_t bufsiz, const char* str1, const char* str2,
 const char* str3, const char* str4, const char* str5);

int rtxUIntToCharStr (OSUINT32 value, char* dest, size_t bufsiz, char padchar);

#ifdef __cplusplus
}
#endif

#endif
