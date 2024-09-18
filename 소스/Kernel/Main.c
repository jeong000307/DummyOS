#include "Main.h"

VOID
DUMMYAPI Main (
	IN CONST	SYSTEM_CONFIGURATION*	_systemConfiguration
)	{
	STATUS					status;

	MESSAGE					message;
	SYSTEM_CONFIGURATION*	systemConfiguration = GetSystemConfiguration();
	SCREEN*					screen = GetScreen();
	CONSOLE*				systemConsole = GetSystemConsole();
	//PCI_DEVICES*			PCIDevices = GetPCIDevices();
	MEMORY_MANAGER*			memoryManager = GetMemoryManager();
	//HEAP*					systemHeap = GetSystemHeap();
	TIMER_MANAGER*			timerManager = GetTimerManager();
	//TASK_MANAGER*			taskManager = GetTaskManager();
	MESSAGE_QUEUE*			messageQueue = GetMessageQueue();

	SetInterruptFlag(FALSE);

	SetSystemConfiguration(_systemConfiguration);
	
	if (InitializeSegmentation() != SUCCESS) {
		return;
	}

	if (InitializeMemoryManager(&systemConfiguration->memoryInformation) != SUCCESS) {
		return;
	}

	if (InitializeHeap(memoryManager) != SUCCESS) {
		return;
	}

	if (InitializeScreen(&systemConfiguration->graphicsInformation) != SUCCESS) {
		return;
	}

	if (InitializeSystemConsole(screen) != SUCCESS) {
		return;
	}

	systemConsole->Print(systemConsole, "DummyOS Booting...\n");

	if (ParseACPITable(systemConfiguration->ACPIInformation) != SUCCESS) {
		return;
	}

	InitializeTimerManager();

	systemConsole->Print(systemConsole, "Timer manager is initialized.\n");

	InitializeInterrupt();

	systemConsole->Print(systemConsole, "Interrupt manager is initialized.\n");

	//status = InitializeTaskManager();

	//systemConsole->Print(systemConsole, "Task manager is initialized.\n");

	status = InitializePCI();

	systemConsole->Print(systemConsole, "PCI devices are initialized.\n");

	timerManager->CreateTimer(timerManager, 10, taskTimerValue);

	for(;;)	{
		SetInterruptFlag(FALSE);

		if (messageQueue->count == 0) {
			SetInterruptFlag(TRUE);
			continue;
		}

		SetInterruptFlag(TRUE);

		WaitMilliSeconds(100);

		message = messageQueue->Pop(messageQueue);

		switch (message.type) {
			case TimerInterruptIndex:
				systemConsole->Print(systemConsole, "Timer: timeout = %u, value = %d\n", message.argument.timer.timeOut, message.argument.timer.value);

				if (message.argument.timer.value > 0) {
					timerManager->CreateTimer(timerManager, message.argument.timer.timeOut + 10, message.argument.timer.value + 1);
				}
				break;
		}
	}
}