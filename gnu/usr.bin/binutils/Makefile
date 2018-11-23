# $FreeBSD$

.include <src.opts.mk>

SUBDIR=	doc\
	libiberty \
	libbfd \
	libopcodes \
	as \
	ld

# We install llvm-objdump as objdump on those architectures using clang.
.if ${MK_CLANG} != "no"
SUBDIR+=libbinutils \
	objdump
.endif

SUBDIR_DEPEND_libbinutils=libbfd		# for bfdver.h
SUBDIR_DEPEND_as=libbfd libiberty libopcodes
SUBDIR_DEPEND_ld=libbfd libiberty
SUBDIR_DEPEND_objdump=libbfd libiberty libbinutils libopcodes

.if !make(install)
SUBDIR_PARALLEL=
.endif

.include <bsd.subdir.mk>
