#ifndef TFTP_H
#define TFTP_H

#include <stdint.h>
#include <stddef.h>
#include <fs/file.h>

#define UNDI_GET_INFORMATION 0xC

#define TFTP_OPEN 0x0020
struct pxenv_open {
    uint16_t status;
    uint32_t sip;
    uint32_t gip;
    uint8_t name[128];
    uint16_t port;
    uint16_t packet_size;
 } __attribute__((packed));

#define TFTP_READ 0x22
struct pxenv_read {
    uint16_t status;
    uint16_t pn;
    uint16_t bsize;
    uint16_t boff;
    uint16_t bseg;
} __attribute__((packed));

#define TFTP_GET_FILE_SIZE 0x25
struct pxenv_get_file_size {
    uint16_t status;
    uint32_t sip;
    uint32_t gip;
    uint8_t name[128];
    uint32_t file_size;
} __attribute__((packed));

#define TFTP_CLOSE 0x21

//server_ip and server_port can be 0 for default
// XXX: This needs to be fixed
#if defined (BIOS)
struct file_handle *tftp_open(uint32_t server_ip, uint16_t server_port, const char *name);
#elif defined (UEFI)
struct file_handle *tftp_open(struct volume *part,  const char *name);
#endif

uint32_t get_boot_server_info(void);

#endif
