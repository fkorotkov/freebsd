# $FreeBSD$
#
# Option file for FreeBSD /usr/src builds.
#
# Users define WITH_FOO and WITHOUT_FOO on the command line or in /etc/src.conf
# and /etc/make.conf files. These translate in the build system to MK_FOO={yes,no}
# with sensible (usually) defaults.
#
# Makefiles must include bsd.opts.mk after defining specific MK_FOO options that
# are applicable for that Makefile (typically there are none, but sometimes there
# are exceptions). Recursive makes usually add MK_FOO=no for options that they wish
# to omit from that make.
#
# Makefiles must include bsd.mkopt.mk before they test the value of any MK_FOO
# variable.
#
# Makefiles may also assume that this file is included by src.opts.mk should it
# need variables defined there prior to the end of the Makefile where
# bsd.{subdir,lib.bin}.mk is traditionally included.
#
# The old-style YES_FOO and NO_FOO are being phased out. No new instances of them
# should be added. Old instances should be removed since they were just to
# bridge the gap between FreeBSD 4 and FreeBSD 5.
#
# Makefiles should never test WITH_FOO or WITHOUT_FOO directly (although an
# exception is made for _WITHOUT_SRCONF which turns off this mechanism
# completely inside bsd.*.mk files).
#

.if !target(__<src.opts.mk>__)
__<src.opts.mk>__:

.include <bsd.own.mk>

#
# Define MK_* variables (which are either "yes" or "no") for users
# to set via WITH_*/WITHOUT_* in /etc/src.conf and override in the
# make(1) environment.
# These should be tested with `== "no"' or `!= "no"' in makefiles.
# The NO_* variables should only be set by makefiles for variables
# that haven't been converted over.
#

# These options are used by the src builds

__DEFAULT_YES_OPTIONS = \
    ACCT \
    ACPI \
    AMD \
    APM \
    AT \
    ATM \
    AUDIT \
    AUTHPF \
    AUTOFS \
    BHYVE \
    BINUTILS \
    BINUTILS_BOOTSTRAP \
    BLACKLIST \
    BLUETOOTH \
    BOOT \
    BOOTPARAMD \
    BOOTPD \
    BSD_CPIO \
    BSD_GREP \
    BSDINSTALL \
    BSNMP \
    BZIP2 \
    CALENDAR \
    CAPSICUM \
    CASPER \
    CCD \
    CDDL \
    CPP \
    CROSS_COMPILER \
    CRYPT \
    CTM \
    CUSE \
    CXX \
    DIALOG \
    DICT \
    DMAGENT \
    DYNAMICROOT \
    ED_CRYPTO \
    EE \
    EFI \
    ELFTOOLCHAIN_BOOTSTRAP \
    EXAMPLES \
    FDT \
    FILE \
    FINGER \
    FLOPPY \
    FMTREE \
    FORTH \
    FP_LIBC \
    FREEBSD_UPDATE \
    FTP \
    GAMES \
    GPIO \
    HAST \
    HTML \
    HYPERV \
    ICONV \
    INET \
    INET6 \
    INETD \
    IPFILTER \
    IPFW \
    ISCSI \
    JAIL \
    KDUMP \
    KVM \
    LDNS \
    LDNS_UTILS \
    LEGACY_CONSOLE \
    LIB32 \
    LIBPTHREAD \
    LIBTHR \
    LOCALES \
    LOCATE \
    LPR \
    LS_COLORS \
    LZMA_SUPPORT \
    MAIL \
    MAILWRAPPER \
    MAKE \
    NDIS \
    NETCAT \
    NETGRAPH \
    NLS_CATALOGS \
    NS_CACHING \
    NTP \
    OPENSSL \
    PAM \
    PC_SYSINSTALL \
    PF \
    PKGBOOTSTRAP \
    PMC \
    PORTSNAP \
    PPP \
    QUOTAS \
    RADIUS_SUPPORT \
    RBOOTD \
    REPRODUCIBLE_BUILD \
    RESCUE \
    ROUTED \
    SETUID_LOGIN \
    SHAREDOCS \
    SOURCELESS \
    SOURCELESS_HOST \
    SOURCELESS_UCODE \
    SVNLITE \
    SYSCONS \
    SYSTEM_COMPILER \
    TALK \
    TCSH \
    TELNET \
    TEXTPROC \
    TFTP \
    TIMED \
    UNBOUND \
    USB \
    UTMPX \
    VI \
    VT \
    WIRELESS \
    WPA_SUPPLICANT_EAPOL \
    ZFS \
    ZONEINFO

