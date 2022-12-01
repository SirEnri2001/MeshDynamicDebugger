#include "VtkEventProcess.h"


void DebuggeeListener::SetDebugger(VtkDebugger* debugger) {
	this->debugger = debugger;
}

void inTryBlock(DebuggeeListener* listener, VtkDebugger* debugger, LPVOID lpBase) {
	
	while (true) {
		cout << "WAITING for debuggee" << endl;
		WaitForSingleObject(listener->readSem, INFINITE);
		cout << "ORDER received" << endl;
		json j = json::parse((char*)lpBase);

		/*
		* json format:
		* {"vertices":[[1,0,0],[0,0,1], ... ],
		*  "edges":[
		*       [[1,0,0],[0,1,0]], [[2,0,0],[0,2,0]], ...
		*  ]
		*  "mesh_ply_base64":"base64 encoded code"
		  }
		*/
		// Iterate vertices and add to debugger
		if (j["vertices"] != nullptr) {
			for (auto& vert : j["vertices"]) {
				double v[3] = { vert[0],vert[1],vert[2] };
				debugger->HighlightVertex(v);
			}
		}

		// Iterate edges and add to debugger
		if (j["edges"]!=nullptr) {
			for (auto it = j["edges"].begin(); it != j["edges"].end(); ++it) {
				debugger->HighlightEdge(
					new double[3] {(*it)[0][0], (*it)[0][1], (*it)[0][2]},
					new double[3] {(*it)[1][0], (*it)[1][1], (*it)[1][2]}
				);
			}
		}
		
		if (j["mesh_ply"] != nullptr) {
			debugger->SetModel(j["mesh_ply"]);
		}
		debugger->Update();
		ReleaseSemaphore(listener->writeSem, 1, NULL);
	}
}

void subThreadLoop(LPVOID lpParamter)
{
	DebuggeeListener* listener = (DebuggeeListener*)lpParamter;
	HANDLE sem = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,          // 访问标志
		TRUE,                          // 继承标志
		L"WAIT_SUBTHREAD");                   // 信号量名
	//while (threadInfo->UserData == nullptr) {
	//	std::this_thread::yield();
	//}
	if (sem) {
		ReleaseSemaphore(sem, 1, NULL);
	}
	//DebuggeeListener* listener = (DebuggeeListener*)threadInfo->UserData;

	VtkDebugger* debugger = listener->debugger;

	LPVOID lpBase = MapViewOfFile(listener->shareMem, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (lpBase == NULL) {
		cout << "FATAL: map view failed"<<lpBase << endl;
	}
	else {
		cout << "INFO: debugger initialized" << endl;
		inTryBlock(listener, debugger, lpBase);
	}
	if (lpBase) {
		UnmapViewOfFile(lpBase);
	}
	CloseHandle(listener->shareMem);
}