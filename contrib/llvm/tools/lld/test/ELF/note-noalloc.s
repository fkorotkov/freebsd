// REQUIRES: x86
// RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-freebsd %s -o %t.o
// RUN: ld.lld %t.o -o %t -shared
// RUN: llvm-readobj -program-headers -sections %t | FileCheck %s

// PR37361: A note without SHF_ALLOC should not create a PT_NOTE program
// header (but should have a SHT_NOTE section).

// CHECK: SHT_NOTE
// CHECK-NOT: PT_NOTE

        .section        .note.test,"",@note
        .quad 1234