__DEFAULT_NO_OPTIONS = \
    BSD_GREP \
    BSD_GREP_FASTMATCH \
    CLANG_EXTRAS \
    DTRACE_TESTS \
    GNU_DIFF \
    GNU_GREP \
    GNU_GREP_COMPAT \
    HESIOD \
    LIBSOFT \
    LOADER_FORCE_LE \
    NAND \
    OFED \
    OPENLDAP \
    RPCBIND_WARMSTART_SUPPORT \
    SENDMAIL \
    SHARED_TOOLCHAIN \
    SORT_THREADS \
    SVN \
    TCP_WRAPPERS \
    ZONEINFO_LEAPSECONDS_SUPPORT \
    ZONEINFO_OLD_TIMEZONES_SUPPORT \


#
# Default behaviour of some options depends on the architecture.  Unfortunately
# this means that we have to test TARGET_ARCH (the buildworld case) as well
# as MACHINE_ARCH (the non-buildworld case).  Normally TARGET_ARCH is not
# used at all in bsd.*.mk, but we have to make an exception here if we want
# to allow defaults for some things like clang to vary by target architecture.
# Additional, per-target behavior should be rarely added only after much
# gnashing of teeth and grinding of gears.
#
.if defined(TARGET_ARCH)
__T=${TARGET_ARCH}
.else
__T=${MACHINE_ARCH}
.endif
.if defined(TARGET)
__TT=${TARGET}
.else
__TT=${MACHINE}
.endif

.include <bsd.compiler.mk>
# If the compiler is not C++11 capable, disable Clang.

.if ${COMPILER_FEATURES:Mc++11} && (${__T} == "aarch64" || \
    ${__T} == "amd64" || ${__TT} == "arm" || ${__T} == "i386" || \
    ${__TT} == "mips" || ${__T:Mpowerpc*} != "")
# Clang is enabled, and will be installed as the default /usr/bin/cc.
__DEFAULT_YES_OPTIONS+=CLANG CLANG_BOOTSTRAP CLANG_FULL CLANG_IS_CC LLD
__DEFAULT_YES_OPTIONS+=LLD_BOOTSTRAP LLD_IS_LD
.elif ${COMPILER_FEATURES:Mc++11} && ${__T:Mriscv*} == "" && ${__T} != "sparc64"
# If an external compiler that supports C++11 is used as ${CC} and Clang
# supports the target, then Clang is enabled.
__DEFAULT_YES_OPTIONS+=CLANG CLANG_FULL
__DEFAULT_NO_OPTIONS+=CLANG_BOOTSTRAP CLANG_IS_CC LLD
__DEFAULT_NO_OPTIONS+=LLD_BOOTSTRAP LLD_IS_LD
.else
# Everything else disables Clang.
__DEFAULT_NO_OPTIONS+=CLANG CLANG_BOOTSTRAP CLANG_FULL CLANG_IS_CC LLD
__DEFAULT_NO_OPTIONS+=LLD_BOOTSTRAP LLD_IS_LD
.endif
# Only program remaining from in-tree binutils (for bootstrap) is GNU as for
# i386/amd64 - assume we will use LLD or external binutils linker and Clang IAS
# everywhere else.
.if ${__T} != "amd64" && ${__T} != "i386"
BROKEN_OPTIONS+=BINUTILS_BOOTSTRAP
.endif
# In-tree binutils/gcc are older versions without modern architecture support.
.if ${__T} == "aarch64" || ${__T:Mriscv*} != ""
BROKEN_OPTIONS+=BINUTILS
.endif
.if ${__T:Mriscv*} != ""
BROKEN_OPTIONS+=PROFILE # "sorry, unimplemented: profiler support for RISC-V"
BROKEN_OPTIONS+=TESTS   # "undefined reference to `_Unwind_Resume'"
BROKEN_OPTIONS+=CXX     # "libcxxrt.so: undefined reference to `_Unwind_Resume_or_Rethrow'"
.endif
.if ${__T} == "aarch64" || ${__T} == "amd64" || ${__T} == "i386" || \
    ${__T:Mriscv*} != ""
__DEFAULT_YES_OPTIONS+=LLVM_LIBUNWIND
.else
__DEFAULT_NO_OPTIONS+=LLVM_LIBUNWIND
.endif
.if ${__T} == "aarch64" || ${__T} == "amd64" || ${__T} == "i386"
__DEFAULT_YES_OPTIONS+=LLDB
.else
__DEFAULT_NO_OPTIONS+=LLDB
.endif
# LLVM lacks support for FreeBSD 64-bit atomic operations for ARMv4/ARMv5
.if ${__T} == "arm" || ${__T} == "armeb"
BROKEN_OPTIONS+=LLDB
.endif
# Only doing soft float API stuff on armv6 and armv7
.if ${__T} != "armv6" && ${__T} != "armv7"
BROKEN_OPTIONS+=LIBSOFT
.endif
.if ${__T:Mmips*}
BROKEN_OPTIONS+=SSP
# profiling won't work on MIPS64 because there is only assembly for o32
BROKEN_OPTIONS+=PROFILE
# binaries too large? "relocation R_MIPS_GOT_DISP out of range" errors
BROKEN_OPTIONS+=CLANG RESCUE
# error: ABI 'o32' is not supported on CPU 'mips3'
BROKEN_OPTIONS+=LIB32
.endif
.if ${__T:Mmips*} || ${__T:Mpowerpc*} || ${__T:Msparc64} || ${__T:Mriscv*}
BROKEN_OPTIONS+=EFI
.endif
.if ${__T} == "aarch64" || ${__T} == "amd64" || ${__T} == "i386" || \
    ${__T} == "powerpc64" || ${__T} == "sparc64"
