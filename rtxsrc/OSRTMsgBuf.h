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
 * @file OSRTMsgBuf.h
 * C++ run-time message buffer class definition.
 */
#ifndef _OSRTMSGBUF_H_
#define _OSRTMSGBUF_H_

#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/OSRTMsgBufIF.h"

/**
 * @addtogroup osrtmsgbuf
 * @{
 */
/**
 * Abstract message buffer base class. This class is used to manage an
 * encode or decode message buffer. For encoding, this is the buffer
 * into which the message is being built. For decoding, it describes a
 * message that was read into memory to be decoded. Further classes are
 * derived from this to handle encoding and decoding of messages for
 * different encoding rules types.
 */
class EXTERNRT OSRTMessageBuffer : public OSRTMessageBufferIF
{
 protected:
   OSRTCtxtHolder mCtxtHolder;
   int mStatus;

   /**
    * The mBufferType member variable holds information on the derived
    * message buffer class type (for example, XMLEncode).
    */
   Type mBufferType;

   /**
    * The protected constructor creates a new context and sets the buffer class
    * type.
    *
    * @param bufferType   Type of message buffer that is being created (for
    *                       example, XMLEncode).
    * @param pContext     Pointer to a context to use. If NULL, new context
    *                       will be allocated.
    */
   OSRTMessageBuffer (Type bufferType, OSRTContext* pContext = 0) :
   mCtxtHolder(pContext), mStatus(0), mBufferType(bufferType) {}

 public:
   /**
    * The virtual destructor does nothing. It is overridden by derived versions
    * of this class.
    */
   virtual ~OSRTMessageBuffer () {}

   /**
    * Returns a pointer to application-specific information block
    */
   virtual void* getAppInfo () { return 0; }

   /**
    * The getByteIndex method is used to fetch the current byte offset within
    * the current working buffer. For encoding, this is the next location that
    * will be written to. For decoding, this is the next byte the parser will
    * read.
    */
   virtual size_t getByteIndex () {
      return getCtxtPtr()->buffer.byteIndex;
   }

   virtual OSRTCtxtPtr getContext () { return mCtxtHolder.getContext(); }

   /**
    * The getCtxtPtr method returns the underlying C runtime context. This
    * context can be used in calls to C runtime functions.
    */
   virtual OSCTXT* getCtxtPtr () {
      return mCtxtHolder.getCtxtPtr();
   }

   /**
    * The getMsgCopy method will return a copy of the encoded message
    * managed by the object.
    */
   virtual OSOCTET* getMsgCopy () {
      return 0;
   }

   /**
    * The getMsgPtr method will return a const pointer to the encoded
    * message managed by the object.
    */
   virtual const OSOCTET* getMsgPtr () {
      return getCtxtPtr()->buffer.data;
   }

   /**
    * Initializes message buffer.
    *
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int init () { return 0; }

   /**
    * This version of the overloaded initBuffer method initializes the message
    * buffer to point at the given buffer address and size.
    *
    * @param pMsgBuf    Pointer to message buffer.
    * @param msgBufLen  Length of message buffer in bytes.
    *
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int initBuffer (OSOCTET* pMsgBuf, size_t msgBufLen) {
      return rtxInitContextBuffer (getCtxtPtr(), pMsgBuf, msgBufLen);
   }

   /**
    * The printErrorInfo method prints information on errors contained within
    * the context.
    */
   virtual void printErrorInfo () {
      rtxErrPrint (getCtxtPtr());
   }

   /**
    * The resetErrorInfo method resets information on errors contained within
    * the context.
    */
   virtual void resetErrorInfo () {
      rtxErrReset (getCtxtPtr());
   }

   /**
    * Sets the application-specific information block.
    */
   virtual void setAppInfo (void*) {}

   /**
    * Get/set status
    */
   inline int getStatus () const { return mStatus; }
   inline void setStatus (int value) { mStatus = value; }

} ;

/** @} */

#endif
