#include "Dashboard.h"
#include <windows.h>
#include <iostream>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


Dashboard::Dashboard() : running(false) {
    setupConsole();
}

Dashboard::~Dashboard() {
    std::cout << "\033[?25h"; // Show cursor again
}

void Dashboard::setupConsole() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    // Set UTF8 for unicode characters (e.g. for blocks)
    SetConsoleOutputCP(CP_UTF8);

    // Hide cursor
    std::cout << "\033[?25l";
}

void Dashboard::addMonitor(std::unique_ptr<Monitor> monitor) {
    monitors.push_back(std::move(monitor));
}

void Dashboard::drawBorders() {
    std::cout << "\033[2J"; // Clear screen
    std::cout << "\033[1;1H"; // top left
    std::cout << "\033[1;36m"; // Cyan
    std::cout << "╔═════════════════════════════════════════════════════════╗\n";
    std::cout << "║\033[1;37m                 SYSTEM RESOURCE MONITOR                 \033[1;36m║\n";
    std::cout << "╠═════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                         ║\n"; // 4 - CPU
    std::cout << "║                                                         ║\n"; // 5 - RAM
    std::cout << "║                                                         ║\n"; // 6 - DISK
    std::cout << "╠═════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                         ║\n"; // 8 - Top Procs Title
    std::cout << "║                                                         ║\n"; // 9 - Headings
    std::cout << "║                                                         ║\n"; // 10
    std::cout << "║                                                         ║\n"; // 11
    std::cout << "║                                                         ║\n"; // 12
    std::cout << "║                                                         ║\n"; // 13
    std::cout << "║                                                         ║\n"; // 14
    std::cout << "╚═════════════════════════════════════════════════════════╝\n";
    std::cout << "\033[0m";
    
    std::cout << "\033[2;45H\033[1;32mRefresh: 1s\033[0m";
}

void Dashboard::start() {
    running = true;
    drawBorders();

    while (running) {
        // Refresh data
        for (auto& m : monitors) {
            m->refresh();
        }

        // Display Data
        int row = 4;
        for (size_t i = 0; i < monitors.size(); ++i) {
            if (i < 3) { // CPU, RAM, Disk
                monitors[i]->display(row++, 4);
            } else { // ProcessMonitor
                monitors[i]->display(8, 4);
            }
        }

        Sleep(1000);
    }
}

void Dashboard::stop() {
    running = false;
}
