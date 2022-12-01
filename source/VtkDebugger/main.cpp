#include "VtkDebugger.h"
#include "VtkEventProcess.h"
#include "VtkDebugger.h"
#include <Windows.h>
#include <process.h>

int main() {
	DebuggeeListener listener;
	VtkDebugger* debugger = new VtkDebugger();
	HANDLE sem = CreateSemaphore(NULL, 0, 1, TEXT("WAIT_SUBTHREAD"));
	listener.SetDebugger(debugger);
	listener.StartListen();
	WaitForSingleObject(sem, INFINITE);
	debugger->Start();
	return EXIT_SUCCESS;
}