#ifndef __PCI_H__
#define __PCI_H__

#include "Type.h"
#include "Console.h"
#include "Error.h"

#define PCI_CONFIG_ADDRESS 0x0CF8
#define PCI_CONFIG_DATA    0x0CFC
#define PCI_DEVICE_MAX     32
#define PCI_FUNCTION_MAX   8

typedef struct _PCI_DEVICES PCI_DEVICES;

typedef struct {
	UINT8 bus;
	UINT8 device;
	UINT8 function;
	UINT8 headerType;
} PCI_DEVICE;

struct _PCI_DEVICES {
	PCI_DEVICE  devices[PCI_DEVICE_MAX];
	UINTN       count;

	STATUS      (DUMMYAPI* ScanPCIBus)(
					OUT			PCI_DEVICES*	_this,
					IN CONST	UINT8			_bus
				);
	STATUS		(DUMMYAPI* ScanPCIDevice)(
					OUT			PCI_DEVICES*	_this,
					IN CONST	UINT8			_bus,
					IN CONST	UINT8			_device
				);
	STATUS		(DUMMYAPI* ScanPCIFunction)(
					OUT			PCI_DEVICES*	_this,
					IN CONST	UINT8			_bus,
					IN CONST	UINT8			_device,
					IN CONST	UINT8			_function
				);
	STATUS		(DUMMYAPI* AddPCIDevice)(
					OUT			PCI_DEVICES*	_this,
					IN CONST	UINT8			_bus,
					IN CONST	UINT8			_device,
					IN CONST	UINT8			_function,
					IN CONST	UINT8			_headerType
				);
};

#pragma pack(push, 1)
typedef union {
	UINT32			data;
	struct {
		unsigned	capabilityID: 8;
		unsigned	nextPointer: 8;
		unsigned	capability: 16;
	}				bits;
} CAPABILITY_HEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	union {
		UINT32			data;
		struct {
			unsigned	capabilityID: 8;
			unsigned	nextPointer: 8;
			unsigned	MSIEnable: 1;
			unsigned	multiMessageCapable: 3;
			unsigned	multiMessageEnable: 3;
			unsigned	Address64Capable: 1;
			unsigned	perVectorMaskCapable: 1;
			unsigned	: 7;
		}				bits;
	}					header;

	UINT32				messageAddress;
	UINT32				messageUpperAddress;
	UINT32				messageData;
	UINT32				maskBits;
	UINT32				pendingBits;
} MSI_CAPABILITY;
#pragma pack(pop)

typedef enum {
	edge = 0,
	level = 1
} MSI_TRIGGER_MODE;

typedef enum {
	Fixed = 0b000,
	LowestPriority = 0b001,
	SMI = 0b010,
	NMI = 0b100,
	INIT = 0b101,
	ExtINT = 0b111
} MSI_DELIVERY_MODE;

STATUS 
DUMMYAPI InitializePCI(
	VOID
);

PCI_DEVICES* 
DUMMYAPI GetPCIDevices(
	VOID
);

STATIC BOOLEAN 
DUMMYAPI HasSingleFunctionPCIDevice(
	IN CONST	UINT8	_headerType
);

UINT16 
DUMMYAPI ReadPCIVendorID(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
);

UINT16 
DUMMYAPI ReadPCIDeviceID(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
);

UINT32 
DUMMYAPI ReadPCIClassCode(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
);

UINT8 
DUMMYAPI ReadPCIHeaderType(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
);

UINT32 
DUMMYAPI ReadPCIBusNumber(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
);

STATIC UINT32
DUMMYAPI MakePCIAddress(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function,
	IN CONST	UINT8	_registerAddress
);

STATIC VOID
DUMMYAPI WritePCIAddress(
	IN CONST	UINT32	_address
);

STATIC VOID
DUMMYAPI WritePCIData(
	IN CONST	UINT32	_value
);

STATIC UINT32 
DUMMYAPI ReadPCIData(
	VOID
);

STATIC STATUS
DUMMYAPI _ScanPCIBus(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus
);

STATIC STATUS
DUMMYAPI _ScanPCIDevice(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus,
	IN CONST	UINT8			_device
);

STATIC STATUS
DUMMYAPI _ScanPCIFunction(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus,
	IN CONST	UINT8			_device,
	IN CONST	UINT8			_function
);

STATIC STATUS
DUMMYAPI _AddPCIDevice(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus,
	IN CONST	UINT8			_device,
	IN CONST	UINT8			_function,
	IN CONST	UINT8			_headerType
);

#endif