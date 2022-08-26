#include <Windows.h>
#include <iostream>
using namespace std;

// Assault Cube bullet hack.

int main()
{
    DWORD pid;
    DWORD adress = 0x00739A70;
    int defaultBullet = 20;
    int bulletNumber;
    HWND hwnd = FindWindowA(0,("AssaultCube"));
    if (!hwnd) cerr << "game not found!" << endl;
    GetWindowThreadProcessId(hwnd,&pid);
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
    if (!pHandle) cerr << "game not connecting" << endl;
    while (true) {
        //WriteProcessMemory(pHandle, (LPVOID)adress, &defaultBullet, sizeof(int), 0); WRITING BULLET
        ReadProcessMemory(pHandle,(LPVOID)adress, &bulletNumber, sizeof(int),NULL); // READING BULLET AMOUNT
        cout << bulletNumber;
        Sleep(100);
    }
}