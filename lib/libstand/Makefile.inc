# $FreeBSD$

# standalone components and stuff we have modified locally
SRCS+=	gzguts.h zutil.h __main.c assert.c bcd.c bswap.c environment.c getopt.c gets.c \
	globals.c pager.c printf.c strdup.c strerror.c strtol.c strtoul.c random.c \
	sbrk.c twiddle.c zalloc.c zalloc_malloc.c

# private (pruned) versions of libc string functions
SRCS+=	strcasecmp.c

.PATH: ${LIBC_SRC}/net
SRCS+= ntoh.c

# string functions from libc
.PATH: ${LIBC_SRC}/string
SRCS+=	bcmp.c bcopy.c bzero.c ffs.c memccpy.c memchr.c memcmp.c memcpy.c \
	memmove.c memset.c qdivrem.c strcat.c strchr.c strchrnul.c strcmp.c \
	strcpy.c strcspn.c strlcat.c strlcpy.c strlen.c strncat.c strncmp.c \
	strncpy.c strpbrk.c strrchr.c strsep.c strspn.c strstr.c strtok.c swab.c

# Compiler support functions
# ...
# _setjmp/_longjmp

# decompression functionality from libbz2
# NOTE: to actually test this functionality after libbz2 upgrade compile
# loader(8) with LOADER_BZIP2_SUPPORT defined
.PATH: ${LIBSTAND_SRC}/../../contrib/bzip2
CFLAGS+= -DBZ_NO_STDIO -DBZ_NO_COMPRESS
SRCS+=	libstand_bzlib_private.h

.for file in bzlib.c crctable.c decompress.c huffman.c randtable.c
SRCS+=	_${file}
CLEANFILES+=	_${file}

_${file}: ${file}
	sed "s|bzlib_private\.h|libstand_bzlib_private.h|" \
	    ${.ALLSRC} > ${.TARGET}
.endfor

CLEANFILES+= libstand_bzlib_private.h
libstand_bzlib_private.h: bzlib_private.h
	sed -e 's|<stdlib.h>|"stand.h"|' \
		${.ALLSRC} > ${.TARGET}

# decompression functionality from libz
.PATH: ${LIBSTAND_SRC}/../libz
CFLAGS+=-DHAVE_MEMCPY -I${LIBSTAND_SRC}/../libz
SRCS+=	adler32.c crc32.c libstand_zutil.h libstand_gzguts.h

.for file in infback.c inffast.c inflate.c inftrees.c zutil.c
SRCS+=	_${file}
CLEANFILES+=	_${file}

_${file}: ${file}
	sed -e "s|zutil\.h|libstand_zutil.h|" \
	    -e "s|gzguts\.h|libstand_gzguts.h|" \
	    ${.ALLSRC} > ${.TARGET}
.endfor

# depend on stand.h being able to be included multiple times
.for file in zutil.h gzguts.h
CLEANFILES+= libstand_${file}
libstand_${file}: ${file}
	sed -e 's|<fcntl.h>|"stand.h"|' \
	    -e 's|<stddef.h>|"stand.h"|' \
	    -e 's|<string.h>|"stand.h"|' \
	    -e 's|<stdio.h>|"stand.h"|' \
	    -e 's|<stdlib.h>|"stand.h"|' \
	    ${.ALLSRC} > ${.TARGET}
.endfor

# io routines
SRCS+=	closeall.c dev.c ioctl.c nullfs.c stat.c \
