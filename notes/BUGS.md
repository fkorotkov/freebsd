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
