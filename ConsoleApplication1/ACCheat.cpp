#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

// Assault Cube bullet hack.

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

DWORD PointerAndOffset(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
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

int main()
{
	DWORD pid;

	DWORD adres;

	int defaultBullet = 20;

	int protect = 100;

	int bulletNumber;

	int protectNumber;

	DWORD pointer = 0x0016F338;

	DWORD offset[3] = { 0x8 ,0x1C, 0xF0 };

	HWND hwnd = FindWindowA(0, ("AssaultCube"));

	if (!hwnd) cerr << "uygulama acik degil." << endl;

	GetWindowThreadProcessId(hwnd, &pid);

	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (!pHandle) cerr << "uygulamaya baglanilamadi" << endl;

	DWORD modul = GetMBA(pid, L"ac_client.exe");

	adres = PointerAndOffset(3, pHandle, offset, (modul + pointer));

	while (true)
	{
		WriteProcessMemory(pHandle, (LPVOID)adres, &protect, sizeof(int), 0);
		ReadProcessMemory(pHandle, (LPCVOID)adres, &protectNumber, sizeof(int), NULL);
		cout << protectNumber << endl;
		Sleep(500);
	}
}