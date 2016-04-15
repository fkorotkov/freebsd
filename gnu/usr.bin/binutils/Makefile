# $FreeBSD$

.include <src.opts.mk>

SUBDIR=	doc\
	libiberty \
	libbfd \
	libopcodes \
	libbinutils \
	as \
	ld \
	${_objcopy}

.if ${MK_ELFCOPY_AS_OBJCOPY} == "no"
_objcopy=	objcopy
.endif

SUBDIR_DEPEND_libbinutils=libbfd		# for bfdver.h
SUBDIR_DEPEND_as=libbfd libiberty libopcodes
SUBDIR_DEPEND_ld=libbfd libiberty
SUBDIR_DEPEND_objcopy=libbfd libiberty libbinutils

.if !make(install)
SUBDIR_PARALLEL=
.endif

.include <bsd.subdir.mk>
