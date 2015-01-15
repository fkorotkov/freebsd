/*-
 * Copyright (c) 2014 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Semihalf under
 * the sponsorship of the FreeBSD Foundation.
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
#include <sys/proc.h>

#ifdef KDB
#include <sys/kdb.h>
#endif

#include <ddb/ddb.h>
#include <ddb/db_variables.h>

#include <machine/cpu.h>
#include <machine/pcb.h>

static int
db_frame(struct db_variable *vp, db_expr_t *valuep, int op)
{
	long *reg;

	if (kdb_frame == NULL)
		return (0);

	reg = (long *)((uintptr_t)kdb_frame + (db_expr_t)vp->valuep);
	if (op == DB_VAR_GET)
		*valuep = *reg;
	else
		*reg = *valuep;
	return (1);
}

#define DB_OFFSET(x)	(db_expr_t *)offsetof(struct trapframe, x)
struct db_variable db_regs[] = {
	{ "spsr", DB_OFFSET(tf_spsr),	db_frame },
	{ "x0", DB_OFFSET(tf_x[0]),	db_frame },
	{ "x1", DB_OFFSET(tf_x[1]),	db_frame },
	{ "x2", DB_OFFSET(tf_x[2]),	db_frame },
	{ "x3", DB_OFFSET(tf_x[3]),	db_frame },
	{ "x4", DB_OFFSET(tf_x[4]),	db_frame },
	{ "x5", DB_OFFSET(tf_x[5]),	db_frame },
	{ "x6", DB_OFFSET(tf_x[6]),	db_frame },
	{ "x7", DB_OFFSET(tf_x[7]),	db_frame },
	{ "x8", DB_OFFSET(tf_x[8]),	db_frame },
	{ "x9", DB_OFFSET(tf_x[9]),	db_frame },
	{ "x10", DB_OFFSET(tf_x[10]),	db_frame },
	{ "x11", DB_OFFSET(tf_x[11]),	db_frame },
	{ "x12", DB_OFFSET(tf_x[12]),	db_frame },
	{ "x13", DB_OFFSET(tf_x[13]),	db_frame },
	{ "x14", DB_OFFSET(tf_x[14]),	db_frame },
	{ "x15", DB_OFFSET(tf_x[15]),	db_frame },
	{ "x16", DB_OFFSET(tf_x[16]),	db_frame },
	{ "x17", DB_OFFSET(tf_x[17]),	db_frame },
	{ "x18", DB_OFFSET(tf_x[18]),	db_frame },
	{ "x19", DB_OFFSET(tf_x[19]),	db_frame },
	{ "x20", DB_OFFSET(tf_x[20]),	db_frame },
	{ "x21", DB_OFFSET(tf_x[21]),	db_frame },
	{ "x22", DB_OFFSET(tf_x[22]),	db_frame },
	{ "x23", DB_OFFSET(tf_x[23]),	db_frame },
	{ "x24", DB_OFFSET(tf_x[24]),	db_frame },
	{ "x25", DB_OFFSET(tf_x[25]),	db_frame },
	{ "x26", DB_OFFSET(tf_x[26]),	db_frame },
	{ "x27", DB_OFFSET(tf_x[27]),	db_frame },
	{ "x28", DB_OFFSET(tf_x[28]),	db_frame },
	{ "x29", DB_OFFSET(tf_x[29]),	db_frame },
	{ "lr", DB_OFFSET(tf_lr),	db_frame },
	{ "elr", DB_OFFSET(tf_elr),	db_frame },
	{ "sp", DB_OFFSET(tf_sp), db_frame },
};

struct db_variable *db_eregs = db_regs + sizeof(db_regs)/sizeof(db_regs[0]);

void
db_show_mdpcpu(struct pcpu *pc)
{
}

static int
db_validate_address(vm_offset_t addr)
{
	/* XXX ARM64TODO: Revisit once pmap_extract() is implemented */
#if 0
	struct proc *p = curproc;
	struct pmap *pmap;

	if (!p || !p->p_vmspace || !p->p_vmspace->vm_map.pmap ||
	    addr >= VM_MAXUSER_ADDRESS)
		pmap = pmap_kernel();
	else
		pmap = p->p_vmspace->vm_map.pmap;

	return (pmap_extract(pmap, addr) == FALSE);
#endif
	return 0;
}

/*
 * Read bytes from kernel address space for debugger.
 */
int
db_read_bytes(vm_offset_t addr, size_t size, char *data)
{
	const char *src = (const char *)addr;

	while (size-- > 0) {
		if (db_validate_address((u_int)src)) {
			db_printf("address %p is invalid\n", src);
			return (-1);
		}
		*data++ = *src++;
	}
	return (0);
}

/*
 * Write bytes to kernel address space for debugger.
 */
int
db_write_bytes(vm_offset_t addr, size_t size, char *data)
{
	char *dst;

	dst = (char *)addr;
	while (size-- > 0) {
		if (db_validate_address((u_int)dst)) {
			db_printf("address %p is invalid\n", dst);
			return (-1);
		}
		*dst++ = *data++;
	}

	/*
	 * XXX ARM64TODO: Revisit while we have D-cache switched on.
	 * For now, take care of I-cache and barriers only.
	 */
	dsb();
	__asm __volatile("ic ialluis" : : : "memory");
	dsb();
	isb();

	return (0);
}
