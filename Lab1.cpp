#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>

void CreateAndManageProcess(LPCSTR applicationName, int timeoutMilliseconds) {
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pi = {};
    DWORD exitCode = 0;

    // Створення процесу
    if (!CreateProcessA(
        applicationName,         // Ім'я програми
        NULL,                    // Аргументи командного рядка
        NULL,                    // Безпека процесу
        NULL,                    // Безпека потоку
        FALSE,                   // Успадкування дескрипторів
        0,                       // Опції створення
        NULL,                    // Середовище процесу
        NULL,                    // Поточний каталог
        &si,                     // Структура STARTUPINFO
        &pi                      // Структура PROCESS_INFORMATION
    )) {
        std::cerr << "Не вдалося створити процес. Код помилки: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Процес запущений з PID: " << pi.dwProcessId << std::endl;

    // Чекати завершення процесу
    DWORD waitResult = WaitForSingleObject(pi.hProcess, timeoutMilliseconds);

    if (waitResult == WAIT_TIMEOUT) {
        std::cout << "Процес перевищив ліміт часу. Завершення процесу..." << std::endl;
        if (!TerminateProcess(pi.hProcess, 1)) {
            std::cerr << "Не вдалося завершити процес. Код помилки: " << GetLastError() << std::endl;
        }
    }
    else if (waitResult == WAIT_OBJECT_0) {
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            std::cout << "Процес завершився. Код завершення: " << exitCode << std::endl;
        }
        else {
            std::cerr << "Не вдалося отримати код завершення процесу. Код помилки: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cerr << "Помилка очікування процесу. Код помилки: " << GetLastError() << std::endl;
    }

    // Закриття дескрипторів
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const int TIMEOUT_MS = 10000; // 10 секунд

    // Приклад створення одного процесу
    CreateAndManageProcess("notepad.exe", TIMEOUT_MS);

    // Реалізація управління кількома процесами
    std::vector<LPCSTR> programs = { "notepad.exe", "calc.exe" };
    for (const auto& program : programs) {
        CreateAndManageProcess(program, TIMEOUT_MS);
    }

    return 0;
}

