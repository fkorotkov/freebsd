/*
 * Copyright (C) 2018 Cavium Inc.
 * All rights reserved.
 *
 * Developed by Semihalf.
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
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "opt_platform.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/kernel.h>
#include <sys/rman.h>
#include <sys/module.h>
#include <sys/bus.h>
#include <sys/endian.h>
#include <sys/cpuset.h>
#include <sys/kdb.h>

#include <contrib/dev/acpica/include/acpi.h>
#include <contrib/dev/acpica/include/accommon.h>

#include <dev/acpica/acpivar.h>
#include <dev/acpica/acpi_pcibvar.h>

#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>
#include <dev/pci/pci_host_generic.h>
#include <dev/pci/pci_host_generic_acpi.h>
#include <dev/pci/pcib_private.h>

#include "thunder_pcie_common.h"

#include "pcib_if.h"

static int thunderx2_pcie_acpi_attach(device_t);
static int thunderx2_pcie_acpi_probe(device_t);
static struct resource *
thunderx2_pci_acpi_alloc_resource(device_t, device_t, int,
    int *, rman_res_t, rman_res_t, rman_res_t, u_int);
static int
thunderx2_pci_acpi_read_ivar(device_t, device_t, int,
    uintptr_t *);
static uint32_t
thunderx2_pci_acpi_read_config(device_t, u_int, u_int,
    u_int, u_int, int);

static device_method_t thunderx2_pcie_acpi_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe,		thunderx2_pcie_acpi_probe),
	DEVMETHOD(device_attach,	thunderx2_pcie_acpi_attach),
	DEVMETHOD(bus_alloc_resource,	thunderx2_pci_acpi_alloc_resource),
	DEVMETHOD(bus_read_ivar,	thunderx2_pci_acpi_read_ivar),
	DEVMETHOD(pcib_read_config,	thunderx2_pci_acpi_read_config),

	/* End */
	DEVMETHOD_END
};

DEFINE_CLASS_1(pcib, thunderx2_pcie_acpi_driver, thunderx2_pcie_acpi_methods,
    sizeof(struct generic_pcie_acpi_softc), generic_pcie_acpi_driver);

static devclass_t thunderx2_pcie_acpi_devclass;

DRIVER_MODULE(pcib, acpi, thunderx2_pcie_acpi_driver, thunderx2_pcie_acpi_devclass,
    0, 0);

static int
thunderx2_pcie_acpi_probe(device_t dev)
{
	ACPI_DEVICE_INFO *devinfo;
	ACPI_HANDLE h;
	int root;

	/* Check if we're running on Cavium ThunderX2 */
	if (!CPU_MATCH(CPU_IMPL_MASK | CPU_PART_MASK,
	    CPU_IMPL_CAVIUM, CPU_PART_THUNDERX2, 0, 0))
		return (ENXIO);

	if (acpi_disabled("pcib") || (h = acpi_get_handle(dev)) == NULL ||
	    ACPI_FAILURE(AcpiGetObjectInfo(h, &devinfo)))
		return (ENXIO);
	root = (devinfo->Flags & ACPI_PCI_ROOT_BRIDGE) != 0;
	AcpiOsFree(devinfo);
	if (!root)
		return (ENXIO);

	device_set_desc(dev, "ThunderX2 PCI host controller");
	return (BUS_PROBE_DEFAULT);
}

static int
thunderx2_pcie_acpi_attach(device_t dev)
{
	struct generic_pcie_acpi_softc *sc;
	ACPI_HANDLE handle;
	struct resource *res;
	int rid;
	int error;

	sc = device_get_softc(dev);

	handle = acpi_get_handle(dev);
	if (ACPI_FAILURE(acpi_GetInteger(handle, "_CCA", &sc->base.coherent)))
		sc->base.coherent = 0;
	if (bootverbose)
		device_printf(dev, "Bus is%s cache-coherent\n",
		    sc->base.coherent ? "" : " not");

	acpi_pcib_fetch_prt(dev, &sc->ap_prt);

	error = pci_host_generic_core_attach(dev);
	if (error != 0)
		return (error);

	/*
	 * All subsequent memory regions represent PCIe Memory Windows.
	 */
	for (rid = 1; rid < 4; rid++) {
		res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid, RF_UNMAPPED);
		if (res == NULL)
			break;
		error = rman_manage_region(&sc->base.mem_rman,
		   rman_get_start(res), rman_get_end(res));
		if (error) {
			device_printf(dev, "unable to allocate %lx-%lx range\n",
			    rman_get_start(res), rman_get_end(res));
			return (ENXIO);
		}
		device_printf(dev, "allocating %lx-%lx range\n", rman_get_start(res), rman_get_end(res));
	}

	device_add_child(dev, "pci", -1);

	/*
	 * XXX:
	 * On Thunder X2, pcib16 is a beginning of second ECAM domain.
	 */
	if (device_get_unit(dev) == 16)
		sc->base.ecam = 1;

	/*
	 * XXX:
	 * On ThunderX2, AHCI BAR2 address is wrong. It needs to precisely
	 * match the one described in datasheet. Fixup it unconditionally.
	 */
	if (device_get_unit(dev) == 0) {
		device_printf(dev, "running AHCI BAR fixup\n");
		PCIB_WRITE_CONFIG(dev, 0, 16, 0, 0x18, 0x01440000, 4);
		PCIB_WRITE_CONFIG(dev, 0, 16, 0, 0x1c, 0x40, 4);
		PCIB_WRITE_CONFIG(dev, 0, 16, 1, 0x18, 0x01450000, 4);
		PCIB_WRITE_CONFIG(dev, 0, 16, 1, 0x1c, 0x40, 4);
	}

	return (bus_generic_attach(dev));
}

static struct resource *
thunderx2_pci_acpi_alloc_resource(device_t dev, device_t child, int type,
    int *rid, rman_res_t start, rman_res_t end, rman_res_t count, u_int flags)
{
#if defined(NEW_PCIB) && defined(PCI_RES_BUS)
	struct generic_pcie_acpi_softc *sc;

	if (type == PCI_RES_BUS) {
		sc = device_get_softc(dev);
		return (pci_domain_alloc_bus(sc->base.ecam, child, rid, start,
		    end, count, flags));
	}
#endif

	if (type == SYS_RES_MEMORY)
		return (pci_host_generic_core_alloc_resource(dev, child, type,
		    rid, start, end, count, flags));

	return (bus_generic_alloc_resource(dev, child, type, rid, start, end,
	    count, flags));
}

static int
thunderx2_pci_acpi_read_ivar(device_t dev, device_t child, int index,
    uintptr_t *result)
{
	struct generic_pcie_acpi_softc *sc;
	int secondary_bus;

	sc = device_get_softc(dev);

	if (index == PCIB_IVAR_BUS) {
		/* this pcib adds only pci bus 0 as child */
		if (sc->base.ecam != 0)
			secondary_bus = 0x80;
		else
			secondary_bus = 0;
		*result = secondary_bus;
		return (0);

	}

	if (index == PCIB_IVAR_DOMAIN) {
		*result = 0;
		return (0);
	}

	if (bootverbose)
		device_printf(dev, "ERROR: Unknown index %d.\n", index);
	return (ENOENT);
}

static uint32_t
thunderx2_pci_acpi_read_config(device_t dev, u_int bus, u_int slot,
    u_int func, u_int reg, int bytes)
{

	/*
	 * XXX:
	 * Disable AHCI on socket#2 on ThunderX boards.
	 */
	if (bus == 0x80 && slot == 0x10)
		return ~0;

	return generic_pcie_read_config(dev, bus, slot, func, reg, bytes);
}

