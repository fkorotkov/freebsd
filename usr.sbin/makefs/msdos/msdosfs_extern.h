/*	$NetBSD: msdos.h,v 1.3 2015/10/16 16:40:02 christos Exp $	*/

/*-
 * Copyright (c) 2013 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MAKEFS_MSDOSFS_EXTERN_H
#define _MAKEFS_MSDOSFS_EXTERN_H

/* Maximum size of a file on a FAT filesystem */
#define MSDOSFS_FILESIZE_MAX	0xFFFFFFFFLL

#define NOCRED		NULL
#define	B_MODIFY	0
#define DOINGASYNC(vp)	0
#define min(a, b) MIN(a, b)
#define MSDOSFS_DPRINTF(args) do {	\
	if (debug & DEBUG_MSDOSFS)	\
		printf args;		\
} while (0);

#define vput(vp)
#define KASSERT(s, m)
#define ASSERT_VOP_LOCKED(vp, m)
#define ASSERT_VOP_ELOCKED(vp, m)
#define MSDOSFS_LOCK_MP(pmp)
#define MSDOSFS_UNLOCK_MP(pmp)
#define MSDOSFS_ASSERT_MP_LOCKED(pmp)

struct winentry;
struct msdosfsmount;

struct componentname {
	char *cn_nameptr;
	size_t cn_namelen;
};

uint8_t winChksum(uint8_t *name);
int winSlotCnt(const u_char *un, size_t unlen);
int unix2dosfn(const u_char *un, u_char dn[12], size_t unlen, u_int gen,
    struct msdosfsmount *pmp __unused);
int winChkName(const u_char *un, size_t unlen, struct winentry *wep,
    int chksum);
int unix2winfn(const u_char *un, size_t unlen, struct winentry *wep, int cnt,
    int chksum, struct msdosfsmount *pmp __unused);

#endif
