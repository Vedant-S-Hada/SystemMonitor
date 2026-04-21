#pragma once
#include "Monitor.h"
#include <vector>
#include <memory>
#include <atomic>

class Dashboard {
private:
    std::vector<std::unique_ptr<Monitor>> monitors;
    std::atomic<bool> running;

    void setupConsole();
    void drawBorders();

public:
    Dashboard();
    ~Dashboard();
    void addMonitor(std::unique_ptr<Monitor> monitor);
    void start();
    void stop();
};
