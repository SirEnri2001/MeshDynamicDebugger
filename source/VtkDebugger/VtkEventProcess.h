#pragma once

#include <vtkNew.h>
#include <vtkMultiThreader.h>
#include <Windows.h>
#include <process.h>
#include <string>
#include "json.hpp"

#include "VtkDebugger.h"

#define BUF_SIZE 10485760

using json = nlohmann::json;

void subThreadLoop(LPVOID lpParamter);

class DebuggeeListener
{
public:
	DebuggeeListener()
	{
		InitSemSharedMem();
		debugger = nullptr;
	}

	~DebuggeeListener()
	{
	}

	void StartListen() {
		cout << "INFO: worker thread created" << endl;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)subThreadLoop, (LPVOID)this, 0, NULL);
	}

	void SetDebugger(VtkDebugger* debugger);

	void InitSemSharedMem() 
	{
		HANDLE read_sem = OpenSemaphore(
			SEMAPHORE_ALL_ACCESS,          // 访问标志
			TRUE,                          // 继承标志
			L"READ_MESH_DATA");                   // 信号量名
		HANDLE write_sem = OpenSemaphore(
			SEMAPHORE_ALL_ACCESS,          // 访问标志
			TRUE,                          // 继承标志
			L"WRITE_MESH_DATA");                   // 信号量名
		HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, L"ShareMemory");
		

		if (read_sem) {
			CloseHandle(read_sem);
		}
		if (write_sem) {
			CloseHandle(write_sem);
		}
		if (hMapFile) {
			CloseHandle(hMapFile);
		}
		
		readSem = CreateSemaphore(NULL, 0, 1, TEXT("READ_MESH_DATA"));
		writeSem = CreateSemaphore(NULL, 1, 1, TEXT("WRITE_MESH_DATA"));
		shareMem = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			BUF_SIZE,
			L"ShareMemory"
		);
		if (!readSem || !writeSem || !shareMem)
		{
			cout << "Create Semaphore or Share Memory Failed!" << endl;
		}
		else {
			cout << "INFO: shared memory object created" << endl;

		}
	}
	

	VtkDebugger* debugger;

	HANDLE readSem, writeSem, shareMem;
};
