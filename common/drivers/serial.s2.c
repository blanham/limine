
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <drivers/serial.h>
#include <sys/cpu.h>
#include <lib/misc.h>

static bool serial_initialised = false;

// The options should be as follows:
// 1. If we are on UEFI (i386/x86_64/aarch64/riscv), use the UEFI serial port functions
// 2. If we are on x86_64 or i386, use the standard COM1/0x3f8 port if the UEFI implementation lacks support for SerialPortLib
// 3. If we are on any other architecture, there should be a way to get the MMIO address of the serial port (maybe from the config file?)

// global initialisation function, called from common/entry.s3.c
//static void serial_initialise(void) {
//    if (serial_initialised) {
//        return;
//    }
//
//#ifdef(UEFI)
//    serial_initialised = true;
//    return;
//#endif
//
//
//}

#if defined(UEFI)

EFI_SERIAL_IO_PROTOCOL *serial_io_protocol;

// get the serial io protocol
static void serial_initialise(void) {
    if (serial_initialised) {
        return;
    }

#if defined (__x86_64__) || defined (__i386__)
//    // Init com1
    //outb(0x3f8 + 3, 0x00);
    //outb(0x3f8 + 1, 0x00);
    //outb(0x3f8 + 3, 0x80);
    //outb(0x3f8 + 0, 0x0c); // 9600 baud
    //outb(0x3f8 + 1, 0x00);
    //outb(0x3f8 + 3, 0x03);
    //outb(0x3f8 + 2, 0xc7);
    //outb(0x3f8 + 4, 0x0b);

#endif


    EFI_STATUS status;
    EFI_GUID serial_io_protocol_guid = EFI_SERIAL_IO_PROTOCOL_GUID;

    status = gBS->LocateProtocol(&serial_io_protocol_guid, NULL, (void **)&serial_io_protocol);
    if (status) {
        //serial_initialised = true;
        panic(false, "Could not get serial io protocol");
    }

    status = serial_io_protocol->Reset(serial_io_protocol);
    if (status) {
        panic(false, "Could not reset serial io protocol");
    }

    serial_initialised = true;
}

void serial_out(uint8_t b) {
    serial_initialise();

#if defined (__x86_64__) || defined (__i386__)
    //while ((inb(0x3f8 + 5) & 0x20) == 0);
    //outb(0x3f8, b);
#endif


//    EFI_STATUS ret;

    uint8_t buffer[] = {b, 0};
    UINTN buffer_size = 1;
    EFI_STATUS ret = serial_io_protocol->Write(serial_io_protocol, &buffer_size, buffer);
    if (ret) {
        panic(false, "Could not write to serial port");
    }

    //const uint8_t *test = "WE ARE DOING IT\n";
    //buffer_size = 16;

    //ret = serial_io_protocol->Write(serial_io_protocol, &buffer_size, test);
    //if (ret) {
    //   // panic(false, "Could not write to serial port");
    //}

    //while(1);
}

int serial_in(void) {
    serial_initialise();
    
    //return 0;

    //uint8_t buffer[1];
    //UINTN buffer_size = 1;
    //EFI_STATUS ret = serial_io_protocol->Read(serial_io_protocol, &buffer_size, buffer);
    //if (ret) {
    //    panic(false, "Could not read from serial port");
    //}

    //return buffer[0];

    return 0;
}

#elif defined(BIOS)
static void serial_initialise(void) {
    if (serial_initialised) {
        return;
    }

    // Init com1
    outb(0x3f8 + 3, 0x00);
    outb(0x3f8 + 1, 0x00);
    outb(0x3f8 + 3, 0x80);
    outb(0x3f8 + 0, 0x0c); // 9600 baud
    outb(0x3f8 + 1, 0x00);
    outb(0x3f8 + 3, 0x03);
    outb(0x3f8 + 2, 0xc7);
    outb(0x3f8 + 4, 0x0b);

    serial_initialised = true;
}

void serial_out(uint8_t b) {
    serial_initialise();

    while ((inb(0x3f8 + 5) & 0x20) == 0);
    outb(0x3f8, b);
}

int serial_in(void) {
    serial_initialise();

    if ((inb(0x3f8 + 5) & 0x01) == 0) {
        return -1;
    }
    return inb(0x3f8);
}

#else


// TODO: Implement serial for other architectures
// Either we somehow get the MMIO address of the serial port
// or we use the UEFI serial port functions from UEFI, if the are available from the bootloader (EFK II probably, uboot???)

static void serial_initialise(void) {
    if (serial_initialised) {
        return;
    }

    // Init com1
   // outb(0x3f8 + 3, 0x00);
   // outb(0x3f8 + 1, 0x00);
   // outb(0x3f8 + 3, 0x80);
   // outb(0x3f8 + 0, 0x0c); // 9600 baud
   // outb(0x3f8 + 1, 0x00);
   // outb(0x3f8 + 3, 0x03);
   // outb(0x3f8 + 2, 0xc7);
   // outb(0x3f8 + 4, 0x0b);

   // serial_initialised = true;
}

void serial_out(uint8_t b) {
    serial_initialise();

    //while ((inb(0x3f8 + 5) & 0x20) == 0);
    //outb(0x3f8, b);
}

int serial_in(void) {
    serial_initialise();

    //if ((inb(0x3f8 + 5) & 0x01) == 0) {
    //    return -1;
   // }
    //return inb(0x3f8);
}

#endif
