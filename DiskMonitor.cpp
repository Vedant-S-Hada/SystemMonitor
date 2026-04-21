#include "DiskMonitor.h"
#include <iostream>
#include <iomanip>

DiskMonitor::DiskMonitor() : totalSpace_GB(0), freeSpace_GB(0), usagePercent(0) {}

void DiskMonitor::refresh() {
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    if (GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        totalSpace_GB = totalNumberOfBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
        freeSpace_GB = totalNumberOfFreeBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
        double usedSpace_GB = totalSpace_GB - freeSpace_GB;
        usagePercent = (usedSpace_GB / totalSpace_GB) * 100.0;
    }
}

void DiskMonitor::display(int startRow, int startCol) {
    std::cout << "\033[" << startRow << ";" << startCol << "H";
    std::cout << "\033[1;34m C:\\ Drive \033[0m ";
    
    int barWidth = 20;
    int filled = (int)((usagePercent / 100.0) * barWidth);
    std::cout << "\033[1;31m"; // Red
    for(int i=0; i<barWidth; ++i) {
        if(i < filled) std::cout << "█";
        else std::cout << "\033[1;30m░\033[1;31m";
    }
    std::cout << "\033[0m ";
    
    std::cout << std::setw(5) << std::fixed << std::setprecision(1) << usagePercent << "%   ";
    std::cout << "\033[1;35m" << std::setprecision(1) << (totalSpace_GB - freeSpace_GB) << "/" << totalSpace_GB << " GB\033[0m";
}

#include "vendor/imgui/imgui.h"
void DiskMonitor::renderGUI() {
    ImGui::Text("C:\\ Drive (%.1f / %.1f GB)", (totalSpace_GB - freeSpace_GB), totalSpace_GB);
    char buf[32];
    sprintf(buf, "%.1f%%", usagePercent);
    ImGui::ProgressBar(usagePercent / 100.0f, ImVec2(-1.0f, 0.0f), buf);
}
