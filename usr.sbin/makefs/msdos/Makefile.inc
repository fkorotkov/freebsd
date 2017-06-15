#	$NetBSD: Makefile.inc,v 1.7 2016/02/06 10:40:58 mlelstv Exp $
#

MSDOS=		${SRCTOP}/sys/fs/msdosfs
MSDOS_NEWFS=	${SRCTOP}/sbin/newfs_msdos

.PATH:	${SRCDIR}/msdos ${MSDOS} ${MSDOS_NEWFS}

CFLAGS+= -DMAKEFS -I${MSDOS} -I${MSDOS_NEWFS}

SRCS+= msdosfs_denode.c msdosfs_vnops.c msdosfs_vfsops.c msdosfs_conv.c
SRCS+= msdosfs_fat.c msdosfs_lookup.c
SRCS+= mkfs_msdos.c
