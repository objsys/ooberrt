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
 * @file OSRTMsgBufIF.h
 * C++ run-time message buffer interface class definition.
 */
#ifndef _OSRTMSGBUFIF_H_
#define _OSRTMSGBUFIF_H_

#include "rtxsrc/OSRTContext.h"

/**
 * @defgroup osrtmsgbuf Message Buffer Classes
 * These classes are used to manage message buffers. During encoding, messages
 * are constructed within these buffers. During decoding, the messages to be
 * decoded are held in these buffers. @{
 */
/**
 * Abstract message buffer or stream interface class. This is the base class
 * for both the in-memory message buffer classes and the run-time stream
 * classes.
 */
class EXTERNRT OSRTMessageBufferIF {
 public:
   enum Type {
      BEREncode, BERDecode
   } ;
 protected:
   /**
    * The virtual destructor does nothing. It is overridden by derived versions
    * of this class.
    */
   virtual ~OSRTMessageBufferIF () {}

 public:
   // Virtual methods
   /**
    * Returns a pointer to application-specific information block
    */
   virtual void* getAppInfo () = 0;

   /**
    * The getByteIndex method is used to fetch the current byte offset within
    * the current working buffer. For encoding, this is the next location that
    * will be written to. For decoding, this is the next byte the parser will
    * read.
    */
   virtual size_t getByteIndex () = 0;

   virtual OSRTCtxtPtr getContext () = 0;
   virtual OSCTXT* getCtxtPtr () = 0;

   /**
    * The getMsgCopy method will return a copy of the encoded ASN.1 message
    * managed by the object. The memory for the copy is allocated by new []
    * operator, user is responsible to free it by delete [] operator.
    *
    * @return The pointer to copied encoded ASN.1 message. NULL, if error
    *            occurred.
    */
   virtual OSOCTET* getMsgCopy () = 0;

   /**
    * The getMsgPtr method will return a const pointer to the encoded ASN.1
    * message managed by the object.
    *
    * @return The pointer to the encoded ASN.1 message.
    */
   virtual const OSOCTET* getMsgPtr () = 0;

   /**
    * Initializes message buffer.
    *
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int init () = 0;

   /**
    * This version of the overloaded initBuffer method initializes the message
    * buffer to point at the given null-terminated character string.
    *
    * @param pMsgBuf    Pointer to message buffer.
    * @param msgBufLen  Length of message buffer in bytes.
    *                       string.
    * @return            Completion status of operation:
    *                      - 0 (0) = success,
    *                      - negative return value is error.
    */
   virtual int initBuffer (OSOCTET* pMsgBuf, size_t msgBufLen) = 0;

   /**
    * This method checks the type of the message buffer.
    *
    * @param bufferType    Enumerated identifier specifying a derived class.
    *                         Possible values
    *                         are: BEREncode, BERDecode, PEREncode, PERDecode,
    *                         XEREncode, XERDecode, XMLEncode, XMLDecode,
    *                         Stream.
    * @return              Boolean result of the match operation. True if this
    *                         is the class corresponding to the identifier
    *                         argument.
    */
   virtual OSBOOL isA (int bufferType) = 0;

   /**
    * Sets the application-specific information block.
    */
   virtual void setAppInfo (void* pAppInfo) = 0;

};

/** @} */

#endif
