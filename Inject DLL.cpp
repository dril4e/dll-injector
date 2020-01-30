#include "Windows.h"
#include "TlHelp32.h"
#include <iostream>
#include <string>

void InjectDLL(DWORD proccessId)
{
	const char* dllPath = "Full DLL Path goes here";
	HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, proccessId);
	if (hTargetProcess)
	{
		LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		LPVOID RemoteString = VirtualAllocEx(hTargetProcess, NULL, strlen(dllPath),	MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(hTargetProcess, RemoteString, dllPath, strlen(dllPath), NULL);
		CreateRemoteThread(hTargetProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, (LPVOID)RemoteString, NULL, NULL);
		Sleep(1000);
		exit(0);
	}
}

DWORD PID(LPCSTR proc)
{
	DWORD pid;
	HWND Program = FindWindowA(NULL, proc);
	while (Program == NULL)
	{
		Program = FindWindowA(NULL, proc);
		Sleep(2000);
		system("cls");
	}
	if(Program != NULL)
	{
		GetWindowThreadProcessId(Program, &pid);
	}
	return pid;
}

int main()
{
	InjectDLL(PID("Untitled - Notepad"));
}