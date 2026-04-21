#include "CPUMonitor.h"
#include "AlertManager.h"
#include <iostream>
#include <string>
#include <iomanip>

CPUMonitor::CPUMonitor() : currentUsage(0.0) {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numCores = sysInfo.dwNumberOfProcessors;
    GetSystemTimes(&prevIdleTime, &prevKernelTime, &prevUserTime);
}

unsigned long long CPUMonitor::fileTimeToInt64(const FILETIME& ft) {
    return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

double CPUMonitor::calculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks) {
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;

    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime  = idleTicks - _previousIdleTicks;

    double ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((double)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks  = idleTicks;
    return ret;
}

void CPUMonitor::refresh() {
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        unsigned long long idle = fileTimeToInt64(idleTime);
        unsigned long long kernel = fileTimeToInt64(kernelTime);
        unsigned long long user = fileTimeToInt64(userTime);
        currentUsage = calculateCPULoad(idle, kernel + user) * 100.0;
        
        if (currentUsage > 80.0) {
            char buf[128];
            snprintf(buf, sizeof(buf), "[WARNING] High CPU Usage detected: %.1f%%", currentUsage);
            AlertManager::getInstance().addAlert(buf);
        }
    }
}

void CPUMonitor::display(int startRow, int startCol) {
    std::cout << "\033[" << startRow << ";" << startCol << "H";
    std::cout << "\033[1;36m CPU Usage \033[0m "; 
    
    int barWidth = 20;
    int filled = (int)((currentUsage / 100.0) * barWidth);
    std::cout << "\033[1;32m"; 
    for(int i=0; i<barWidth; ++i) {
        if(i < filled) std::cout << "█";
        else std::cout << "\033[1;30m░\033[1;32m"; 
    }
    std::cout << "\033[0m ";
    
    std::cout << std::setw(5) << std::fixed << std::setprecision(1) << currentUsage << "%   ";
    std::cout << "\033[1;33m" << numCores << " cores\033[0m";
}

#include "vendor/imgui/imgui.h"
void CPUMonitor::renderGUI() {
    ImGui::Text("CPU Usage (%d cores)", numCores);
    char buf[32];
    sprintf(buf, "%.1f%%", currentUsage);
    ImGui::ProgressBar(currentUsage / 100.0f, ImVec2(-1.0f, 0.0f), buf);
}
