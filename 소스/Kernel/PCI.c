#include "PCI.h"
#include "Timer.h"

STATIC	PCI_DEVICES	PCIDevices;

PCI_DEVICES*
DUMMYAPI GetPCIDevices(
	VOID
)   {
	return &PCIDevices;
}

STATUS
DUMMYAPI InitializePCI(
	VOID
)   {
	STATUS  status;
	UINT8   function;
	UINT8   headerType = ReadPCIHeaderType(0, 0, 0);

	PCIDevices.ScanPCIBus = _ScanPCIBus;
	PCIDevices.ScanPCIDevice = _ScanPCIDevice;
	PCIDevices.ScanPCIFunction = _ScanPCIFunction;
	PCIDevices.AddPCIDevice = _AddPCIDevice;

	PCIDevices.count = 0;

	if (HasSingleFunctionPCIDevice(headerType)) {
		return PCIDevices.ScanPCIBus(&PCIDevices, 0);
	}

	for (function = 1; function < PCI_FUNCTION_MAX; ++function) {
		if (ReadPCIVendorID(0, 0, function) == 0xffffu) {
			continue;
		}

		status = PCIDevices.ScanPCIBus(&PCIDevices, function);

		if (status == ERR_DEVICE_ERROR) {
			return ERR_DEVICE_ERROR;
		}
	}

	return SUCCESS;
}

STATIC BOOLEAN
DUMMYAPI HasSingleFunctionPCIDevice(
	IN CONST	UINT8	_headerType
)   {
	return (_headerType & 0x80u) == 0;
}

STATIC STATUS
DUMMYAPI _ScanPCIBus(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus
)   {
	STATUS	status;
	UINT8	device;

	for (device = 0; device < PCI_DEVICE_MAX; ++device) {
		if (ReadPCIVendorID(_bus, device, 0) == 0xFFFFu) {
			continue;
		}

		status = _this->ScanPCIDevice(_this, _bus, device);

		if (status == ERR_DEVICE_ERROR) {
			return status;
		}
	}

	return SUCCESS;
}

STATIC STATUS
DUMMYAPI _ScanPCIDevice(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus,
	IN CONST	UINT8			_device
)   {
	STATUS  status;
	UINT8   function;

	status = _this->ScanPCIFunction(_this, _bus, _device, 0);

	if (status == ERR_DEVICE_ERROR) {
		return ERR_DEVICE_ERROR;
	}

	if (HasSingleFunctionPCIDevice(ReadPCIHeaderType(_bus, _device, 0))) {
		return SUCCESS;
	}

	for (function = 1; function < PCI_FUNCTION_MAX; ++function) {
		if (ReadPCIVendorID(_bus, _device, function) == 0xFFFFu) {
			continue;
		}

		status = _this->ScanPCIFunction(_this, _bus, _device, function);

		if (status == ERR_DEVICE_ERROR) {
			return ERR_DEVICE_ERROR;
		}
	}

	return SUCCESS;
}

STATIC STATUS
DUMMYAPI _ScanPCIFunction(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus,
	IN CONST	UINT8			_device,
	IN CONST	UINT8			_function
)   {
	STATUS  status;
	UINT8   headerType = ReadPCIHeaderType(_bus, _device, _function);
	UINT8   baseClassCode;
	UINT8   subClassCode;
	UINT8   secondaryBusNumber;
	UINT32  classCode;
	UINT32  busNumber;

	status = _this->AddPCIDevice(_this, _bus, _device, _function, headerType);

	if (status == ERR_DEVICE_ERROR) {
		return ERR_DEVICE_ERROR;
	}

	classCode = ReadPCIClassCode(_bus, _device, _function);
	baseClassCode = (classCode >> 24) & 0xffu;
	subClassCode = (classCode >> 16) & 0xffu;

	if (baseClassCode == 0x06u AND subClassCode == 0x04u) {
		busNumber = ReadPCIBusNumber(_bus, _device, _function);
		secondaryBusNumber = (busNumber >> 8) & 0xffu;

		return _this->ScanPCIBus(_this, secondaryBusNumber);
	}

	return SUCCESS;
}

STATIC STATUS
DUMMYAPI _AddPCIDevice(
	OUT			PCI_DEVICES*	_this,
	IN CONST	UINT8			_bus,
	IN CONST	UINT8			_device,
	IN CONST	UINT8			_function,
	IN CONST	UINT8			_headerType
)	{
	if (_this->count == PCI_DEVICE_MAX) {
		return ERR_DEVICE_ERROR;
	}

	_this->devices[_this->count].bus = _bus;
	_this->devices[_this->count].device = _device;
	_this->devices[_this->count].function = _function;
	_this->devices[_this->count].headerType = _headerType;

	++_this->count;

	return SUCCESS;
}

UINT16
DUMMYAPI ReadPCIVendorID(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
)	{
	WritePCIAddress(MakePCIAddress(_bus, _device, _function, 0x00));

	return ReadPCIData() & 0xFFFFu;
}

UINT16
DUMMYAPI ReadPCIDeviceID(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
)	{
	WritePCIAddress(MakePCIAddress(_bus, _device, _function, 0x00));

	return ReadPCIData() >> 16;
}

UINT32
DUMMYAPI ReadPCIClassCode(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
)	{
	WritePCIAddress(MakePCIAddress(_bus, _device, _function, 0x08));

	return ReadPCIData();
}

UINT8
DUMMYAPI ReadPCIHeaderType(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
)	{
	WritePCIAddress(MakePCIAddress(_bus, _device, _function, 0x0C));

	return (ReadPCIData() >> 16) & 0xffu;
}

UINT32
DUMMYAPI ReadPCIBusNumber(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function
)	{
	WritePCIAddress(MakePCIAddress(_bus, _device, _function, 0x18));

	return ReadPCIData();
}

STATIC UINT32
DUMMYAPI MakePCIAddress(
	IN CONST	UINT8	_bus,
	IN CONST	UINT8	_device,
	IN CONST	UINT8	_function,
	IN CONST	UINT8	_registerAddress
)	{
	return (1 << 31) | (_bus << 16) | (_device << 11) | (_function << 8) | (_registerAddress & 0xFCu);
}

STATIC VOID
DUMMYAPI WritePCIAddress(
	IN CONST	UINT32	_address
)	{
	IOOut32(PCI_CONFIG_ADDRESS, _address);
}

STATIC VOID
DUMMYAPI WritePCIData(
	IN CONST	UINT32	_value
)	{
	IOOut32(PCI_CONFIG_DATA, _value);
}

STATIC UINT32
DUMMYAPI ReadPCIData(
	VOID
)	{
	return IOIn32(PCI_CONFIG_DATA);
}