__DEFAULT_YES_OPTIONS+=CXGBETOOL
.else
__DEFAULT_NO_OPTIONS+=CXGBETOOL
.endif

.include <bsd.mkopt.mk>

#
# MK_* options that default to "yes" if the compiler is a C++11 compiler.
#
.for var in \
    LIBCPLUSPLUS
.if !defined(MK_${var})
.if ${COMPILER_FEATURES:Mc++11}
.if defined(WITHOUT_${var})
MK_${var}:=	no
.else
MK_${var}:=	yes
.endif
.else
.if defined(WITH_${var})
MK_${var}:=	yes
.else
MK_${var}:=	no
.endif
.endif
.endif
.endfor

#
# Force some options off if their dependencies are off.
# Order is somewhat important.
#
.if !${COMPILER_FEATURES:Mc++11}
MK_LLVM_LIBUNWIND:=	no
.endif

.if ${MK_CAPSICUM} == "no"
MK_CASPER:=	no
.endif

.if ${MK_LIBPTHREAD} == "no"
MK_LIBTHR:=	no
.endif

.if ${MK_LDNS} == "no"
MK_LDNS_UTILS:=	no
MK_UNBOUND:= no
.endif

.if ${MK_SOURCELESS} == "no"
MK_SOURCELESS_HOST:=	no
MK_SOURCELESS_UCODE:= no
.endif

.if ${MK_CDDL} == "no"
MK_ZFS:=	no
MK_CTF:=	no
.endif

.if ${MK_CRYPT} == "no"
MK_OPENSSL:=	no
MK_OPENSSH:=	no
MK_KERBEROS:=	no
.endif

.if ${MK_CXX} == "no"
MK_CLANG:=	no
MK_TESTS:=	no
.endif

.if ${MK_DIALOG} == "no"
MK_BSDINSTALL:=	no
.endif

.if ${MK_MAIL} == "no"
MK_MAILWRAPPER:= no
MK_SENDMAIL:=	no
MK_DMAGENT:=	no
.endif

.if ${MK_NETGRAPH} == "no"
MK_ATM:=	no
MK_BLUETOOTH:=	no
.endif

.if ${MK_NLS} == "no"
MK_NLS_CATALOGS:= no
.endif

.if ${MK_OPENSSL} == "no"
MK_OPENSSH:=	no
MK_KERBEROS:=	no
.endif

.if ${MK_PF} == "no"
MK_AUTHPF:=	no
.endif

.if ${MK_TESTS} == "no"
MK_DTRACE_TESTS:= no
.endif

.if ${MK_ZONEINFO} == "no"
MK_ZONEINFO_LEAPSECONDS_SUPPORT:= no
MK_ZONEINFO_OLD_TIMEZONES_SUPPORT:= no
.endif

.if ${MK_CROSS_COMPILER} == "no"
MK_BINUTILS_BOOTSTRAP:= no
MK_CLANG_BOOTSTRAP:= no
MK_ELFTOOLCHAIN_BOOTSTRAP:= no
.endif

.if ${MK_TOOLCHAIN} == "no"
MK_BINUTILS:=	no
MK_CLANG:=	no
MK_INCLUDES:=	no
MK_LLD:=	no
MK_LLDB:=	no
.endif

.if ${MK_CLANG} == "no"
MK_CLANG_EXTRAS:= no
MK_CLANG_FULL:= no
.endif

#
# MK_* options whose default value depends on another option.
#
.for vv in \
    GSSAPI/KERBEROS \
    MAN_UTILS/MAN
.if defined(WITH_${vv:H})
MK_${vv:H}:=	yes
.elif defined(WITHOUT_${vv:H})
MK_${vv:H}:=	no
.else
MK_${vv:H}:=	${MK_${vv:T}}
.endif
.endfor

#
# Set defaults for the MK_*_SUPPORT variables.
#

#
# MK_*_SUPPORT options which default to "yes" unless their corresponding
# MK_* variable is set to "no".
#
.for var in \
    BLACKLIST \
    BZIP2 \
    INET \
    INET6 \
    KERBEROS \
    KVM \
    NETGRAPH \
    PAM \
    TESTS \
    WIRELESS
.if defined(WITHOUT_${var}_SUPPORT) || ${MK_${var}} == "no"
MK_${var}_SUPPORT:= no
.else
MK_${var}_SUPPORT:= yes
.endif
.endfor

.if !${COMPILER_FEATURES:Mc++11}
MK_LLDB:=	no
.endif

.endif #  !target(__<src.opts.mk>__)
