# $FreeBSD$

.include <src.opts.mk>

SUBDIR=	doc\
	libiberty \
	libbfd \
	libopcodes \
	as \
	ld

SUBDIR_DEPEND_as=libbfd libiberty libopcodes
SUBDIR_DEPEND_ld=libbfd libiberty

.if !make(install)
SUBDIR_PARALLEL=
.endif

.include <bsd.subdir.mk>
