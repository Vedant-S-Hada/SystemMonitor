#include "Dashboard.h"
#include "CPUMonitor.h"
#include "RAMMonitor.h"
#include "DiskMonitor.h"
#include "ProcessMonitor.h"
#include <windows.h>
#include <iostream>

Dashboard* globalDashboard = nullptr;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
            if (globalDashboard) {
                globalDashboard->stop();
            }
            std::cout << "\n\033[16;1H\033[?25h\033[0mExiting Gracefully...\n";
            return TRUE;
        default:
            return FALSE;
    }
}

int main() {
    Dashboard dashboard;
    globalDashboard = &dashboard;

    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        std::cerr << "Could not set control handler\n";
        return 1;
    }

    dashboard.addMonitor(std::make_unique<CPUMonitor>());
    dashboard.addMonitor(std::make_unique<RAMMonitor>());
    dashboard.addMonitor(std::make_unique<DiskMonitor>());
    dashboard.addMonitor(std::make_unique<ProcessMonitor>());

    dashboard.start();

    return 0;
}
