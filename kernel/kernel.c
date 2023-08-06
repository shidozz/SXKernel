#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "./include/serial.h"
#include "./include/kio.h"
#include "./include/convert.h"
#include "./include/graph.h"
#include "./include/idt.h"
#include "./include/gdt.h"
#include "./include/fonts.h"
#include "./include/term.h"
#include "./include/heap.h"

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.
// Image BMP 4x4 pixels (format RGB, 0x24, bits par pixel)


// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
struct limine_framebuffer *framebuffer;
struct limine_rsdp *rsdp;
struct limine_memmap_entry *memmap;

void _start(void) {
    
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    framebuffer = framebuffer_request.response->framebuffers[0];    

    // Ensure we got a framebuffer.
    if (bootloader_info_request.response == NULL
     || bootloader_info_request.response->version == NULL || bootloader_info_request.response->name == NULL) {
        hcf();
    }
    // Ensure we got a framebuffer.
    if (memmap_request.response == NULL
     || bootloader_info_request.response->version == NULL || bootloader_info_request.response->name == NULL) {
        hcf();
    }
    memmap = memmap_request.response->entries[0];
    // Passer memmap->base et (memmap->base + memmap->length) directement à kheap_init
    if (kheap_init((void *)(memmap->base), (void *)(memmap->base + memmap->length)) != 0) {
        serial_printf("[+]: KERNEL HEAP Initialized!\n");
    }
    gen_fonts();

    change_terminal_fonts(1);
    terminal_printf("Bootloader: ");

    change_terminal_fg(RGB(255 / 2, 255 / 2, 255));
    terminal_printf("%s\n", bootloader_info_request.response->name);

    change_terminal_fg(RGB(255, 255, 255));
    terminal_printf("Version:");

    change_terminal_fg(RGB(255 / 2, 255 / 2, 255));
    terminal_printf(" v%s\n", bootloader_info_request.response->version);

    change_terminal_fg(RGB(255, 255, 255));
    change_terminal_fonts(3);

    if(gdt_install() == 0){
        serial_printf("[+]: GDT Initialized!\n");
        
        change_terminal_fg(RGB(0, 255, 0));
        terminal_printf("[+]: GDT Initialized!\n");
        change_terminal_fg(RGB(255, 255, 255));
    }
    __asm__ __volatile__("cli");
    if(idt_install() == 0){
        serial_printf("[+]: IDT Initialized!\n");
        change_terminal_fg(RGB(0, 255, 0));
        terminal_printf("[+]: IDT Initialized!\n");
        change_terminal_fg(RGB(255, 255, 255));
    }
    
    if(keyboard_install() == 0){
        serial_printf("[+]: KEYBOARD Initialized!\n");
        change_terminal_fg(RGB(0, 255, 0));
        terminal_printf("[+]: KEYBOARD Initialized!\n");
        change_terminal_fg(RGB(255, 255, 255));
    }

    __asm__  ("div %0" :: "r"(0));asm__ ("int $32");
    hcf();
}
