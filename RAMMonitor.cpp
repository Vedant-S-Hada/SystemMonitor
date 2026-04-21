#include "RAMMonitor.h"
#include "AlertManager.h"
#include <iostream>
#include <iomanip>

RAMMonitor::RAMMonitor() : totalRAM_GB(0), usedRAM_GB(0), usagePercent(0) {}

void RAMMonitor::refresh() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    totalRAM_GB = memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
    double availRAM_GB = memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
    usedRAM_GB = totalRAM_GB - availRAM_GB;
    usagePercent = (usedRAM_GB / totalRAM_GB) * 100.0;

    if (usagePercent > 80.0) {
        char buf[128];
        snprintf(buf, sizeof(buf), "[WARNING] High RAM Usage detected: %.1f%%", usagePercent);
        AlertManager::getInstance().addAlert(buf);
    }
}

void RAMMonitor::display(int startRow, int startCol) {
    std::cout << "\033[" << startRow << ";" << startCol << "H";
    std::cout << "\033[1;35m RAM Usage \033[0m ";
    
    int barWidth = 20;
    int filled = (int)((usagePercent / 100.0) * barWidth);
    std::cout << "\033[1;33m"; 
    for(int i=0; i<barWidth; ++i) {
        if(i < filled) std::cout << "█";
        else std::cout << "\033[1;30m░\033[1;33m";
    }
    std::cout << "\033[0m ";
    
    std::cout << std::setw(5) << std::fixed << std::setprecision(1) << usagePercent << "%   ";
    std::cout << "\033[1;36m" << std::setprecision(1) << usedRAM_GB << "/" << totalRAM_GB << " GB\033[0m";
}

#include "vendor/imgui/imgui.h"
void RAMMonitor::renderGUI() {
    ImGui::Text("RAM Usage (%.1f / %.1f GB)", usedRAM_GB, totalRAM_GB);
    char buf[32];
    sprintf(buf, "%.1f%%", usagePercent);
    ImGui::ProgressBar(usagePercent / 100.0f, ImVec2(-1.0f, 0.0f), buf);
}
