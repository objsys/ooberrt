/**
 * Copyright (c) 1997-2025 by Objective Systems, Inc.
 * http://www.obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/lgpl2.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/rtxCharStr.h"

const signed char gDaysInMonth[12] =
   { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

OSBOOL rtxDateIsValid (const OSNumDateTime* dateTime)
{
   int dim;
   if (dateTime->year < 0 || dateTime->year > 9999 ||
       dateTime->mon < 1 || dateTime->mon > 12 ||
       dateTime->day < 1 || dateTime->day > 31 ||
       (dateTime->tz_flag && (dateTime->tzo < -840 || dateTime->tzo > 840)) )
      return FALSE;

   dim = (int) gDaysInMonth [dateTime->mon - 1];

   /* check for February of leap-year */
   if (dateTime->mon == 2 && dateTime->year % 4 == 0 &&
       (dateTime->year % 100 != 0 || dateTime->year % 400 == 0))
      dim++;

   if (dateTime->day < 1 || dateTime->day > dim)
      return FALSE;

   return TRUE;
}

OSBOOL rtxTimeIsValid (const OSNumDateTime* dateTime)
{
   if (dateTime->hour > 23 ||
       dateTime->min > 59 ||
       dateTime->sec < 0 || dateTime->sec >= 60 ||
       (dateTime->tz_flag && (dateTime->tzo < -840 || dateTime->tzo > 840)))
      return FALSE;

   return TRUE;
}


OSBOOL rtxDateTimeIsValid (const OSNumDateTime* dateTime)
{
   return (OSBOOL)(rtxDateIsValid (dateTime) && rtxTimeIsValid (dateTime));
}

#define SEC_PRECISION 0.0000005

static void normalizeTimeZone_static(OSNumDateTime*);

 /**
  * rtxCmpDate:
  * This function compares date part of two OSNumDateTime structure and
  * return the result as integer.
  */
int rtxCmpDate (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2)
{

   int stat = 0;
   if (pvalue1->year > pvalue2->year) {
      stat = 1;
   } else if (pvalue1->year < pvalue2->year) {
      stat = -1;
   } else {
      if (pvalue1->mon > pvalue2->mon) {
         stat = 1;
      } else if (pvalue1->mon < pvalue2->mon) {
         stat = -1;
      } else {
         if (pvalue1->day > pvalue2->day) {
            stat = 1;
         } else if (pvalue1->day < pvalue2->day) {
            stat = -1;
         } else {
            stat = 0;
         }
      }
   }
   return stat;
}

/**
 * rtxCmpTime:
 * This function compares time part of two OSNumDateTime structure and
 * return the result as integer.
 */
int rtxCmpTime (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2)
{
   int stat = 0;
   if (pvalue1->hour  > pvalue2->hour) {
      stat = 1;
   } else if (pvalue1->hour < pvalue2->hour) {
      stat = -1;
   } else {
      if (pvalue1->min > pvalue2->min) {
         stat = 1;
      } else if (pvalue1->min < pvalue2->min) {
         stat = -1;
      } else {
         if (pvalue1->sec > pvalue2->sec + SEC_PRECISION) {
            stat = 1;
         } else if (pvalue1->sec < pvalue2->sec  - SEC_PRECISION) {
            stat = -1;
         } else {
            stat = 0;
         }
      }
   }
   return stat;
}

/**
 * rtxCmpDateTime:
 * This function compares date and time  of two OSNumDateTime structure and
 * return the result as integer.
 */
int rtxCmpDateTime (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2)
{
   int stat = 0;
   OSNumDateTime tmpvalue1, tmpvalue2;
   if (pvalue1->tz_flag == (OSBOOL)TRUE) {
      memcpy (&tmpvalue1, pvalue1, sizeof(OSNumDateTime));
      normalizeTimeZone_static (&tmpvalue1);
      pvalue1 = &tmpvalue1;
   }
   if (pvalue2->tz_flag == (OSBOOL)TRUE) {
      memcpy (&tmpvalue2, pvalue2, sizeof(OSNumDateTime));
      normalizeTimeZone_static (&tmpvalue2);
      pvalue2 = &tmpvalue2;
   }

   stat = rtxCmpDate(pvalue1, pvalue2);
   if (stat == 0)  stat = rtxCmpTime(pvalue1, pvalue2);
   return stat;
}

