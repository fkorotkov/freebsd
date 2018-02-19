#include <sys/cdefs.h>
#include <sys/errno.h>

/*
 * Linux syscalls return negative errno's, we do positive and map them
 * Reference:
 *   FreeBSD: src/sys/sys/errno.h
 *   Linux:   linux-2.6.17.8/include/asm-generic/errno-base.h
 *            linux-2.6.17.8/include/asm-generic/errno.h
 */
const int bsd_to_linux_errno_generic[ELAST + 1] = {
	-0,
	-1,
	-2,
	-3,
	-4,
	-5,
	-6,
	-7,
	-8,
	-9,

	-10,
	-35,	/* EDEADLK */
	-12,
	-13,
	-14,
	-15,
	-16,
	-17,
	-18,
	-19,

	-20,
	-21,
	-22,
	-23,
	-24,
	-25,
	-26,
	-27,
	-28,
	-29,

	-30,
	-31,
	-32,
	-33,
	-34,
	-11,	/* EAGAIN */
	-115,
	-114,
	-88,
	-89,

	-90,
	-91,
	-92,
	-93,
	-94,
	-95,
	-96,
	-97,
	-98,
	-99,

	-100,
	-101,
	-102,
	-103,
	-104,
	-105,
	-106,
	-107,
	-108,
	-109,

	-110,
	-111,
	-40,
	-36,
	-112,
	-113,
	-39,
	-11,
	-87,
	-122,

	-116,
	-66,
	-6,	/* EBADRPC -> ENXIO */
	-6,	/* ERPCMISMATCH -> ENXIO */
	-6,	/* EPROGUNAVAIL -> ENXIO */
	-6,	/* EPROGMISMATCH -> ENXIO */
	-6,	/* EPROCUNAVAIL -> ENXIO */
	-37,
	-38,
	-9,

	-6,	/* EAUTH -> ENXIO */
	-6,	/* ENEEDAUTH -> ENXIO */
	-43,
	-42,
	-75,
	-125,
	-84,
	-61,
	-16,	/* EDOOFUS -> EBUSY */
	-74,

	-72,
	-67,
	-71,
	-1,	/* ENOTCAPABLE -> EPERM */
	-1,	/* ECAPMODE -> EPERM */
	-131,	/* ENOTRECOVERABLE */
	-130,	/* EOWNERDEAD */
};

_Static_assert(ELAST == 96,
    "missing errno entries in bsd_to_linux_errno_generic");
