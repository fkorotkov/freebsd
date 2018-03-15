/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2018 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Konstantin Belousov <kib@FreeBSD.org>
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/pcpu.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <sys/systm.h>
#include <vm/vm.h>
#include <vm/vm_param.h>
#include <vm/vm_extern.h>
#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_page.h>

static int
cp_fast0(vm_offset_t uva, size_t len, bool write,
    void (*f)(vm_offset_t, void *), void *arg)
{
	struct pcpu *pc;
	vm_page_t m[2];
	pt_entry_t *pte;
	vm_offset_t kaddr;
	int error, i, plen;
	bool sleepable;

	plen = howmany(uva - trunc_page(uva) + len, PAGE_SIZE);
	MPASS(plen <= nitems(m));
	error = 0;
	i = vm_fault_quick_hold_pages(&curproc->p_vmspace->vm_map, uva, len,
	    (write ? VM_PROT_WRITE : VM_PROT_READ) | VM_PROT_QUICK_NOFAULT,
	    m, nitems(m));
	if (i != plen)
		return (EFAULT);
	sched_pin();
	pc = get_pcpu();
	if (curthread->td_vslock_sz > 0) {
		sleepable = false;
		mtx_lock(&pc->pc_copyout_mlock);
		kaddr = pc->pc_copyout_maddr;
	} else {
		sleepable = true;
		sx_xlock(&pc->pc_copyout_slock);
		kaddr = pc->pc_copyout_saddr;
	}
	for (i = 0, pte = vtopte(kaddr); i < plen; i++, pte++) {
		*pte = PG_V | PG_RW | PG_A | PG_M | VM_PAGE_TO_PHYS(m[i]) |
		    pmap_cache_bits(pmap_page_get_memattr(m[i]), FALSE);
		invlpg(kaddr + ptoa(i));
	}
	kaddr += uva - trunc_page(uva);
	f(kaddr, arg);
	sched_unpin();
	if (sleepable)
		sx_xunlock(&pc->pc_copyout_slock);
	else
		mtx_unlock(&pc->pc_copyout_mlock);
	for (i = 0; i < plen; i++) {
		vm_page_lock(m[i]);
		vm_page_unhold(m[i]);
		vm_page_unlock(m[i]);
	}
	return (error);
}

struct copyinstr_arg0 {
	vm_offset_t kc;
	size_t len;
	size_t alen;
	bool end;
};

static void
copyinstr_fast0(vm_offset_t kva, void *arg)
{
	struct copyinstr_arg0 *ca;
	char c;

	ca = arg;
	MPASS(ca->alen == 0 && ca->len > 0 && !ca->end);
	while (ca->alen < ca->len && !ca->end) {
		c = *(char *)(kva + ca->alen);
		*(char *)ca->kc = c;
		ca->alen++;
		ca->kc++;
		if (c == '\0')
			ca->end = true;
	}
}

int
copyinstr(const void *udaddr, void *kaddr, size_t maxlen, size_t *lencopied)
{
	struct copyinstr_arg0 ca;
	vm_offset_t uc;
	size_t plen;
	int error;

	error = 0;
	ca.end = false;
	for (plen = 0, uc = (vm_offset_t)udaddr, ca.kc = (vm_offset_t)kaddr;
	    plen < maxlen && !ca.end; uc += ca.alen, plen += ca.alen) {
		ca.len = round_page(uc) - uc;
		if (ca.len == 0)
			ca.len = PAGE_SIZE;
		if (plen + ca.len > maxlen)
			ca.len = maxlen - plen;
		ca.alen = 0;
		if (cp_fast0(uc, ca.len, false, copyinstr_fast0, &ca) != 0) {
			error = EFAULT;
			break;
		}
	}
	if (!ca.end && plen == maxlen && error == 0)
		error = ENAMETOOLONG;
	if (lencopied != NULL)
		*lencopied = plen;
	return (error);
}

struct copyin_arg0 {
	vm_offset_t kc;
	size_t len;
};

static void
copyin_fast0(vm_offset_t kva, void *arg)
{
	struct copyin_arg0 *ca;

	ca = arg;
	bcopy((void *)kva, (void *)ca->kc, ca->len);
}

int
copyin(const void *udaddr, void *kaddr, size_t len)
{
	struct copyin_arg0 ca;
	vm_offset_t uc;
	size_t plen;

	for (plen = 0, uc = (vm_offset_t)udaddr, ca.kc = (vm_offset_t)kaddr;
	    plen < len; uc += ca.len, ca.kc += ca.len, plen += ca.len) {
		ca.len = round_page(uc) - uc;
		if (ca.len == 0)
			ca.len = PAGE_SIZE;
		if (plen + ca.len > len)
			ca.len = len - plen;
		if (cp_fast0(uc, ca.len, false, copyin_fast0, &ca) != 0)
			return (EFAULT);
	}
	return (0);
}

