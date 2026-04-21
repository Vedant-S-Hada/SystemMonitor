#include "NetworkMonitor.h"
#include <iostream>
#include <iomanip>

NetworkMonitor::NetworkMonitor() : uploadSpeedKB(0), downloadSpeedKB(0), prevBytesSent(0), prevBytesReceived(0), prevTime(0) {
    uploadHistory.resize(90, 0.0f);
    downloadHistory.resize(90, 0.0f);
}

void NetworkMonitor::refresh() {
    DWORD dwSize = 0;
    if (GetIfTable(NULL, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        MIB_IFTABLE* pIfTable = (MIB_IFTABLE*)malloc(dwSize);
        if (GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR) {
            unsigned long long currentBytesSent = 0;
            unsigned long long currentBytesReceived = 0;

            for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
                // Ignore software loopback (type 24)
                if (pIfTable->table[i].dwType != 24) { 
                    currentBytesSent += pIfTable->table[i].dwOutOctets;
                    currentBytesReceived += pIfTable->table[i].dwInOctets;
                }
            }

            DWORD currentTime = GetTickCount();
            if (prevTime != 0) {
                DWORD deltaTime = currentTime - prevTime;
                if (deltaTime > 0) {
                    uploadSpeedKB = ((currentBytesSent - prevBytesSent) / 1024.0) / (deltaTime / 1000.0);
                    downloadSpeedKB = ((currentBytesReceived - prevBytesReceived) / 1024.0) / (deltaTime / 1000.0);
                }
            }

            prevBytesSent = currentBytesSent;
            prevBytesReceived = currentBytesReceived;
            prevTime = currentTime;
        }
        free(pIfTable);
    }

    // Update histories
    uploadHistory.erase(uploadHistory.begin());
    uploadHistory.push_back((float)uploadSpeedKB);

    downloadHistory.erase(downloadHistory.begin());
    downloadHistory.push_back((float)downloadSpeedKB);
}

void NetworkMonitor::display(int startRow, int startCol) {
    std::cout << "\033[" << startRow << ";" << startCol << "H";
    std::cout << "\033[1;34m Network \033[0m ";
    std::cout << "Up: " << std::fixed << std::setprecision(1) << uploadSpeedKB << " KB/s | ";
    std::cout << "Down: " << downloadSpeedKB << " KB/s";
}

#include "vendor/imgui/imgui.h"
void NetworkMonitor::renderGUI() {
    ImGui::Text("Network Traffic");
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Upload: %.1f KB/s", uploadSpeedKB);
    ImGui::PlotLines("##Upload", uploadHistory.data(), uploadHistory.size(), 0, nullptr, 0.0f, FLT_MAX, ImVec2(0, 40));
    
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Download: %.1f KB/s", downloadSpeedKB);
    ImGui::PlotLines("##Download", downloadHistory.data(), downloadHistory.size(), 0, nullptr, 0.0f, FLT_MAX, ImVec2(0, 40));
}
