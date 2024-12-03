#include <windows.h>
#include <iostream>
#include <vector>

#define MAX_EXECUTION_TIME 10000  

STARTUPINFO si;
PROCESS_INFORMATION pi;

void CreateAndManageProcess(const char* programName, const char* arguments) {
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    ZeroMemory(&pi, sizeof(pi));

    
    if (!CreateProcess(
        programName,           
        (LPSTR)arguments,      
        NULL,                  
        NULL,                 
        FALSE,                 
        0,                    
        NULL,                  
        NULL,                 
        &si,                   
        &pi                    
    )) {
        std::cerr << "Error creating process. Error code: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Process created. Process ID: " << pi.dwProcessId << std::endl;

  
    DWORD dwWaitResult = WaitForSingleObject(pi.hProcess, MAX_EXECUTION_TIME);
    if (dwWaitResult == WAIT_TIMEOUT) {
        std::cout << "Process exceeded maximum execution time, it will be terminated." << std::endl;
        TerminateProcess(pi.hProcess, 0);  
    } else {
        
        DWORD exitCode;
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            std::cout << "Process ended with exit code: " << exitCode << std::endl;
        } else {
            std::cerr << "Failed to retrieve the exit code of the process." << std::endl;
        }
    }

 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    
    std::vector<const char*> programs = { "notepad.exe", "calc.exe" };  
    std::vector<const char*> arguments = { "", "" }; 

    for (size_t i = 0; i < programs.size(); ++i) {
        CreateAndManageProcess(programs[i], arguments[i]);
    }

    return 0;
}

