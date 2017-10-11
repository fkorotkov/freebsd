# MIPS

## world failures

mips.mipsn32
```
/scratch/tmp/emaste/freebsd/lib/libc/mips/sys/sbrk.S:56:2: error: la used to load 64-bit address
 la $12, __curbrk
 ^
/scratch/tmp/emaste/freebsd/lib/libc/mips/sys/sbrk.S:66:2: error: la used to load 64-bit address
 la $12, __curbrk
 ^
/scratch/tmp/emaste/freebsd/lib/libc/mips/sys/sbrk.S:72:2: error: la used to load 64-bit address
 la $25, __cerror; .cpreturn; jr $25
 ^
```

mips.mipshf mips.mipselhf mips.mips64hf mips.mips64elhf
```
/scratch/tmp/emaste/obj/mips.mipshf/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: target floating point ABI '-msoft-float' is incompatible with '-mdouble-float': /scratch/tmp/emaste/obj/mips.mipshf/scratch/tmp/emaste/freebsd/sys/boot/mips/uboot/../../ficl/libficl.a(vm.o)
/scratch/tmp/emaste/obj/mips.mipshf/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: target floating point ABI '-msoft-float' is incompatible with '-mdouble-float': /scratch/tmp/emaste/obj/mips.mipshf/scratch/tmp/emaste/freebsd/sys/boot/mips/uboot/../../ficl/libficl.a(loader.o)
```

## kernel failures

AR71XX_BASE QCA953X_BASE RSPRO_STANDALONE
```
ld -m elf32btsmip_fbsd -EB -d -warn-common --build-id=sha1 -r -d -o otusfw_init.ko.full otusfw_init.fwo otusfw_init.o
ld: error: target ABI 'n64' is incompatible with 'o32': otusfw_init.o
```

ERL
```
error: unknown target CPU 'octeon+'
```

MT7620 RT305X RT5350
```
/scratch/tmp/emaste/freebsd/sys/dev/rt/if_rt.c:310:54: error: cast from 'char **' to 'const char **' must have all intermediate pointers const qualified to be safe [-Werror,-Wcast-qual]
            device_get_unit(dev), "macaddr", (const char **)&var)) {
                                                            ^
```

XLP XLP64
```
cc: error: unknown argument: '-fno-tree-loop-optimize'
cc: error: unknown argument: '-mno-branch-likely'
cc: error: optimization flag '-finline-limit=20000' is not supported [-Werror,-Wignored-optimization-argument]
cc: error: optimization flag '-finline-limit=20000' is not supported [-Werror,-Wignored-optimization-argument]
```

XLPN32
```
converting xlp-basic.dts -> /scratch/tmp/emaste/obj/mips.mipsn32/scratch/tmp/emaste/freebsd/sys/XLPN32/xlp-basic.dtb
error: unknown target triple 'mipsn32-unknown-freebsd12.0', please use -triple or -arch
Error at <standard input>:1:0: Expected /dts-v1/; version string

^
Error at <standard input>:1:0: Failed to find root node /.

^
Failed to parse tree.
```

OCTEON1
```
Assertion failed: (isExpr() && "This is not an expression"), function getExpr, file /scratch/tmp/emaste/freebsd/contrib/llvm/include/llvm/MC/MCInst.h, line 97.
cc: error: unable to execute command: Abort trap (core dumped)
cc: error: clang integrated assembler command failed due to signal (use -v to see invocation)
FreeBSD clang version 5.0.0 (tags/RELEASE_500/final 312559) (based on LLVM 5.0.0svn)
Target: mips64-unknown-freebsd12.0
Thread model: posix
InstalledDir: /scratch/tmp/emaste/obj/mips.mips64/scratch/tmp/emaste/freebsd/tmp/usr/bin
cc: note: diagnostic msg: PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
```

XLR XLR64
```
/scratch/tmp/emaste/freebsd/sys/mips/rmi/msgring.h:213:7: error: unknown instruction
            "c2         0x80002\n"    /* msgld $8 */
             ^
<inline asm>:4:1: note: instantiated into assembly here
c2              0x80002
^
1 error generated.
```

XLRN32
```
/scratch/tmp/emaste/freebsd/sys/mips/mips/exception.S:980:2: error: la used to load 64-bit address
 la $26, pcpu_space
 ^
/scratch/tmp/emaste/freebsd/sys/mips/mips/exception.S:1015:2: error: la used to load 64-bit address
 la $4, 9f; jal panic; nop; .rdata; 9: .asciiz "kernel stack overflow - trapframe at %p"; .text
 ^
```

# powerpc

## world

powerpc.powerpcspe
```
error: unknown target triple 'powerpcspe-unknown-freebsd12.0', please use -triple or -arch
```