/**
 * normalizeTimeZone:
 * This function normalizes the Time Zone value of OSNumDateTime structure.
 */
#define IS_LEAP(y) (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
#define FQUOTIENT(a,b)  (int)(floor((double)a/(double)b))
#define FQUOTIENT_RANGE(a,low,high)  (FQUOTIENT((a-low),(high-low)))
#define MODULO(a,b)     (a - (FQUOTIENT(a,b))*b)
#define MODULO_RANGE(a,low,high) ((MODULO((a-low),(high-low)))+low)
#define MAX_DAY_INMONTH(yr,mon) \
((IS_LEAP(yr)&& mon == 2)? (gDaysInMonth[mon-1] + 1) : gDaysInMonth[mon-1])

static void normalizeTimeZone_static(OSNumDateTime* pvalue)
{
   OSINT32 tmp, carry, tmpdays;
   tmp = pvalue->min - pvalue->tzo;
   pvalue->min = (OSINT8)(MODULO(tmp,60));
   carry = FQUOTIENT(tmp,60);
   tmp = pvalue->hour + carry;
   pvalue->hour = (OSINT8)(MODULO(tmp,24));
   carry = FQUOTIENT(tmp,24);
   tmpdays = pvalue->day + carry;

   for (;;) {
      if(tmpdays < 1){
         int tmon = MODULO_RANGE(pvalue->mon-1, 1, 13);
         int tyr  = pvalue->year + FQUOTIENT_RANGE(pvalue->mon-1, 1, 13);
         if (tyr == 0)
            tyr--;
         tmpdays += MAX_DAY_INMONTH(tyr, tmon);
         carry = -1;
      } else if (tmpdays > MAX_DAY_INMONTH(pvalue->year, pvalue->mon)){
         tmpdays = tmpdays - MAX_DAY_INMONTH(pvalue->year, pvalue->mon);
         carry = 1;
      } else
         break;
      tmp = pvalue->mon + carry;
      pvalue->mon = (OSINT8)(MODULO_RANGE(tmp,1,13));
      pvalue->year = (OSINT8)(pvalue->year + FQUOTIENT_RANGE(tmp,1,13));
   }
   pvalue->day = (OSINT8)tmpdays;
}

static OSINT32 digitCharsToInt (const char* digits, OSSIZE ndigits, int* pvalue)
{
   OSSIZE i;
   *pvalue = 0;
   for (i = 0; i < ndigits; i++) {
      if (OS_ISDIGIT(digits[i])) *pvalue = *pvalue * 10 + (digits[i] - '0');
      else return RTERR_BADVALUE;
   }
   return 0;
}

static OSINT32 parseYYMMDD (const char* str, int* pyear, int* pmonth, int* pday)
{
   int ret;
   *pyear = *pmonth = *pday = 0;
   ret = digitCharsToInt (str, 2, pyear);
   if (0 == ret) {
      str += 2;
      ret = digitCharsToInt (str, 2, pmonth);
   }
   if (0 == ret) {
      str += 2;
      ret = digitCharsToInt (str, 2, pday);
   }
   return ret;
}

static OSINT32 parseYYYYMMDD (const char* str, int* pyear, int* pmonth, int* pday)
{
   int ret;
   *pyear = *pmonth = *pday = 0;
   ret = digitCharsToInt (str, 4, pyear);
   if (0 == ret) {
      str += 4;
      ret = digitCharsToInt (str, 2, pmonth);
   }
   if (0 == ret) {
      str += 2;
      ret = digitCharsToInt (str, 2, pday);
   }
   return ret;
}

#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
#    define SNPRINTF(str, size, ...) \
     _snprintf_s(str, size, _TRUNCATE, __VA_ARGS__)
#  else
#    define SNPRINTF _snprintf
#  endif
#else
#define SNPRINTF snprintf
#endif

