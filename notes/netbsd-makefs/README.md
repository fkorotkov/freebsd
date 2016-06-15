# Previous Updates #

Last full FreeBSD update of makefs code was r214921:

commit 69d9f8b92c590010252135df10fc18177928b63d
Author: cognet <cognet@FreeBSD.org>
Date:   Sun Nov 7 16:05:04 2010 +0000

    Sync with the latest version from NetBSD. It notably addds ISO9660 support.

    Submitted by:       bapt

The last NetBSD commit prior to this was:

commit cfb2fb0dfbae05a530544e6e459287f1af7ea15e
Author: christos <christos>
Date:   Wed Oct 27 18:51:34 2010 +0000

    Handle > 2GB images. Thanks to Greg Troxel for testing!

cd9660 was sync'd in r224762, Aug 10 2011

# Commands #

Obtain patches / history via:
~~~~
git format-patch --stdout cfb2fb0dfbae05a530544e6e459287f1af7ea15e.. usr.sbin/makefs
git log --reverse --stat cfb2fb0dfbae05a530544e6e459287f1af7ea15e.. usr.sbin/makefs
~~~~

# TODO #

## parsedate and -T option ##

r301879 added -T support to makefs to set an explicit timestamp.
makefs.c had minor changes to adapt to FreeBSD, and needs the following:

1. Import parsedate (into libnetbsd/libutil/?)
2. Restore parsedate to main()
3. Restore clock_relatime to main()
4. Update makefs.8 to include the parsedate reference
5. Send bug report to NetBSD (from cem@ in review, st_ino == 1 vs != 0)
