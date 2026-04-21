#pragma once
#include "Monitor.h"
#include <vector>
#include <string>
#include <windows.h>

struct ProcessInfo {
    unsigned long pid;
    std::string name;
    SIZE_T memUsage;
};

class ProcessMonitor : public Monitor {
private:
    std::vector<ProcessInfo> topProcesses;
    int selectedProcessIndex = -1;
public:
    ProcessMonitor();
    void refresh() override;
    void display(int startRow, int startCol) override;
    void renderGUI() override;
};
