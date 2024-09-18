#include "SystemConfiguration.h"

STATIC	SYSTEM_CONFIGURATION	systemConfiguration;

STATUS
DUMMYAPI SetSystemConfiguration(
	IN CONST	SYSTEM_CONFIGURATION*	_systemConfiguration
)   {
	systemConfiguration.graphicsInformation = _systemConfiguration->graphicsInformation;
	systemConfiguration.memoryInformation = _systemConfiguration->memoryInformation;
	systemConfiguration.ACPIInformation = _systemConfiguration->ACPIInformation;

	return SUCCESS;
}

SYSTEM_CONFIGURATION*
DUMMYAPI GetSystemConfiguration(
	VOID
)	{
	return &systemConfiguration;
}