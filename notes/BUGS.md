# Tool Chain bugs
## arm
lld refuses to link pre-armv7 objects.

armv6 reports two errors:
```
lld uses extended branch encoding, no object with architecture supporting feature detected.
lld may use movt/movw, no object with architecture supporting feature detected.
```

arm and armeb reports one additional error:
```
lld uses blx instruction, no object with architecture supporting feature detected.
```

See also a
[mailing list post about movt/movw](https://lists.freebsd.org/pipermail/freebsd-arm/2018-January/017389.html)
concerning armv7, although in practice it seems we do not emit movt/movw.

An armeb build also emits many times
```
subsection length greater than section length
```
which comes from `ARMAttributeParser::ParseSubsection()` in
_contrib/llvm/lib/Support/ARMAttributeParser.cpp_.

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

# Build
src.conf.5 knobs need documentation:
* `WITHOUT_BSD_GREP`
* `WITH_BSD_GREP_FASTMATCH`
* `WITH_GNU_DIFF`
* `WITH_GNU_GREP`
* `WITH_LIB32`
* `WITH_LOADER_FORCE_LE`
* `WITHOUT_REPRODUCIBLE_BUILD`
* `WITH_RESCUE`
* `WITH_SENDMAIL`
* `WITH_TCP_WRAPPERS`
* `WITH_TESTS`
* `WITH_TESTS_SUPPORT`

# ASLR + Linuxulator
0a541b719b contains a workaround for a panic from the Linuxulator with the
ASLR patch applied, which probably disables or otherwise renders ASLR
ineffective.

I find the logic in `linux_proc_exec` and `linux_proc_init` somewhat odd.
TODO investigate the special cases for handling execing to/from non-FreeBSD
ABIs in those functions and `__elfN(imgact))`.
