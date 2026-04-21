#pragma once
#include "Monitor.h"
#include <windows.h>

class CPUMonitor : public Monitor {
private:
    FILETIME prevIdleTime, prevKernelTime, prevUserTime;
    double currentUsage;
    int numCores;

    double calculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
    unsigned long long fileTimeToInt64(const FILETIME& ft);

public:
    CPUMonitor();
    void refresh() override;
    void display(int startRow, int startCol) override;
    void renderGUI() override;
};
