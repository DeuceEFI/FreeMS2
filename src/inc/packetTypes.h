/* FreeMS2 - the open source engine management system
 *
 * Copyright 2008, 2009, 2010 Fred Cooke
 *
 * This file is part of the FreeMS2 project.
 *
 * FreeMS2 software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeMS2 software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeMS2 software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at admin(at)diyefi(dot)org or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeMS2 to run your engine!
 */


/** @file packetTypes.h
 * @ingroup allHeaders
 */


/* Header file multiple inclusion protection courtesy eclipse Header Template	*/
/* and http://gcc.gnu.org/onlinedocs/gcc-3.1.1/cpp/ C pre processor manual		*/
#ifndef FILE_PACKET_TYPES_H_SEEN
#define FILE_PACKET_TYPES_H_SEEN


/*&&&&&&&&&&&&&&&&&&&&  Payload Type ID Name Value Pairs  &&&&&&&&&&&&&&&&&&&&*/

/* Please note, requests use even ID numbers and their matching responses use */
/* the odd number immediately above the original request ID. There are a few  */
/* examples where either a request is not required or a response not sent. In */
/* such cases the odd ID may be used for an packet asyncronously sent from    */
/* the EMS to the PC. These will be well commented when present.			  */


/*&&&&&&&&&&&&&&&&&&&&&&&&  Protocol Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Firmware Independent functions */
#define requestInterfaceVersion     0x0000
#define requestFirmwareVersion      0x0002
#define requestMaxPacketSize        0x0004
#define requestEchoPacketReturn     0x0006
#define requestSoftSystemReset      0x0008 /* System comes up fresh, so no response */
//efine replytoSoftSystemReset      0x0009 /* This is reserved */
#define requestHardSystemReset      0x000A /* System comes up fresh, so no response */
//efine replytoHardSystemReset      0x000B /* This is reserved */
#define requestReInitOfSystem      0x000C /* EXPERIMENTAL, currently responds like normal */
//efine replytoReInitOfSystem      0x000D /* This is reserved */


//efine requestAsyncErrorCode       0x000C /* This is reserved */
#define asyncErrorCodePacket        0x000D /* NOTE : Unrequested error event code packet */
//efine requestAsyncDebugInfo       0x000E /* This is reserved */
#define asyncDebugInfoPacket        0x000F /* NOTE : Unrequested debug information packet */


/*&&&&&&&&&&&&&&&&&&&&&&&&  Firmware Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Data block manipulation */
#define updateBlockInRAM            0x0100
#define updateBlockInFlash          0x0102
#define retrieveBlockFromRAM        0x0104
#define retrieveBlockFromFlash      0x0106
#define burnBlockFromRamToFlash     0x0108

/* Main table manipulation */
#define adjustMainTableCell         0x012C
#define adjustMainTableRPMAxis      0x012E
#define	adjustMainTableLoadAxis     0x0130

/* 2d table manipulation */
#define adjust2dTableAxis           0x0132
#define adjust2dTableCell           0x0134

/* Datalog request packets */
#define requestBasicDatalog         0x0190
#define responseBasicDatalog        0x0191 /* Defined because it can be used both synchronously and asynchronously */
#define requestConfigurableDatalog  0x0192
#define responseConfigurableDatalog 0x0193 /* Defined because it can be used both synchronously and asynchronously */
#define setAsyncDatalogType         0x0194

/* Special function */
#define forwardPacketOverCAN        0x01F4
#define forwardPacketOverOtherUART  0x01F6

/* Generic memory grabber for debugging */
#define retrieveArbitraryMemory     0x0258

// Data interrogation gold
#define retrieveListOfLocationIDs   0xDA5E // Idea is Dave's, impl is Fred's
#define retrieveLocationIDDetails   0xF8E0 // Idea is Fred's, impl is Fred's


#else
	/* let us know if we are being untidy with headers */
	#warning "Header file PACKET_TYPES_H seen before, sort it out!"
/* end of the wrapper ifdef from the very top */
#endif
