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
/**
 * @file OSRTContext.h
 * C++ run-time context class definition.
 */
#ifndef _OSRTCONTEXT_H_
#define _OSRTCONTEXT_H_

#include "rtxsrc/rtxCommon.h"

class EXTERNRT OSRTContext {
 private:
   OSCTXT mCtxt;
   OSUINT32 mCount;

 public:
   OSRTContext () : mCount(0) { rtInitContext (&mCtxt); }
   ~OSRTContext () {
      rtFreeContext (&mCtxt);
   }
   inline OSCTXT* getCtxtPtr() { return &mCtxt; }
   inline void _ref() {
      mCount++;
   }
   inline void _unref() {
      mCount--;
      if (0 == mCount) { delete this; }
   }
} ;

class OSRTCtxtPtr {
 protected:
   OSRTContext* mPointer;

 public:
   OSRTCtxtPtr (OSRTContext* rf = 0) : mPointer(rf) {
      if (mPointer != 0) mPointer->_ref();
   }

   OSRTCtxtPtr (const OSRTCtxtPtr& o) : mPointer(o.mPointer) {
      if (mPointer != 0) mPointer->_ref();
   }

   virtual ~OSRTCtxtPtr() { if (mPointer != 0) mPointer->_unref(); }

   OSRTCtxtPtr& operator= (const OSRTCtxtPtr& rf) {
      if (mPointer != 0) mPointer->_unref();
      mPointer = rf.mPointer;
      if (mPointer != 0) mPointer->_ref();
      return *this;
   }

   operator OSRTContext* () { return mPointer; }
   operator const OSRTContext* () const { return mPointer; }

   OSRTContext* operator -> () { return mPointer; }
   const OSRTContext* operator -> () const { return mPointer; }

   OSBOOL operator == (const OSRTContext* o) const {
      return (mPointer == o);
   }

   inline OSBOOL isNull() const { return (mPointer == 0); }
   inline OSCTXT* getCtxtPtr() { return isNull() ? 0 : mPointer->getCtxtPtr(); }

};

class OSRTCtxtHolder {
 protected:
   OSRTCtxtPtr mpContext;

 public:
   OSRTCtxtHolder (OSRTContext* pContext = 0) : mpContext(pContext) {
      if (pContext == 0)
         mpContext = new OSRTContext;
   }

   inline OSRTCtxtPtr getContext () {
      return mpContext;
   }

   OSCTXT* getCtxtPtr () {
      return mpContext->getCtxtPtr();
   }
} ;

#endif
