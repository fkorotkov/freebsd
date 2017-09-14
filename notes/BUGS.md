# jemalloc !malloc_slow assertion
When using LLD to link i386 binaries all failed with
`Failed assertion: "!malloc_slow"`

In upstream r322589 / 811a0d83 I forced i386 libc to link with ld.bfd which
worked around the issue. In WIPBSD ld.bfd has been removed and this is not
possible.

Zak discovered that emulators/virtualbox-ose failed to build with the same
error and I reproduced it:

```
kBuild: Generating tstVMStructSize - /usr/home/emaste/src/freebsd-ports/emulators/virtualbox-ose/work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/VMM/tstVMStructRC.h
<jemalloc>: /usr/home/emaste/src/freebsd-wip/contrib/jemalloc/include/jemalloc/internal/tsd.h:239: Failed assertion: "!malloc_slow"
kmk: *** [/usr/home/emaste/src/freebsd-ports/emulators/virtualbox-ose/work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/VMM/tstVMStructRC.h] Error 134
kmk: *** Deleting file `/usr/home/emaste/src/freebsd-ports/emulators/virtualbox-ose/work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/VMM/tstVMStructRC.h'
kmk: *** Waiting for unfinished jobs....
filesplitter: Out of 156 files: 156 rewritten, 0 unchanged. (/usr/home/emaste/src/freebsd-ports/emulators/virtualbox-ose/work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/VirtualBox/include)
kmk_builtin_append "/usr/home/emaste/src/freebsd-ports/emulators/virtualbox-ose/work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/VirtualBox/include/COMWrappers"
kmk: *** Exiting with status 2
```

It turns out this is because the VirtualBox build creates a 32-bit test binary
during the build, for whatever reason:
```
% file ./work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/tstVMStructRC/tstVMStructRC
./work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/tstVMStructRC/tstVMStructRC: ELF 32-bit LSB executable, Intel 80386, version 1 (FreeBSD), dynamically linked, interpreter /libexec/ld-elf.so.1, for FreeBSD 12.0 (1200031), FreeBSD-style, stripped
% ./work/VirtualBox-5.1.26/out/freebsd.amd64/release/obj/tstVMStructRC/tstVMStructRC
<jemalloc>: /usr/home/emaste/src/freebsd-wip/contrib/jemalloc/include/jemalloc/internal/tsd.h:239: Failed assertion: "!malloc_slow"
```
