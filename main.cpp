#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

const char* procName = "t6zm.exe"; // zm process name
const unsigned short pollRate = 100; // poll rate for sleeping
const unsigned int pointAddr = 0x0234C068; // user points address
const unsigned int healthAddr = 0x021C5868; // user health address
const unsigned int clipAddr = 0x02346ECC; // magazine one address
const unsigned int clip2Addr = 0x02346ED4; // mag two address (mustang and sally, dual wields)
const unsigned int pointsHacked = 1000000;
const unsigned short preHealthHacked = 0;
const unsigned long healthHacked = 1000000;
const unsigned short clipOneHacked = 10000;
const unsigned short clipTwoHacked = 10000;


using namespace std;

DWORD get_process_id_by_process_name(const char* process_name_) // process ID function
{
    PROCESSENTRY32 process_entry = { sizeof(PROCESSENTRY32) };
    HANDLE processes_snapshot;
    DWORD process_id = 0;

    // search for the process name
    processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(processes_snapshot, &process_entry))
    {
        do
        {
            if (!strcmp(process_entry.szExeFile, process_name_))
            {
                process_id = process_entry.th32ProcessID;
                break;
            }
        } while (Process32Next(processes_snapshot, &process_entry));
    }

    CloseHandle(processes_snapshot);

    return process_id;
}

int main(int argc_, char** argv_)
{
    // get game process id
    cout << "Looking for game.. (Is it launched?)" << endl;
    DWORD process_id = NULL;
    while (!process_id)
    {
        process_id = get_process_id_by_process_name(procName);
        Sleep(pollRate);
    }
    cout << "Game found." << endl;

    // open game handle
    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (process_handle)
    {
        cout << "Handle obtained to game. (Is it launched?)" << endl;
    }
    else
    {
        cout << "Handle not obtained to game." << endl;
        exit(1);
    }

    bool lockPoints = false;
    bool lockClip1 = false;
    bool lockClip2 = false;
    bool lockHealth = false;
    bool zombiesIgnorePlayer = false;
    bool update_screen = true;

    while (true)
    {
        if (update_screen)
        {
            system("cls");
            cout << "Call of Duty Black Ops 2 Zombies Memory Cheat" << endl;
            cout;
            cout << " F1 [" << (lockPoints ? "on" : "off") << "] - Set points to above 100k" << endl;
            cout << " F2 [" << (lockClip1 ? "on" : "off") << "] - Set clip 1 to 10k" << endl;
            cout << " F3 [" << (lockClip2 ? "on" : "off") << "] - Set clip 2 to 10k" << endl;
            cout << " F4 [" << (lockHealth ? "on" : "off") << "] - Set health to 10k" << endl;
            cout << " F5 [" << (zombiesIgnorePlayer ? "on" : "off") << "] - Zombies ignore player" << endl;
            update_screen = false;
        }

        if (GetAsyncKeyState(VK_F1))
        {
            lockPoints = !lockPoints;
            update_screen = true;
        }
        if (GetAsyncKeyState(VK_F2))
        {
            lockClip1 = !lockClip1;
            update_screen = true;
        }
        if (GetAsyncKeyState(VK_F3))
        {
            lockClip2 = !lockClip2;
            update_screen = true;
        }
        if (GetAsyncKeyState(VK_F4))
        {
            lockHealth = !lockHealth;
            update_screen = true;
        } if (GetAsyncKeyState(VK_F5)) {
            zombiesIgnorePlayer = !zombiesIgnorePlayer;
            update_screen = true;
        }

        if (lockHealth)
        {
            unsigned int preHacked = preHealthHacked;
            unsigned int postHacked = healthHacked;

            WriteProcessMemory(process_handle, (LPVOID)healthAddr, &preHacked, sizeof(preHacked), NULL);
            WriteProcessMemory(process_handle, (LPVOID)healthAddr, &postHacked, sizeof(postHacked), NULL);
        }
        if (lockPoints)
        {
            unsigned int postPointsHacked = pointsHacked;
            WriteProcessMemory(process_handle, (LPVOID)pointAddr, &postPointsHacked, sizeof(postPointsHacked), NULL);
        }
        if (lockClip1)
        {
            unsigned int postClipHacked = clipOneHacked;
            WriteProcessMemory(process_handle, (LPVOID)clipAddr, &clipOneHacked, sizeof(clipOneHacked), NULL);
        }
        if (lockClip2)
        {
            unsigned int postClip2Hacked = clipTwoHacked;
            WriteProcessMemory(process_handle, (LPVOID)clip2Addr, &postClip2Hacked, sizeof(postClip2Hacked), NULL);
        }
        if (zombiesIgnorePlayer) {
            unsigned int ignorePlayer = 0;
            lockHealth = false;
            WriteProcessMemory(process_handle, (LPVOID)healthAddr, &ignorePlayer, sizeof(ignorePlayer), NULL);
        }
        // sleep to check x seconds
        Sleep(pollRate);
    }
}