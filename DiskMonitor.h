#pragma once
#include "Monitor.h"
#include <windows.h>

class DiskMonitor : public Monitor {
private:
    double totalSpace_GB;
    double freeSpace_GB;
    double usagePercent;

public:
    DiskMonitor();
    void refresh() override;
    void display(int startRow, int startCol) override;
    void renderGUI() override;
};
