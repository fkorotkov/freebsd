# $FreeBSD$

.include <src.opts.mk>

SUBDIR= libdialog libregex

.if ${MK_GCC_CSU} != "no"
SUBDIR+= csu
.endif

.if ${MK_GCC_LIBGCC} != "no"
SUBDIR+= libgcc
.endif

.if ${MK_GCC} != "no"
SUBDIR+= libgcov libgomp
.endif

.if ${MK_SSP} != "no"
SUBDIR+= libssp
.endif

.if ${MK_TESTS} != "no"
SUBDIR+= tests
.endif

.if ${MK_GDB} != "no"
SUBDIR+=	libreadline
.endif

# libsupc++ uses libstdc++ headers, although 'make includes' should
# have taken care of that already.
.if ${MK_GNUCXX} != "no"
SUBDIR+= libstdc++ libsupc++
.endif

.include <bsd.subdir.mk>
