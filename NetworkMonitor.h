#pragma once
#include "Monitor.h"
#include <vector>
#include <windows.h>
#include <iphlpapi.h>

class NetworkMonitor : public Monitor {
private:
    double uploadSpeedKB; // KB/s
    double downloadSpeedKB; // KB/s
    
    unsigned long long prevBytesSent;
    unsigned long long prevBytesReceived;
    DWORD prevTime;

    // For ImGui plotting
    std::vector<float> uploadHistory;
    std::vector<float> downloadHistory;

public:
    NetworkMonitor();
    void refresh() override;
    void display(int startRow, int startCol) override;
    void renderGUI() override;
};