static void
copyout_fast0(vm_offset_t kva, void *arg)
{
	struct copyin_arg0 *ca;

	ca = arg;
	bcopy((void *)ca->kc, (void *)kva, ca->len);
}

int
copyout(const void *kaddr, void *udaddr, size_t len)
{
	struct copyin_arg0 ca;
	vm_offset_t uc;
	size_t plen;

	for (plen = 0, uc = (vm_offset_t)udaddr, ca.kc = (vm_offset_t)kaddr;
	    plen < len; uc += ca.len, ca.kc += ca.len, plen += ca.len) {
		ca.len = round_page(uc) - uc;
		if (ca.len == 0)
			ca.len = PAGE_SIZE;
		if (plen + ca.len > len)
			ca.len = len - plen;
		if (cp_fast0(uc, ca.len, true, copyout_fast0, &ca) != 0)
			return (EFAULT);
	}
	return (0);
}

static void
fubyte_fast0(vm_offset_t kva, void *arg)
{

	*(int *)arg = *(u_char *)kva;
}

int
fubyte(volatile const void *base)
{
	int res;

	if (cp_fast0((vm_offset_t)base, sizeof(char), false, fubyte_fast0,
	    &res) != 0)
		return (-1);
	return (res);
}

static void
fuword16_fast0(vm_offset_t kva, void *arg)
{

	*(int *)arg = *(uint16_t *)kva;
}

int
fuword16(volatile const void *base)
{
	int res;

	if (cp_fast0((vm_offset_t)base, sizeof(uint16_t), false, fuword16_fast0,
	    &res) != 0)
		return (-1);
	return (res);
}

static void
fueword_fast0(vm_offset_t kva, void *arg)
{

	*(uint32_t *)arg = *(uint32_t *)kva;
}

int
fueword(volatile const void *base, long *val)
{
	uint32_t res;

	if (cp_fast0((vm_offset_t)base, sizeof(long), false, fueword_fast0,
	    &res) != 0)
		return (-1);
	*val = res;
	return (0);
}

int
fueword32(volatile const void *base, int32_t *val)
{
	uint32_t res;

	if (cp_fast0((vm_offset_t)base, sizeof(int32_t), false, fueword_fast0,
	    &res) != 0)
		return (-1);
	*val = res;
	return (0);
}

static void
subyte_fast0(vm_offset_t kva, void *arg)
{

	*(u_char *)kva = *(int *)arg;
}

int
subyte(volatile void *base, int byte)
{

	return (cp_fast0((vm_offset_t)base, sizeof(u_char), true, subyte_fast0,
	    &byte) != 0 ? -1 : 0);
}

static void
suword16_fast0(vm_offset_t kva, void *arg)
{

	*(int *)kva = *(uint16_t *)arg;
}

int
suword16(volatile void *base, int word)
{

	return (cp_fast0((vm_offset_t)base, sizeof(int16_t), true,
	    suword16_fast0, &word) != 0 ? -1 : 0);
}

static void
suword_fast0(vm_offset_t kva, void *arg)
{

	*(int *)kva = *(uint32_t *)arg;
}

int
suword(volatile void *base, long word)
{

	return (cp_fast0((vm_offset_t)base, sizeof(long), true,
	    suword_fast0, &word) != 0 ? -1 : 0);
}

int
suword32(volatile void *base, int32_t word)
{


	return (cp_fast0((vm_offset_t)base, sizeof(int32_t), true,
	    suword_fast0, &word) != 0 ? -1 : 0);
}

struct casueword_arg0 {
	uint32_t oldval;
	uint32_t newval;
};

static void
casueword_fast0(vm_offset_t kva, void *arg)
{
	struct casueword_arg0 *ca;

	ca = arg;
	atomic_fcmpset_int((u_int *)kva, &ca->oldval, ca->newval);
}

int
casueword32(volatile uint32_t *base, uint32_t oldval, uint32_t *oldvalp,
    uint32_t newval)
{
	struct casueword_arg0 ca;
	int res;

	ca.oldval = oldval;
	ca.newval = newval;
	res = cp_fast0((vm_offset_t)base, sizeof(int32_t), true,
	    casueword_fast0, &ca);
	if (res == 0) {
		*oldvalp = ca.oldval;
		return (0);
	}
	return (-1);
}

int
casueword(volatile u_long *base, u_long oldval, u_long *oldvalp, u_long newval)
{
	struct casueword_arg0 ca;
	int res;

	ca.oldval = oldval;
	ca.newval = newval;
	res = cp_fast0((vm_offset_t)base, sizeof(int32_t), true,
	    casueword_fast0, &ca);
	if (res == 0) {
		*oldvalp = ca.oldval;
		return (0);
	}
	return (-1);
}
