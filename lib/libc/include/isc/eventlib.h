/*-
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2004, 2005, 2008  Internet Systems Consortium, Inc. ("ISC")
 * Copyright (C) 1995-1999, 2001, 2003  Internet Software Consortium.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/* eventlib.h - exported interfaces for eventlib
 * vix 09sep95 [initial]
 *
 * $Id: eventlib.h,v 1.7 2008/11/14 02:36:51 marka Exp $
 */

#ifndef _EVENTLIB_H
#define _EVENTLIB_H

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <stdio.h>

#include <isc/platform.h>

/* In the absence of branded types... */
typedef struct { void *opaque; } evConnID;
typedef struct { void *opaque; } evFileID;
typedef struct { void *opaque; } evStreamID;
typedef struct { void *opaque; } evTimerID;
typedef struct { void *opaque; } evWaitID;
typedef struct { void *opaque; } evContext;
typedef struct { void *opaque; } evEvent;

#define	evInitID(id) ((id)->opaque = NULL)
#define	evTestID(id) ((id).opaque != NULL)

typedef void(*evConnFunc)(evContext, void *, int, const void *, int,
			  const void *, int);
typedef void(*evFileFunc)(evContext, void *, int, int);
typedef	void(*evStreamFunc)(evContext, void *, int, int);
typedef void(*evTimerFunc)(evContext, void *, struct timespec,
			   struct timespec);
typedef	void(*evWaitFunc)(evContext, void *, const void *);

typedef	struct { unsigned char mask[256/8]; } evByteMask;
#define	EV_BYTEMASK_BYTE(b) ((b) / 8)
#define	EV_BYTEMASK_MASK(b) (1 << ((b) % 8))
#define	EV_BYTEMASK_SET(bm, b) \
	((bm).mask[EV_BYTEMASK_BYTE(b)] |= EV_BYTEMASK_MASK(b))
#define	EV_BYTEMASK_CLR(bm, b) \
	((bm).mask[EV_BYTEMASK_BYTE(b)] &= ~EV_BYTEMASK_MASK(b))
#define	EV_BYTEMASK_TST(bm, b) \
	((bm).mask[EV_BYTEMASK_BYTE(b)] & EV_BYTEMASK_MASK(b))

#define	EV_POLL		1
#define	EV_WAIT		2
#define	EV_NULL		4

#define	EV_READ		1
#define	EV_WRITE	2
#define	EV_EXCEPT	4

#define EV_WASNONBLOCKING 8	/* Internal library use. */

/* eventlib.c */
#define evCreate	__evCreate
#define evSetDebug	__evSetDebug
#define evDestroy	__evDestroy
#define evGetNext	__evGetNext
#define evDispatch	__evDispatch
#define evDrop		__evDrop
#define evMainLoop	__evMainLoop
#define evHighestFD	__evHighestFD
#define evGetOption	__evGetOption
#define evSetOption	__evSetOption

int  evCreate(evContext *);
void evSetDebug(evContext, int, FILE *);
int  evDestroy(evContext);
int  evGetNext(evContext, evEvent *, int);
int  evDispatch(evContext, evEvent);
void evDrop(evContext, evEvent);
int  evMainLoop(evContext);
int  evHighestFD(evContext);
int  evGetOption(evContext *, const char *, int *);
int  evSetOption(evContext *, const char *, int);

/* ev_connects.c */
#define evListen	__evListen
#define evConnect	__evConnect
#define evCancelConn	__evCancelConn
#define evHold		__evHold
#define evUnhold	__evUnhold
#define evTryAccept	__evTryAccept

int evListen(evContext, int, int, evConnFunc, void *, evConnID *);
int evConnect(evContext, int, const void *, int, evConnFunc, void *,
	      evConnID *);
int evCancelConn(evContext, evConnID);
int evHold(evContext, evConnID);
int evUnhold(evContext, evConnID);
int evTryAccept(evContext, evConnID, int *);

/* ev_files.c */
#define evSelectFD	__evSelectFD
#define evDeselectFD	__evDeselectFD

int evSelectFD(evContext, int, int, evFileFunc, void *, evFileID *);
int evDeselectFD(evContext, evFileID);

/* ev_streams.c */
#define evConsIovec	__evConsIovec
#define evWrite		__evWrite
#define evRead		__evRead
#define evTimeRW	__evTimeRW
#define evUntimeRW	__evUntimeRW
#define	evCancelRW	__evCancelRW

struct iovec evConsIovec(void *, size_t);
int evWrite(evContext, int, const struct iovec *, int, evStreamFunc func,
	    void *, evStreamID *);
int evRead(evContext, int, const struct iovec *, int, evStreamFunc func,
	   void *, evStreamID *);
int evTimeRW(evContext, evStreamID, evTimerID timer);
int evUntimeRW(evContext, evStreamID);
int evCancelRW(evContext, evStreamID);

/* ev_timers.c */
#define evConsTime	__evConsTime
#define evAddTime	__evAddTime
#define evSubTime	__evSubTime
#define evCmpTime	__evCmpTime
#define	evTimeSpec	__evTimeSpec
#define	evTimeVal	__evTimeVal

#define evNowTime		__evNowTime
#define evUTCTime		__evUTCTime
#define evLastEventTime		__evLastEventTime
#define evSetTimer		__evSetTimer
#define evClearTimer		__evClearTimer
#define evConfigTimer		__evConfigTimer
#define evResetTimer		__evResetTimer
#define evSetIdleTimer		__evSetIdleTimer
#define evClearIdleTimer	__evClearIdleTimer
#define evResetIdleTimer	__evResetIdleTimer
#define evTouchIdleTimer	__evTouchIdleTimer

struct timespec evConsTime(time_t sec, long nsec);
struct timespec evAddTime(struct timespec, struct timespec);
struct timespec evSubTime(struct timespec, struct timespec);
struct timespec evNowTime(void);
struct timespec evUTCTime(void);
struct timespec evLastEventTime(evContext);
struct timespec evTimeSpec(struct timeval);
struct timeval evTimeVal(struct timespec);
int evCmpTime(struct timespec, struct timespec);
int evSetTimer(evContext, evTimerFunc, void *, struct timespec,
	       struct timespec, evTimerID *);
int evClearTimer(evContext, evTimerID);
int evConfigTimer(evContext, evTimerID, const char *param, int value);
int evResetTimer(evContext, evTimerID, evTimerFunc, void *, struct timespec,
		 struct timespec);
int evSetIdleTimer(evContext, evTimerFunc, void *, struct timespec,
		   evTimerID *);
int evClearIdleTimer(evContext, evTimerID);
int evResetIdleTimer(evContext, evTimerID, evTimerFunc, void *,
		     struct timespec);
int evTouchIdleTimer(evContext, evTimerID);

/* ev_waits.c */
#define evWaitFor	__evWaitFor
#define evDo		__evDo
#define evUnwait	__evUnwait
#define evDefer		__evDefer

int evWaitFor(evContext, const void *, evWaitFunc, void *, evWaitID *);
int evDo(evContext, const void *);
int evUnwait(evContext, evWaitID);
int evDefer(evContext, evWaitFunc, void *);

#endif /*_EVENTLIB_H*/

/*! \file */
