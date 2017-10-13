# $FreeBSD$

.include <src.opts.mk>

SUBDIR=			csu
SUBDIR.${MK_DIALOG}+=	libdialog
SUBDIR.${MK_SSP}+=	libssp
SUBDIR.${MK_TESTS}+=	tests

.if ${MK_GNU_GREP} != "no" || ${MK_GNU_GREP_COMPAT} != "no"
SUBDIR+=		libregex
.endif

.if ${MK_LLVM_LIBUNWIND} == "no"
SUBDIR+=		libgcc
.endif

SUBDIR_PARALLEL=

.include <bsd.subdir.mk>
