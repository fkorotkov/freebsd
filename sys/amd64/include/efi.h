#ifndef _MACHINE_EFI_H_
#define _MACHINE_EFI_H_

struct efi_map_header {
        size_t          memory_size;
        size_t          descriptor_size;
        uint64_t        descriptor_version;
};

struct efi_descriptor {
	uint32_t	type;
	vm_offset_t	physical_start;
	vm_offset_t	virtual_start;
	uint64_t	pages;
	uint64_t	attribute;
};

#define efi_next_descriptor(ptr, size) \
	((struct efi_descriptor *)(((uint8_t *) ptr) + size))

#endif  /* _MACHINE_EFI_H_ */