OSINT32 rtMakeGeneralizedTime
(OSCTXT* pctxt, const OSNumDateTime* dateTime,
 char** outdata, size_t outdataSize)
{
   char dateTimeStr[100];
   size_t dateTimeStrLen;
   double frac;

   if (!rtxDateTimeIsValid (dateTime))
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   /* YYYYMMDDHH[MM[SS[(.|,)SSSS]]][TZD] */
   SNPRINTF (dateTimeStr, 100, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
            dateTime->year, dateTime->mon, dateTime->day,
            dateTime->hour, dateTime->min, (int)dateTime->sec);

   frac = dateTime->sec - (int)dateTime->sec;
   if (frac > 0.00000001) {
      char tmpbuf[32]; int i, len;
      SNPRINTF (tmpbuf, 32, "%.6f", frac);
      len = (int) OSCRTLSTRLEN (tmpbuf);

      /* cut trailing zeros off frac part */
      for (i = len - 1; i > 0; i--) {
         if (tmpbuf [i] == '0')
            tmpbuf [i] = '\0';
         else
            break;
      }

      if (i > 1) {
         rtxStrncat (dateTimeStr, 100, ".", 1);
         rtxStrncat (dateTimeStr, 100, tmpbuf + 2, len - 2);
      }
   }

   if (dateTime->tz_flag && dateTime->tzo == 0) {
      rtxStrncat (dateTimeStr, 100, "Z", 1);
   }
   else if (dateTime->tz_flag) {
      char tmpbuf[32];
      char tzd = (dateTime->tzo < 0) ? '-' : '+';
      int  tzo = dateTime->tzo;
      if (tzo < 0) tzo *= -1;
      SNPRINTF (tmpbuf, 32, "%c%02d%02d", tzd, tzo/60, tzo%60);
      rtxStrncat (dateTimeStr, 100, tmpbuf, 5);
   }

   dateTimeStrLen = OSCRTLSTRLEN (dateTimeStr);
   if (outdataSize == 0) {
      *outdata = (char*) rtxMemAlloc (pctxt, dateTimeStrLen+1);
      if (0 == *outdata)
         return LOG_RTERR (pctxt, RTERR_NOMEM);

      outdataSize = dateTimeStrLen+1;
   }
   else {
      if (outdataSize <= dateTimeStrLen)
         return LOG_RTERR (pctxt, RTERR_STROVFLW);
   }
   rtxStrncpy (*outdata, outdataSize, dateTimeStr, dateTimeStrLen);

   return (0);
}


OSINT32 rtMakeUTCTime
(OSCTXT* pctxt, const OSNumDateTime* dateTime,
 char** outdata, size_t outdataSize)
{
   char dateTimeStr[100];
   size_t dateTimeStrLen;

   if (!rtxDateTimeIsValid (dateTime))
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   /* Year is more restricted in UTC time */
   if (dateTime->year < 1950 || dateTime->year > 2049)
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   /* YYYYMMDDHH[MM[SS[(.|,)SSSS]]][TZD] */
   SNPRINTF (dateTimeStr, 100, "%.2d%.2d%.2d%.2d%.2d",
            dateTime->year%100, dateTime->mon, dateTime->day,
            dateTime->hour, dateTime->min);

   if (dateTime->sec != 0 || (pctxt->flags & ASN1DER) != 0 )
   {
      char tmpstr[32];
      SNPRINTF (tmpstr, 32, "%.2d", (int)dateTime->sec);
      rtxStrncat (dateTimeStr, 100, tmpstr, 2);
   }

   /* the tz_flag should always be set for a UTCTime. */
   if ( !dateTime->tz_flag )
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   if (dateTime->tzo == 0) {
      rtxStrncat (dateTimeStr, 100, "Z", 1);
   }
   else {
      char tmpbuf[32];
      char tzd = (dateTime->tzo < 0) ? '-' : '+';
      int  tzo = dateTime->tzo;
      if (tzo < 0) tzo *= -1;
      SNPRINTF (tmpbuf, 32, "%c%02d%02d", tzd, tzo/60, tzo%60);
      rtxStrncat (dateTimeStr, 100, tmpbuf, 5);
   }

   dateTimeStrLen = OSCRTLSTRLEN (dateTimeStr);
   if (outdataSize == 0) {
      *outdata = (char*) rtxMemAlloc (pctxt, dateTimeStrLen+1);
      if (0 == *outdata)
         return LOG_RTERR (pctxt, RTERR_NOMEM);

      outdataSize = dateTimeStrLen+1;
   }
   else {
      if (outdataSize <= dateTimeStrLen)
         return LOG_RTERR (pctxt, RTERR_STROVFLW);
   }
   rtxStrncpy (*outdata, outdataSize, dateTimeStr, dateTimeStrLen);

   return (0);
}

