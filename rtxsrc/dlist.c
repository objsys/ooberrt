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

#include "rtxsrc/rtxCommon.h"

void rtxDListInit (OSRTDList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (OSRTDListNode*) 0;
      pList->tail = (OSRTDListNode*) 0;
   }
}

OSRTDListNode* rtxDListAppend
(OSCTXT* pctxt, OSRTDList* pList, const void* pData)
{
   OSRTDListNode* pListNode = (OSRTDListNode*)
      rtxMemAlloc (pctxt, sizeof(OSRTDListNode));

   if (0 != pListNode) {
      pListNode->data = (void*)pData;
      pListNode->next = (OSRTDListNode*) 0;
      if (0 != pList->tail) {
         pList->tail->next = pListNode;
         pListNode->prev = pList->tail;
      }
      if (0 == pList->head) {
         pList->head = pListNode;
         pListNode->prev = (OSRTDListNode*) 0;
      }
      pList->tail = pListNode;
      pList->count++;
   }

   return pListNode;
}

OSRTDListNode* rtxDListAppendNode (OSRTDList* pList, OSRTDListNode* pListNode)
{
   if (0 != pListNode) {
      pListNode->next = (OSRTDListNode*) 0;
      if (0 != pList->tail) {
         pList->tail->next = pListNode;
         pListNode->prev = pList->tail;
      }
      if (0 == pList->head) {
         pList->head = pListNode;
         pListNode->prev = (OSRTDListNode*) 0;
      }
      pList->tail = pListNode;
      pList->count++;
   }

   return pListNode;
}

/* Free all nodes, but not the data */
void rtxDListFreeNodes (OSCTXT* pctxt, OSRTDList* pList)
{
   OSRTDListNode* pNode, *pNextNode;

   for (pNode = pList->head; pNode != 0; pNode = pNextNode) {
      pNextNode = pNode->next;
      rtxMemFreePtr (pctxt, pNode);
   }
   pList->count = 0;
   pList->head = pList->tail = 0;
}

/* Free all nodes and their data */
void rtxDListFreeAll (OSCTXT* pctxt, OSRTDList* pList)
{
   OSRTDListNode* pNode, *pNextNode;

   for (pNode = pList->head; pNode != 0; pNode = pNextNode) {
      pNextNode = pNode->next;

      rtxMemFreePtr (pctxt, pNode->data);
      rtxMemFreePtr (pctxt, pNode);
   }
   pList->count = 0;
   pList->head = pList->tail = 0;
}

/* Remove node from list. Node is not freeing */
void rtxDListRemove (OSRTDList* pList, OSRTDListNode* node)
{
   if(node->next != 0) {
      node->next->prev = node->prev;
   }
   else { /* tail */
      pList->tail = node->prev;
   }
   if(node->prev != 0) {
      node->prev->next = node->next;
   }
   else { /* head */
      pList->head = node->next;
   }
   pList->count--;
}

OSRTDListNode* rtxDListFindByIndex (OSRTDList* pList, size_t idx)
{
   OSRTDListNode* curNode;
   size_t i;

   if (idx >= pList->count) return 0;
   for (i = 0, curNode = pList->head; i < idx && curNode != 0; i++) {
      curNode = curNode->next;
   }
   return curNode;
}

void rtxSListInitEx (OSCTXT* pctxt, OSRTSList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (OSRTSListNode*) 0;
      pList->tail = (OSRTSListNode*) 0;
      pList->pctxt = pctxt;
   }
}

OSRTSListNode* rtxSListAppend (OSRTSList* pList, void* pData)
{
   OSRTSListNode* pListNode =
      rtxMemAllocType (pList->pctxt, OSRTSListNode);

   if (pListNode) {
      pListNode->data = pData;
      pListNode->next = (OSRTSListNode*) 0;
      if (pList->tail) {
         pList->tail->next = pListNode;
      }
      if (!pList->head) {
         pList->head = pListNode;
      }
      pList->tail = pListNode;
      pList->count++;
   }

   return pListNode;
}

