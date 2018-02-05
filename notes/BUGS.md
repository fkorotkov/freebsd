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

# Tool Chain bugs
## arm
lld refuses to link pre-armv7 objects
### arm
```
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.arm/tmp/usr/bin/ld: error: lld uses blx instruction, no object with architecture supporting feature detected.
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.arm/tmp/usr/bin/ld: error: lld uses extended branch encoding, no object with architecture supporting feature detected.
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.arm/tmp/usr/bin/ld: error: lld may use movt/movw, no object with architecture supporting feature detected.
```

### armv6
```
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.armv6/tmp/usr/bin/ld: error: lld uses extended branch encoding, no object with architecture supporting feature detected.
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.armv6/tmp/usr/bin/ld: error: lld may use movt/movw, no object with architecture supporting feature detected.
```

### armv7
```
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.armeb/tmp/usr/bin/ld: error: lld uses blx instruction, no object with architecture supporting feature detected.
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.armeb/tmp/usr/bin/ld: error: lld uses extended branch encoding, no object with architecture supporting feature detected.
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/arm.armeb/tmp/usr/bin/ld: error: lld may use movt/movw, no object with architecture supporting feature detected.
```
also many lines of
```
subsection length greater than section length
```

## powerpc buildworld
All powerpc build worlds fail.
### powerpc
```
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc/tmp/usr/bin/ld: error: can't create dynamic relocation R_PPC_LOCAL24PC against symbol: _GLOBAL_OFFSET_TABLE_ in readonly segment; recompile object files with -fPIC
>>> defined in <internal>
>>> referenced by /scratch/tmp/emaste/freebsd/contrib/gcc/crtstuff.c
>>>               /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc/tmp/usr/lib/crtbeginS.o:(__do_global_dtors_aux)

/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc/tmp/usr/bin/ld: error: can't create dynamic relocation R_PPC_GOT16 against symbol: __do_global_dtors_aux.completed in readonly segment; recompile object files with -fPIC
>>> defined in /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc/tmp/usr/lib/crtbeginS.o
>>> referenced by /scratch/tmp/emaste/freebsd/contrib/gcc/crtstuff.c
>>>               /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc/tmp/usr/lib/crtbeginS.o:(__do_global_dtors_aux)
```
### powerpc64
```
/scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc64/tmp/usr/bin/ld: error: can't create dynamic relocation R_PPC64_REL24 against local symbol in readonly segment; recompile object files with -fPIC
>>> defined in /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc64/tmp/usr/lib/crtbeginS.o
>>> referenced by /scratch/tmp/emaste/freebsd/contrib/gcc/crtstuff.c
>>>               /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/powerpc.powerpc64/tmp/usr/lib/crtbeginS.o:(.fini+0x0)
```
### powerpcspe
```
cc: error: unknown argument: '-mspe=yes'
cc: error: unknown argument: '-mfloat-gprs=double'
```

## mips buildworld

### mipsn32
```
chdir.S:3:239: error: la used to load 64-bit address
```

## riscv
### riscv64
```
/usr/local/bin/riscv64-freebsd-ld: cannot find -lstdc++
```
### riscv64sf
```
/usr/local/bin/riscv64-freebsd-ld: cannot find -lstdc++
```