void normalizeTimeZone(OSNumDateTime* pvalue)
{
   OSINT32 tmp, carry, tmpdays;
   tmp = pvalue->min - pvalue->tzo;
   pvalue->min = (OSINT8)(MODULO(tmp,60));
   carry = FQUOTIENT(tmp,60);
   tmp = pvalue->hour + carry;
   pvalue->hour = (OSINT8)(MODULO(tmp,24));
   carry = FQUOTIENT(tmp,24);
   tmpdays = pvalue->day + carry;

   for (;;) {
      if(tmpdays < 1){
         int tmon = MODULO_RANGE(pvalue->mon-1, 1, 13);
         int tyr  = pvalue->year + FQUOTIENT_RANGE(pvalue->mon-1, 1, 13);
         if (tyr == 0)
            tyr--;
         tmpdays += MAX_DAY_INMONTH(tyr, tmon);
         carry = -1;
      } else if (tmpdays > MAX_DAY_INMONTH(pvalue->year, pvalue->mon)){
         tmpdays = tmpdays - MAX_DAY_INMONTH(pvalue->year, pvalue->mon);
         carry = 1;
      } else
         break;
      tmp = pvalue->mon + carry;
      pvalue->mon = (OSINT8)(MODULO_RANGE(tmp,1,13));
      pvalue->year = (OSINT32)(pvalue->year + FQUOTIENT_RANGE(tmp,1,13));
   }
   pvalue->day = (OSINT8)tmpdays;
   pvalue->tzo = 0;
}

#define LOG_CANON(msg) { \
   rtxErrAddStrParm(pctxt, msg); \
   LOG_RTERRNEW(pctxt, ASN_E_NOTCANON); }


