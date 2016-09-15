# $FreeBSD$

LD_EMULATION_aarch64=aarch64elf
LD_EMULATION_amd64=elf_x86_64_fbsd
LD_EMULATION_arm=armelf_fbsd
LD_EMULATION_armeb=armelf_fbsd
LD_EMULATION_armv6=armelf_fbsd
LD_EMULATION_i386=elf_i386_fbsd
LD_EMULATION_mips= elf32btsmip_fbsd
LD_EMULATION_mips64= elf64btsmip_fbsd
LD_EMULATION_mipsel= elf32ltsmip_fbsd
LD_EMULATION_mips64el= elf64ltsmip_fbsd
LD_EMULATION_mipsn32= elf32btsmipn32_fbsd
LD_EMULATION_mipsn32el= elf32btsmipn32_fbsd   # I don't think this is a thing that works
LD_EMULATION_powerpc= elf32ppc_fbsd
LD_EMULATION_powerpc64= elf32ppc_fbsd
LD_EMULATION_riscv= elf32ppc_fbsd
LD_EMULATION_sparc64= elf32ppc_fbsd
LD_EMULATION=${LD_EMULATION_${MACHINE_ARCH}}
