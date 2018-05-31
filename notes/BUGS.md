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

## arm buildkernel
`_.arm.AVILA`, `_.arm.CAMBRIA`
```
ld: error: xxhash.c:(.ARM.exidx+0x68): relocation R_ARM_PREL31 out of range: -1077893248 is not in [-1073741824, 1073741823]
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

## powerpc buildkernel
`_.powerpc.MPC85XXSPE`
```
cc: error: unknown argument: '-mspe=yes'
cc: error: unknown argument: '-mfloat-gprs=double'
```

`_.powerpc.GENERIC64`, `_.powerpc.LINT64`, `_.powerpc.QORIQ64`
```
/scratch/tmp/emaste/freebsd/sys/dev/drm2/drm_crtc.c:229:63: error: passing 'int *' to parameter of type 'uint32_t *' (aka 'unsigned int *') converts between pointers to integer types with different sign [-Werror,-Wpointer-sign]
        ret = drm_gem_name_create(&dev->mode_config.crtc_names, obj, &new_id);
                                                                     ^~~~~~~
/scratch/tmp/emaste/freebsd/sys/dev/drm2/drm_gem_names.h:62:75: note: passing argument to parameter 'name' here
int drm_gem_name_create(struct drm_gem_names *names, void *obj, uint32_t *name);
                                                                          ^
```

`_.powerpc.MPC85XX`
```
/scratch/tmp/emaste/freebsd/sys/contrib/ncsw/inc/Peripherals/dpaa_ext.h:60:9: error: '__BYTE_ORDER__' macro redefined [-Werror,-Wmacro-redefined]
#define __BYTE_ORDER__ BYTE_ORDER
        ^
<built-in>:31:9: note: previous definition is here
#define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
```

`_.powerpc.LINT`
```
/scratch/tmp/emaste/freebsd/sys/dev/mlx5/mlx5_en/mlx5_en_rl.c:242:2: error: implicit declaration of function 'atomic_add_64' is invalid in C99 [-Werror,-Wimplicit-function-declaration]
        atomic_add_64(&priv->rl.stats.tx_allocate_resource_failure, 1ULL);
        ^
/scratch/tmp/emaste/freebsd/sys/dev/mlx5/mlx5_en/mlx5_en_rl.c:242:2: note: did you mean 'atomic_add_int'?
./machine/atomic.h:141:1: note: 'atomic_add_int' declared here
_ATOMIC_ADD(int)
^
./machine/atomic.h:121:5: note: expanded from macro '_ATOMIC_ADD'
    atomic_add_##type(volatile u_##type *p, u_##type v) {       \
    ^
<scratch space>:18:1: note: expanded from here
atomic_add_int
^
/scratch/tmp/emaste/freebsd/sys/dev/mlx5/mlx5_en/mlx5_en_rl.c:242:2: error: this function declaration is not a prototype [-Werror,-Wstrict-prototypes]
        atomic_add_64(&priv->rl.stats.tx_allocate_resource_failure, 1ULL);
        ^
/scratch/tmp/emaste/freebsd/sys/dev/mlx5/mlx5_en/mlx5_en_rl.c:424:4: error: implicit declaration of function 'atomic_add_64' is invalid in C99 [-Werror,-Wimplicit-function-declaration]
                        atomic_add_64(&rlw->priv->rl.stats.tx_modify_rate_failure, 1ULL);
                        ^
```

`_.powerpc.GENERIC-NODEBUG`, `_.powerpc.GENERIC`
```
ld: error: can't create dynamic relocation R_PPC_ADDR32 against local symbol in readonly segment; recompile object files with -fPIC
>>> defined in locore.o
>>> referenced by trap_subr32.S:305 (/scratch/tmp/emaste/freebsd/sys/powerpc/aim/trap_subr32.S:305)
>>>               locore.o:(.text+0xB8)
```

## mips buildworld

### mipsn32
```
chdir.S:3:239: error: la used to load 64-bit address
```

## mips buildkernel
`_.mips.ERL`
```
error: unknown target CPU 'octeon+'
```

`_.mips.AR71XX_BASE`, `_.mips.RSPRO_STANDALONE`, `_.mips.QCA953X_BASE`,
```
ld: error: target ABI 'n64' is incompatible with 'o32': otusfw_init.o
```

`_.mips.OCTEON1`
```
Assertion failed: (isExpr() && "This is not an expression"), function getExpr, file /scratch/tmp/emaste/freebsd/contrib/llvm/include/llvm/MC/MCInst.h, line 97.
cc: error: unable to execute command: Abort trap (core dumped)
cc: error: clang integrated assembler command failed due to signal (use -v to see invocation)
```

`_.mips.XLP64`
```
error: ABI 'o32' is not supported for 'mips64-unknown-freebsd12.0'
```

`_.mips.XLPN32`
```
sh -c 'MACHINE=mips /scratch/tmp/emaste/freebsd/sys/tools/fdt/make_dtb.sh /scratch/tmp/emaste/freebsd/sys xlp-basic.dts /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/mips.mipsn32/sys/XLPN32'
converting xlp-basic.dts -> /scratch/tmp/emaste/obj/scratch/tmp/emaste/freebsd/mips.mipsn32/sys/XLPN32/xlp-basic.dtb
error: unknown target triple 'mipsn32-unknown-freebsd12.0', please use -triple or -arch
```

`_.mips.XLP`
```
error: ABI 'n64' is not supported for 'mips-unknown-freebsd12.0'
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

## riscv buildkernel
`_.riscv.GENERIC`
```
crypto.kld: In function `swcr_newsession':
/scratch/tmp/emaste/freebsd/sys/opencrypto/cryptosoft.c:871:(.text+0x45a2): relocation truncated to fit: R_RISCV_JAL against `des_ecb_encrypt'
/usr/local/riscv64-unknown-freebsd12.0/bin/ld: final link failed: Nonrepresentable section on output
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
