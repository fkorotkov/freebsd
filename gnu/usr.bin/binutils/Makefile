# $FreeBSD$

.include <src.opts.mk>

SUBDIR=	doc\
	libiberty \
	libbfd \
	libopcodes \
	as

# When we use ld.lld as /usr/bin/ld, do not install the non-ifunc-capable
# GNU binutils 2.17.50 ld.
.if ${MK_LLD_IS_LD} == "no"
SUBDIR+=ld
.endif

SUBDIR_DEPEND_as=libbfd libiberty libopcodes
SUBDIR_DEPEND_ld=libbfd libiberty

.if !make(install)
SUBDIR_PARALLEL=
.endif

.include <bsd.subdir.mk>
