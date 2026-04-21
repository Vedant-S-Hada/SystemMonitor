#pragma once
#include "Monitor.h"
#include <windows.h>

class RAMMonitor : public Monitor {
private:
    double totalRAM_GB;
    double usedRAM_GB;
    double usagePercent;

public:
    RAMMonitor();
    void refresh() override;
    void display(int startRow, int startCol) override;
    void renderGUI() override;
};
