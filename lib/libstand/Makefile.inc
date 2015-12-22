# $FreeBSD$

# string functions from libc
.PATH: ${LIBC_SRC}/string
SRCS+=	bcmp.c bcopy.c bzero.c ffs.c memccpy.c memchr.c memcmp.c memcpy.c \
	memmove.c memset.c qdivrem.c strcat.c strchr.c strchrnul.c strcmp.c \
	strcpy.c strcspn.c strlcat.c strlcpy.c strlen.c strncat.c strncmp.c \
	strncpy.c strpbrk.c strrchr.c strsep.c strspn.c strstr.c strtok.c swab.c