powerpc.powerpc64
```
/scratch/tmp/emaste/obj/powerpc.powerpc64/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: can't create dynamic relocation R_PPC64_REL24 against local symbol in readonly segment
>>> defined in /scratch/tmp/emaste/obj/powerpc.powerpc64/scratch/tmp/emaste/freebsd/tmp/usr/lib/crtbeginS.o
>>> referenced by /scratch/tmp/emaste/freebsd/contrib/gcc/crtstuff.c
>>>               /scratch/tmp/emaste/obj/powerpc.powerpc64/scratch/tmp/emaste/freebsd/tmp/usr/lib/crtbeginS.o:(.fini+0x0)
...
```

powerpc.powerpc
```
/scratch/tmp/emaste/obj/powerpc.powerpc/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: can't create dynamic relocation R_PPC_LOCAL24PC against symbol: _GLOBAL_OFFSET_TABLE_ in readonly segment
>>> defined in <internal>
>>> referenced by /scratch/tmp/emaste/freebsd/contrib/gcc/crtstuff.c
>>>               /scratch/tmp/emaste/obj/powerpc.powerpc/scratch/tmp/emaste/freebsd/tmp/usr/lib/crtbeginS.o:(__do_global_dtors_aux)
...
```


## kernel
MPC85XX QORIQ64
```
cc: error: unsupported argument '-me500' to option 'Wa,'
```

MPC85XXSPE
```
cc: warning: argument unused during compilation: '-mabi=spe' [-Wunused-command-line-argument]
cc: warning: argument unused during compilation: '-msoft-float' [-Wunused-command-line-argument]
cc: warning: argument unused during compilation: '-mno-altivec' [-Wunused-command-line-argument]
cc: warning: argument unused during compilation: '-msoft-float' [-Wunused-command-line-argument]
error: unknown target triple 'powerpcspe-unknown-freebsd12.0', please use -triple or -arch
```

LINT64 GENERIC64
```
/scratch/tmp/emaste/freebsd/sys/dev/drm2/drm_crtc.c:229:63: error: passing 'int *' to parameter of type 'uint32_t *' (aka 'unsigned int *') converts between pointers to integer types with different sign [-Werror,-Wpointer-sign]
        ret = drm_gem_name_create(&dev->mode_config.crtc_names, obj, &new_id);
                                                                     ^~~~~~~
/scratch/tmp/emaste/freebsd/sys/dev/drm2/drm_gem_names.h:60:75: note: passing argument to parameter 'name' here
int drm_gem_name_create(struct drm_gem_names *names, void *obj, uint32_t *name);
                                                                          ^
1 error generated.
```

GENERIC GENERIC-NODEBUG LINT
```
linking kernel
ld: error: can't create dynamic relocation R_PPC_ADDR32 against local symbol in readonly segment
>>> defined in locore.o
>>> referenced by locore.o:(.text+0xB0)

ld: error: can't create dynamic relocation R_PPC_ADDR32 against local symbol in readonly segment
>>> defined in locore.o
>>> referenced by locore.o:(.text+0x1E8)

...
```

# arm

## world

arm.arm arm.armv6 arm.armv7
```
/scratch/tmp/emaste/obj/arm.arm/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: /scratch/tmp/emaste/freebsd/gnu/lib/libgcc/../../../contrib/gcc/config/arm/unwind-arm.c:405: relocation R_ARM_CALL out of range
```

arm.armeb
```
/scratch/tmp/emaste/obj/arm.armeb/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: /scratch/tmp/emaste/freebsd/sys/boot/efi/loader/../../common/disk.c:(.ARM.exidx+0x28): relocation R_ARM_PREL31 out of range
/scratch/tmp/emaste/obj/arm.armeb/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: /scratch/tmp/emaste/freebsd/sys/boot/efi/libefi/efi_console.c:(.ARM.exidx+0x8): relocation R_ARM_PREL31 out of range
/scratch/tmp/emaste/obj/arm.armeb/scratch/tmp/emaste/freebsd/tmp/usr/bin/ld: error: /scratch/tmp/emaste/freebsd/sys/boot/ficl/dict.c:(.ARM.exidx+0x30): relocation R_ARM_PREL31 out of range
```

## kernel

AVILA CAMBRIA
```
ld: error: /scratch/tmp/emaste/freebsd/sys/cam/ata/ata_all.c:(.ARM.exidx+0x80): relocation R_ARM_PREL31 out of range
ld: error: /scratch/tmp/emaste/freebsd/sys/cam/ata/ata_all.c:(.ARM.exidx+0x13C): relocation R_ARM_PREL31 out of range
ld: error: /scratch/tmp/emaste/freebsd/sys/cam/cam_periph.c:(.ARM.exidx+0xBC): relocation R_ARM_PREL31 out of range
ld: error: /scratch/tmp/emaste/freebsd/sys/cam/ata/ata_xpt.c:(.ARM.exidx+0x68): relocation R_ARM_PREL31 out of range
ld: error: /scratch/tmp/emaste/freebsd/sys/cam/ata/ata_xpt.c:(.ARM.exidx+0x70): relocation R_ARM_PREL31 out of range
ld: error: /scratch/tmp/emaste/freebsd/sys/cam/cam_xpt.c:(.ARM.exidx+0x18): relocation R_ARM_PREL31 out of range
```
