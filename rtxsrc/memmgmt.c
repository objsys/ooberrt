/**
 * Copyright (c) 1997-2016 by Objective Systems, Inc.
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

#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxCommon.h"

typedef struct MemBlk {
   struct MemBlk* pnext;
   void*          pmem;
} OSMemBlk;

typedef struct MemHeap {
   OSMemBlk*    phead;
   OSMemBlk*    ptail;
   OSUINT32     count;
} OSMemHeap;

void* rtxMemAlloc (OSCTXT* pctxt, size_t nbytes)
{
   OSMemHeap* pMemHeap;
   OSMemBlk*  pMemBlk;
   void*      pmem;

   if (nbytes == 0) return NULL;

   if (pctxt->pMemHeap == 0) {
      pMemHeap = (OSMemHeap*) malloc (sizeof (OSMemHeap));
      if (pMemHeap == NULL) return NULL;

      memset (pMemHeap, 0, sizeof (OSMemHeap));
      pctxt->pMemHeap = (void*) pMemHeap;
   }
   else pMemHeap = (OSMemHeap*) pctxt->pMemHeap;

   /* Allocate memory using configured malloc function */

   pmem = malloc (nbytes);
   if (pmem == NULL) return NULL;

   /* Add memory block to end of mem block list */

   pMemBlk = (OSMemBlk*) malloc (sizeof(OSMemBlk));
   if (pMemBlk == NULL) {
      free (pmem);
      return NULL;
   }
   pMemBlk->pnext = NULL;
   pMemBlk->pmem = pmem;

   if (pMemHeap->ptail != NULL) {
      pMemHeap->ptail->pnext = pMemBlk;
      pMemHeap->ptail = pMemBlk;
   }
   else { /* first record */
      pMemHeap->phead = pMemHeap->ptail = pMemBlk;
   }
   pMemHeap->count++;

   return pmem;
}

void* rtxMemAllocZ (OSCTXT* pctxt, size_t nbytes)
{
   void* ptr = rtxMemAlloc (pctxt, nbytes);
   if (0 != ptr) memset (ptr, 0, nbytes);
   return ptr;
}

void rtxMemFreePtr2 (OSCTXT* pctxt, void* pmem)
{
   OSMemHeap* pMemHeap;
   OSMemBlk*  pMemBlk;
   OSMemBlk*  pPrevMemBlk;

   if (pmem == 0 || pctxt == 0 || pctxt->pMemHeap == 0) return;

   pMemHeap = (OSMemHeap*) pctxt->pMemHeap;
   pMemBlk = pMemHeap->phead;
   pPrevMemBlk = 0;

   /* Lookup pointer to be freed in the mem block list */

   while (0 != pMemBlk) {
      if (pMemBlk->pmem == pmem) break;
      else {
         pPrevMemBlk = pMemBlk;
         pMemBlk = pMemBlk->pnext;
      }
   }

   /* If found, free memory and remove block from list */

   if (0 != pMemBlk) {
      free (pmem);
      if (0 != pPrevMemBlk) {
         pPrevMemBlk->pnext = pMemBlk->pnext;
         if (pMemBlk == pMemHeap->ptail) {
            pMemHeap->ptail = pPrevMemBlk;
         }
      }
      else { /* head record */
         /* Check for single record case, head == tail */
         if (pMemHeap->count == 1) {
            pMemHeap->ptail = pMemHeap->phead = 0;
         }
         else { /* only need to adjust head */
            pMemHeap->phead = pMemBlk->pnext;
         }
      }
      free (pMemBlk);
      pMemHeap->count--;
   }
}

void rtxMemFree (OSCTXT* pctxt)
{
   OSMemHeap* pMemHeap;
   OSMemBlk*  pMemBlk;
   OSMemBlk*  pPrevMemBlk;

   if (pctxt == 0 || pctxt->pMemHeap == 0) return;

   pMemHeap = (OSMemHeap*) pctxt->pMemHeap;
   pMemBlk = pMemHeap->phead;

   /* Free all blocks in the list */

   while (0 != pMemBlk) {
      pPrevMemBlk = pMemBlk;
      pMemBlk = pMemBlk->pnext;
      free (pPrevMemBlk->pmem);
      free (pPrevMemBlk);
   }
}

void* rtxMemRealloc (OSCTXT* pctxt, void* pmem, size_t nbytes)
{
   OSMemHeap* pMemHeap;
   OSMemBlk*  pMemBlk;

   if (pmem == 0 || pctxt == 0 || pctxt->pMemHeap == 0 || nbytes == 0)
      return NULL;

   pMemHeap = (OSMemHeap*) pctxt->pMemHeap;
   pMemBlk = pMemHeap->phead;

   /* Lookup pointer to be reallocated in the mem block list */

   while (0 != pMemBlk) {
      if (pMemBlk->pmem == pmem) break;
      else pMemBlk = pMemBlk->pnext;
   }

   /* If found, call memory reallocation function */

   if (0 != pMemBlk) {
      pMemBlk->pmem = realloc (pmem, nbytes);
      return pMemBlk->pmem;
   }
   else return NULL;
}

void rtxMemReset (OSCTXT* pctxt)
{
   rtxMemFree (pctxt);
}

OSBOOL rtxMemHeapIsEmpty (OSCTXT* pctxt)
{
   OSMemHeap* pMemHeap = (OSMemHeap*) pctxt->pMemHeap;
   return (OSBOOL)(0 == pMemHeap->count);
}

void rtxMemFreeOpenSeqExt (OSCTXT* pctxt, OSRTDList* pElemList)
{
   if (!rtxMemHeapIsEmpty (pctxt)) {
      OSRTDListNode* pNode, *pNextNode;
      OSOpenType* pOpenType;

      for (pNode = pElemList->head; pNode != 0; pNode = pNextNode) {
         pNextNode = pNode->next;

         pOpenType = (OSOpenType*) pNode->data;

         if (0 != pOpenType) {
            if (0 != pOpenType->data) {
               rtxMemFreePtr (pctxt, (void*)pOpenType->data);
            }

            rtxMemFreePtr (pctxt, pOpenType);
         }

         rtxMemFreePtr (pctxt, pNode);
      }
   }

   pElemList->count = 0;
   pElemList->head = pElemList->tail = 0;
}
