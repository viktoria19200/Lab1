#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>

void CreateAndManageProcess(LPCSTR applicationName, int timeoutMilliseconds) {
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = {};
    DWORD exitCode = 0;

   
    if (!CreateProcessA(
        applicationName,         
        NULL,                    
        NULL,                    
        NULL,                   
        FALSE,                   
        0,                      
        NULL,                    
        NULL,                    
        &si,                    
        &pi                     
    )) {
        std::cerr << "Failed to create process. Error code: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Process started with PID:" << pi.dwProcessId << std::endl;

   
    DWORD waitResult = WaitForSingleObject(pi.hProcess, timeoutMilliseconds);

    if (waitResult == WAIT_TIMEOUT) {
        std::cout << "The process has timed out. Ending the process..." << std::endl;
        if (!TerminateProcess(pi.hProcess, 1)) {
            std::cerr << "The process could not be completed. Error code: " << GetLastError() << std::endl;
        }
    }
    else if (waitResult == WAIT_OBJECT_0) {
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            std::cout << "The process has ended. Completion code: " << exitCode << std::endl;
        }
        else {
            std::cerr << "Could not get process exit code. Error code: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cerr << "Error waiting for process. Error code: " << GetLastError() << std::endl;
    }

    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const int TIMEOUT_MS = 10000; 

    
    CreateAndManageProcess("notepad.exe", TIMEOUT_MS);

    /
    std::vector<LPCSTR> programs = { "notepad.exe", "calc.exe" };
    for (const auto& program : programs) {
        CreateAndManageProcess(program, TIMEOUT_MS);
    }

    return 0;
}

