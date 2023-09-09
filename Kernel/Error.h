#pragma once

#define SUCCESS                          0x00
#define INVALID_PARAMETER                1
#define MEMORY_ERROR                     2
#define PCI_ERROR                        3
#define QUEUE_ERROR                      4
#define STACK_ERROR                      5
#define TIMER_ERROR                      6
#define ACPI_ERROR                       7
#define ACPI_INVALID_RSDP                0x20
#define ACPI_INVALID_XSDT                0x20
#define ACPI_INVALID_FADT                0x21
#define MESSAGE_QUEUE_EMPTY              0x30
#define TIMER_QUEUE_EMPTY                0x40
#define PROGRAM_BREAK_ERROR              0x50

typedef signed __int8    byte;
typedef signed __int8    code;