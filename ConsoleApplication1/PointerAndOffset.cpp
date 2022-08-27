#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

DWORD GetMBA(DWORD procId, const wchar_t* modName)
{
	DWORD modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{

			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (DWORD)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}

	CloseHandle(hSnap);
	return modBaseAddr;
}

DWORD pointerveoffset(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{

	DWORD pointer = BaseAddress;
	DWORD pTemp;
	DWORD pointerAddr;

	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, 4, NULL);
		}

		pointerAddr = pTemp + Offsets[i];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, 4, NULL);
	}
	return pointerAddr;
}