OSINT32 rtParseGeneralizedTime2 (OSCTXT *pctxt,
                            const char* value,
                            OSNumDateTime* dateTime,
                            OSBOOL logCanon)
{
   int year, month, day, hour, minute, second, diffhour, diffmin;
   OSREAL fracsec = 0.0, mult = 0.1;
   char tzd;
   const char* p = value;
   OSBOOL canon = (pctxt->flags & ASN1DER ) != 0;
   OSBOOL notcanon = FALSE;   /* true if value is non-canonical */
   OSBOOL fixMidnight = FALSE;

   year = month = day = hour = RTERR_IDNOTFOU;
   minute = second = diffhour = diffmin = 0;

   if (parseYYYYMMDD (p, &year, &month, &day) == 0) {
      int n = 0;
      p += 8;

      if (OS_ISDIGIT(*p) && 0 == digitCharsToInt (p, 2, &hour)) {
         p += 2;
         n++;
      }
      else
         return LOG_RTERR(pctxt, RTERR_INVFORMAT);

      if (OS_ISDIGIT(*p) && 0 == digitCharsToInt (p, 2, &minute)) {
         p += 2;
         n++;
         if(OS_ISDIGIT(*p) && 0 == digitCharsToInt (p, 2, &second)) {
            p += 2;
            n++;
         }
      }

      if ( canon && n < 3 ) {
         if ( logCanon ) LOG_CANON("seconds is required");
         notcanon = TRUE;
      }

      if (*p == '.' || *p == ',') { /* fraction */
         int j;      /* count of fractional digits */

         if ( canon && *p == ',') {
            if ( logCanon ) LOG_CANON("must use '.' for fraction");
            notcanon = TRUE;
         }

         p++;

         /* parse next fraction second */
         for (j = 0; OS_ISDIGIT(*p); j++, p++) {
            fracsec += ((*p - '0') * mult);
            mult /= 10.0;
         }
         if (j == 0)
            return LOG_RTERR(pctxt, RTERR_INVFORMAT);

         if (canon && *(p-1) == '0') {
            if ( logCanon ) LOG_CANON("trailing zeros in fraction disallowed");
            notcanon = TRUE;
         }

         /* convert frational hours/mins to frac seconds */
         if (n == 1) {
            fracsec *= 60;    /* was hours; now is minutes */
            if (fracsec >= 1) {
               minute = (int) fracsec;
               fracsec -= minute;
            }
         }

         if ( n < 3 ) {
            fracsec *= 60;    /* was minutes; now is seconds */
            if (fracsec >= 1) {
               second = (int)fracsec;
               fracsec -= second;
            }
         }
      }
      dateTime->tz_flag = FALSE;
      tzd = *p;
      if (tzd == 'Z') { /* utc */
         dateTime->tz_flag = TRUE;
         p++;
         if(*p != 0)
            return LOG_RTERR(pctxt, RTERR_INVFORMAT);
      }
      else if (tzd == '-' || tzd == '+') {
         if ( canon ) {
            if (logCanon) LOG_CANON("must use Z");
            notcanon = TRUE;
         }

         dateTime->tz_flag = TRUE;
         p++;
         if(!OS_ISDIGIT(*p) || 0 != digitCharsToInt (p, 2, &diffhour))
            return LOG_RTERR(pctxt, RTERR_INVFORMAT);
         p += 2;
         if(diffhour < 0 || diffhour > 13)
            return LOG_RTERR(pctxt, RTERR_INVFORMAT);
         if(OS_ISDIGIT(*p)) {
            if(digitCharsToInt (p, 2, &diffmin) != 0)
               return LOG_RTERR(pctxt, RTERR_INVFORMAT);
            if(diffmin < 0 || diffmin > 59)
               return LOG_RTERR(pctxt, RTERR_INVFORMAT);
         }
      }
      else if ( canon ) {
         if (logCanon) LOG_CANON("must use Z");
         notcanon = TRUE;
      }
   }
   else
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   dateTime->year = (OSINT32)year;
   dateTime->mon = (OSUINT8)month;
   dateTime->day = (OSUINT8)day;
   dateTime->hour = (OSUINT8)hour;
   dateTime->min = (OSUINT8)minute;
   dateTime->sec = (OSREAL)second + fracsec;
   dateTime->tzo = (diffhour * 60) + diffmin;
   if (tzd == '-') dateTime->tzo *= -1;

   if (dateTime->hour == 24 && dateTime->min == 0 && dateTime->sec == 0)
   {
      /* 240000 is valid for midnight.  Change it to 00, makes sure fields
         are otherwise valid, then add a day. */
      fixMidnight = TRUE;
      dateTime->hour = 0;
      if ( canon ) {
         if (logCanon) LOG_CANON("Must use 00 for midnight");
         notcanon = TRUE;
      }
   }

   if (!rtxDateTimeIsValid (dateTime))
      return LOG_RTERR (pctxt, RTERR_INVFORMAT);

   if (fixMidnight)
   {
      if (dateTime->day == MAX_DAY_INMONTH(dateTime->year, dateTime->mon))
      {
         dateTime->day = 1;
         if (dateTime->mon == 12) {
            dateTime->mon = 1;
            dateTime->year += 1;
         }
         else dateTime->mon += 1;
      }
      else dateTime->day += 1;
   }

   if (notcanon) return ASN_E_NOTCANON;
   else return 0;
}

OSINT32 rtParseGeneralizedTime(OSCTXT *pctxt,
   const char* value,
   OSNumDateTime* dateTime)
{
   int status = rtParseGeneralizedTime2(pctxt, value, dateTime, FALSE);
   if ( status == ASN_E_NOTCANON ) return 0;
   else return status;
}

