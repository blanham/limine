#if defined (UEFI)

#include <lib/print.h>
#include <lib/real.h>
#include <pxe/pxe.h>
#include <lib/libc.h>
#include <lib/misc.h>
#include <mm/pmm.h>

struct volume *pxe_bind_volume(void * pxe_base) {
    struct volume *volume = ext_mem_alloc(sizeof(struct volume));

    volume->pxe = true;
	volume->pxe_base_code = pxe_base;

    EFI_PXE_BASE_CODE *pxe_base_code = (EFI_PXE_BASE_CODE *)pxe_base;

    if (pxe_base_code->Mode->Started) {
        print("pxe: Already started\n");
    } else {
        print("pxe: Not started\n");
    }

    if (pxe_base_code->Mode->UsingIpv6) {
        panic(true, "pxe: Using IPv6\n");
    } else {
        print("pxe: Using IPv4\n");
    }

    if (pxe_base_code->Mode->DhcpDiscoverValid) {
        print("pxe: DHCP discover valid\n");
    } else {
        print("pxe: DHCP discover invalid\n");
    }

    if (pxe_base_code->Mode->DhcpAckReceived) {
        print("pxe: DHCP ack received\n");
    } else {
        print("pxe: DHCP ack not received\n");
    }

    if (pxe_base_code->Mode->ProxyOfferReceived) {
        print("pxe: Proxy offer received\n");
    } else {
        printv("pxe: Proxy offer not received\n");
    }

    return volume;
}

void pxe_init(void) {
  
}

#endif
