#include <Windows.h>
#include <vector>
#include <cstring>
#include <iostream>

uintptr_t FindRenderView(HANDLE hProcess) {
    static uintptr_t cachedRenderView = 0;
    if (cachedRenderView != 0) {
        return cachedRenderView;
    }

    const char* pattern = "RenderJob(EarlyRendering;";
    const size_t patternLen = strlen(pattern);

    uintptr_t currentAddress = 0;
    MEMORY_BASIC_INFORMATION mbi;

    while (true) {
        if (!VirtualQueryEx(hProcess, (LPCVOID)currentAddress, &mbi, sizeof(mbi))) {
            break;
        }

        if (mbi.State == MEM_COMMIT && (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
            std::vector<char> buffer(mbi.RegionSize);
            SIZE_T bytesRead;
            if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                for (size_t i = 0; i < bytesRead - patternLen; ++i) {
                    if (memcmp(buffer.data() + i, pattern, patternLen) == 0) {
                        uintptr_t foundAddress = (uintptr_t)mbi.BaseAddress + i;
                        cachedRenderView = foundAddress + 0x1E8;
                        return cachedRenderView;
                    }
                }
            }
        }

        currentAddress = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
        if (currentAddress < (uintptr_t)mbi.BaseAddress) { // Checks for overflow
            break;
        }
    }

    return 0; // Not found
}

int main() {
    DWORD pid = 9920; // roblox process id
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return 1;
    }

    uintptr_t renderView = FindRenderView(hProcess);
    if (renderView) {
        std::cout << "RenderView found: 0x" << std::hex << renderView << std::endl;
    }
    else {
        std::cerr << "RenderView not found." << std::endl;
    }

    CloseHandle(hProcess);
    return 0;
}