OSINT32 rtParseUTCTime2
(OSCTXT *pctxt, const char* value, OSNumDateTime* dateTime, OSBOOL logCanon)
{
   int year, month, day, hour, minute, second, diffhour, diffmin;
   char tzd;
   const char* p = value;
   OSBOOL canon = (pctxt->flags & ASN1DER) != 0;
   OSBOOL notcanon = FALSE;   /* true if value is non-canonical */
   OSBOOL fixMidnight = FALSE;

   year = month = day = hour = minute = RTERR_IDNOTFOU;
   second = diffhour = diffmin = 0;

   if (parseYYMMDD (p, &year, &month, &day) == 0) {
      int n = 0;
      p += 6;

      if (year < 0)
         return LOG_RTERR(pctxt, RTERR_INVFORMAT);

      if (year < 100) {
         if (year >= 50)
            year += 1900;
         else
            year += 2000;
      }

      if (OS_ISDIGIT(*p) && digitCharsToInt(p, 2, &hour) == 0) {
         p += 2;
         n++;
      }
      else
         return LOG_RTERR(pctxt, RTERR_INVFORMAT);

      if (OS_ISDIGIT(*p) && digitCharsToInt(p, 2, &minute) == 0) {
         p += 2;
         n++;
         if (OS_ISDIGIT(*p) && digitCharsToInt(p, 2, &second) == 0) {
            p += 2;
            n++;
         }
         else if ( canon ) {
            if ( logCanon ) LOG_CANON("seconds required");
            notcanon = TRUE;
         }
      }
      else
         return LOG_RTERR(pctxt, RTERR_INVFORMAT);

      dateTime->tz_flag = FALSE;
      tzd = *p;
      if (tzd == 'Z') { /* utc | GMT */
         dateTime->tz_flag = TRUE;
         if (*++p != 0)
            return LOG_RTERR(pctxt, RTERR_INVFORMAT);
      }
      else {
         /* DER/CER/PER time should be terminated with 'Z'!
             else return LOG_RTERR(pctxt, RTERR_INVFORMAT); */
         if (tzd == '-' || tzd == '+') {
            if ( canon ) {
               if (logCanon) LOG_CANON ("must use Z");
               notcanon = TRUE;
            }

            dateTime->tz_flag = TRUE;
            p++;
            if(!OS_ISDIGIT(*p) || digitCharsToInt(p, 2, &diffhour) != 0)
               return LOG_RTERR(pctxt, RTERR_INVFORMAT);
            p += 2;
            if(!OS_ISDIGIT(*p) || digitCharsToInt(p, 2, &diffmin) != 0)
               return LOG_RTERR(pctxt, RTERR_INVFORMAT);
            if(diffhour < 0 || diffhour > 13)
               return LOG_RTERR(pctxt, RTERR_INVFORMAT);
            if(diffmin < 0 || diffmin > 59)
               return LOG_RTERR(pctxt, RTERR_INVFORMAT);
         }
         else {
            return LOG_RTERR(pctxt, RTERR_INVFORMAT);
         }
      }
   }
   else
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   dateTime->year = (OSINT32)year;
   dateTime->mon = (OSUINT8)month;
   dateTime->day = (OSUINT8)day;
   dateTime->hour = (OSUINT8)hour;
   dateTime->min = (OSUINT8)minute;
   dateTime->sec = (OSREAL)second;
   dateTime->tzo = (diffhour * 60) + diffmin;
   if (tzd == '-') dateTime->tzo *= -1;

   if (dateTime->hour == 24 && dateTime->min == 0 && dateTime->sec == 0)
   {
      /* 240000 is valid for midnight.  Change it to 00, makes sure fields
      are otherwise valid, then add a day. */
      fixMidnight = TRUE;
      dateTime->hour = 0;
      if (canon) {
         if (logCanon) LOG_CANON("Must use 00 for midnight");
         notcanon = TRUE;
      }
   }

   if (!rtxDateTimeIsValid(dateTime))
      return LOG_RTERR(pctxt, RTERR_INVFORMAT);

   if (fixMidnight)
   {
      if (dateTime->day == MAX_DAY_INMONTH(dateTime->year, dateTime->mon))
      {
         dateTime->day = 1;
         if (dateTime->mon == 12) {
            dateTime->mon = 1;
            dateTime->year += 1;
         }
         else dateTime->mon += 1;
      }
      else dateTime->day += 1;
   }

   if (notcanon) return ASN_E_NOTCANON;
   else return 0;
}

OSINT32 rtParseUTCTime
(OSCTXT *pctxt, const char* value, OSNumDateTime* dateTime)
{
   int status = rtParseUTCTime2(pctxt, value, dateTime, FALSE);
   if ( status == ASN_E_NOTCANON ) return 0;
   else return status;
}

