#if defined (UEFI)
#include <pxe/tftp.h>
#include <pxe/pxe.h>
#include <lib/real.h>
#include <lib/print.h>
#include <lib/libc.h>
#include <mm/pmm.h>
#include <lib/misc.h>

struct file_handle *tftp_open(struct volume *part, const char *name) {
	print("tftp_open: name = %s\n", name);
    EFI_PXE_BASE_CODE *pxe = part->pxe_base_code;
	//EFI_IP_ADDRESS *server_ip = pxe->Mode->DhcpAck.Dhcpv4.BootpSiAddr;
	EFI_IP_ADDRESS *server_ip = pxe->Mode->ProxyOfferReceived ? &pxe->Mode->ProxyOffer.Dhcpv4.BootpSiAddr : &pxe->Mode->DhcpAck.Dhcpv4.BootpSiAddr;


	// TODO: check ProxyOfferReceived and the ipv4/ipv6 shit at least

	//panic(false, "tftp_open: ip = %d.%d.%d.%d", server_ip->v4.Addr[0], server_ip->v4.Addr[1], server_ip->v4.Addr[2], server_ip->v4.Addr[3]);
	UINT64 tftp_buffer_size;
	EFI_STATUS status = pxe->Mtftp(
		pxe,
		EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
		NULL,
		FALSE,
		&tftp_buffer_size,
		NULL,
		server_ip,
		name,	// IN CHAR8 *Filename OPTIONAL
		NULL,	// IN EFI_PXE_BASE_CODE_MTFTP_INFO *Info OPTIONAL
		TRUE	// IN BOOLEAN DontUseBuffer
	);

	if (status != EFI_SUCCESS) {
		print("tftp_open: Mtftp GET_FILE_SIZE failed with status %d", status);
		tftp_buffer_size = 1<<24;
		//return NULL;
	}

	print("tftp_open: tftp_buffer_size = %d\n", tftp_buffer_size);

    struct file_handle *handle = ext_mem_alloc(sizeof(struct file_handle));

    handle->size = tftp_buffer_size;
    handle->is_memfile = true;
    handle->pxe = true;
    //handle->pxe_ip = server_ip;
    //handle->pxe_port = server_port;

	print("tftp_open: tftp_buffer_size = %d\n", tftp_buffer_size);
	handle->fd = ext_mem_alloc(handle->size);

	print("tftp_open: tftp_buffer_size = %d\n", tftp_buffer_size);
	UINTN block_size = 1024;

    UINT64 download_buffer_length = handle->size;
	status = pxe->Mtftp(pxe, EFI_PXE_BASE_CODE_TFTP_READ_FILE, handle->fd, FALSE, &download_buffer_length, &block_size,
	        server_ip,      // IN EFI_IP_ADDRESS *ServerIp
        name,    // IN CHAR8 *Filename OPTIONAL
        NULL,                                       // IN EFI_PXE_BASE_CODE_MTFTP_INFO *Info OPTIONAL
        FALSE                                       // IN BOOLEAN DontUseBuffer
	);

	if (status != EFI_SUCCESS) {
		panic(false, "tftp_open: Mtftp READ_FILE failed with status %d", status);
	}

	if (download_buffer_length != handle->size) {
		//panic(false, "tftp_open: download_buffer_length != handle->size");
	}


	return handle;
}
#endif