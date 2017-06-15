/*	$NetBSD: msdos.c,v 1.20 2017/04/14 15:40:35 christos Exp $	*/

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
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/mount.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <util.h>

#include <fs/msdosfs/bpb.h>
#include <fs/msdosfs/denode.h>
#include <fs/msdosfs/msdosfsmount.h>

#include "ffs/buf.h"
#include "msdos/msdosfs_extern.h"
#include "makefs.h"
#include "msdos.h"
#include "mkfs_msdos.h"

static int msdos_populate_dir(const char *, struct denode *, fsnode *,
    fsnode *, fsinfo_t *);

void
msdos_prep_opts(fsinfo_t *fsopts)
{
	struct msdos_options *msdos_spec = ecalloc(1, sizeof(*msdos_spec));
	const option_t msdos_opts[] = {
#define AOPT(_opt, _type, _name, _min, _desc) { 			\
	.letter = _opt,							\
	.name = # _name,						\
	.type = _min == -1 ? OPT_STRPTR :				\
	    (_min == -2 ? OPT_BOOL :					\
	    (sizeof(_type) == 1 ? OPT_INT8 :				\
	    (sizeof(_type) == 2 ? OPT_INT16 :				\
	    (sizeof(_type) == 4 ? OPT_INT32 : OPT_INT64)))),		\
	.value = &msdos_spec->_name,					\
	.minimum = _min,						\
	.maximum = sizeof(_type) == 1 ? UINT8_MAX :			\
	    (sizeof(_type) == 2 ? UINT16_MAX :				\
	    (sizeof(_type) == 4 ? UINT32_MAX : INT64_MAX)),		\
	.desc = _desc,							\
},
ALLOPTS
#undef AOPT
		{ .name = NULL }
	};

	fsopts->fs_specific = msdos_spec;
	fsopts->fs_options = copy_opts(msdos_opts);
}

void
msdos_cleanup_opts(fsinfo_t *fsopts)
{
	free(fsopts->fs_specific);
	free(fsopts->fs_options);
}

int
msdos_parse_opts(const char *option, fsinfo_t *fsopts)
{
	struct msdos_options *msdos_spec = fsopts->fs_specific;
	option_t *msdos_opts = fsopts->fs_options;
	int rv;

	assert(option != NULL);
	assert(fsopts != NULL);
	assert(msdos_spec != NULL);

	if (debug & DEBUG_FS_PARSE_OPTS)
		printf("msdos_parse_opts: got '%s'\n", option);

	rv = set_option(msdos_opts, option, NULL, 0);
	if (rv == -1)
		return rv;

	if (strcmp(msdos_opts[rv].name, "volume_id") == 0)
		msdos_spec->volume_id_set = 1;
	else if (strcmp(msdos_opts[rv].name, "media_descriptor") == 0)
		msdos_spec->media_descriptor_set = 1;
	else if (strcmp(msdos_opts[rv].name, "hidden_sectors") == 0)
		msdos_spec->hidden_sectors_set = 1;

	if (stampst.st_ino) {
		msdos_spec->timestamp_set = 1;
		msdos_spec->timestamp = stampst.st_mtime;
	}

	return 1;
}


void
msdos_makefs(const char *image, const char *dir, fsnode *root, fsinfo_t *fsopts)
{
	struct msdos_options *msdos_spec = fsopts->fs_specific;
	struct vnode vp, rootvp;
	struct timeval start;
	struct msdosfsmount *pmp;
	uint32_t flags;

	assert(image != NULL);
	assert(dir != NULL);
	assert(root != NULL);
	assert(fsopts != NULL);

	fsopts->size = fsopts->maxsize;
	msdos_spec->create_size = MAX(msdos_spec->create_size,
	    fsopts->offset + fsopts->size);
	if (fsopts->offset > 0)
		msdos_spec->offset = fsopts->offset;
	if (msdos_spec->bytes_per_sector == 0) {
		if (fsopts->sectorsize == -1)
			fsopts->sectorsize = 512;
		msdos_spec->bytes_per_sector = fsopts->sectorsize;
	} else if (fsopts->sectorsize == -1) {
		fsopts->sectorsize = msdos_spec->bytes_per_sector;
	} else if (fsopts->sectorsize != msdos_spec->bytes_per_sector) {
		err(1, "inconsistent sectorsize -S %u"
		    "!= -o bytes_per_sector %u",
		    fsopts->sectorsize, msdos_spec->bytes_per_sector);
	}

	/* create image */
	printf("Creating '%s'\n", image);
	TIMER_START(start);
	if (mkfs_msdos(image, NULL, msdos_spec) == -1)
		return;
	TIMER_RESULTS(start, "mkfs_msdos");

	fsopts->fd = open(image, O_RDWR);
	vp.fs = fsopts;

	flags = 0;
	if ((pmp = msdosfs_mount(&vp)) == NULL)
		err(1, "msdosfs_mount");

	if (msdosfs_root(pmp, &rootvp) != 0)
		err(1, "msdosfs_root");

	if (debug & DEBUG_FS_MAKEFS)
		printf("msdos_makefs: image %s directory %s root %p\n",
		    image, dir, root);

	/* populate image */
	printf("Populating '%s'\n", image);
	TIMER_START(start);
	if (msdos_populate_dir(dir, VTODE(&rootvp), root,
	    root, fsopts) == -1)
		errx(1, "Image file '%s' not created.", image);
	TIMER_RESULTS(start, "msdos_populate_dir");

	if (debug & DEBUG_FS_MAKEFS)
		putchar('\n');

	/* ensure no outstanding buffers remain */
	if (debug & DEBUG_FS_MAKEFS)
		bcleanup();

	printf("Image '%s' complete\n", image);
}

static int
msdos_populate_dir(const char *path, struct denode *dir, fsnode *root,
    fsnode *parent, fsinfo_t *fsopts)
{
	fsnode *cur;
	char pbuf[MAXPATHLEN];

	assert(dir != NULL);
	assert(root != NULL);
	assert(fsopts != NULL);

	for (cur = root->next; cur != NULL; cur = cur->next) {
		if ((size_t)snprintf(pbuf, sizeof(pbuf), "%s/%s", path,
		    cur->name) >= sizeof(pbuf)) {
			warnx("path %s too long", pbuf);
			return -1;
		}

		if ((cur->inode->flags & FI_ALLOCATED) == 0) {
			cur->inode->flags |= FI_ALLOCATED;
			if (cur != root) {
				fsopts->curinode++;
				cur->inode->ino = fsopts->curinode;
				cur->parent = parent;
			}
		}

		if (cur->inode->flags & FI_WRITTEN) {
			continue;	// hard link
		}
		cur->inode->flags |= FI_WRITTEN;

		if (cur->child) {
			struct denode *de;
			if ((de = msdosfs_mkdire(pbuf, dir, cur)) == NULL) {
				warn("msdosfs_mkdire %s", pbuf);
				return -1;
			}
			if (msdos_populate_dir(pbuf, de, cur->child, cur,
			    fsopts) == -1) {
				warn("msdos_populate_dir %s", pbuf);
				return -1;
			}
			continue;
		} else if (!S_ISREG(cur->type)) {
			warnx("skipping non-regular file %s/%s", cur->path,
			    cur->name);
			continue;
		}
		if (msdosfs_mkfile(pbuf, dir, cur) == NULL) {
			warn("msdosfs_mkfile %s", pbuf);
			return -1;
		}
	}
	return 0;
}
