#include "ProcessMonitor.h"
#include <psapi.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

ProcessMonitor::ProcessMonitor() {}

void ProcessMonitor::refresh() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return;
    }

    cProcesses = cbNeeded / sizeof(DWORD);
    std::vector<ProcessInfo> currentProcs;

    for (unsigned int i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            DWORD pid = aProcesses[i];
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (hProcess) {
                HMODULE hMod;
                DWORD cbNeededMod;
                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededMod)) {
                    char szProcessName[MAX_PATH];
                    GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(char));
                    
                    PROCESS_MEMORY_COUNTERS pmc;
                    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                        currentProcs.push_back({pid, std::string(szProcessName), pmc.WorkingSetSize});
                    }
                }
                CloseHandle(hProcess);
            }
        }
    }

    // Sort by memory usage descending
    std::sort(currentProcs.begin(), currentProcs.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.memUsage > b.memUsage;
    });

    // Keep top 5
    if (currentProcs.size() > 5) {
        currentProcs.resize(5);
    }
    topProcesses = currentProcs;
}

void ProcessMonitor::display(int startRow, int startCol) {
    std::cout << "\033[" << startRow << ";" << startCol << "H";
    std::cout << "\033[1;37m TOP PROCESSES (By Memory)\033[0m";
    
    std::cout << "\033[" << startRow+1 << ";" << startCol << "H";
    std::cout << "\033[1;30m PID    NAME                 MEM (MB)\033[0m";

    for (size_t i = 0; i < topProcesses.size(); ++i) {
        std::cout << "\033[" << startRow + 2 + i << ";" << startCol << "H";
        
        // Clear line up to certain width to prevent leftover characters from previous refreshes
        std::cout << "                                                         ";
        std::cout << "\033[" << startRow + 2 + i << ";" << startCol << "H";

        std::string name = topProcesses[i].name;
        if (name.length() > 18) name = name.substr(0, 15) + "...";
        
        double memMB = topProcesses[i].memUsage / (1024.0 * 1024.0);

        std::cout << "\033[0;36m" << std::left << std::setw(6) << topProcesses[i].pid << "\033[0m ";
        std::cout << "\033[1;32m" << std::left << std::setw(20) << name << "\033[0m ";
        std::cout << "\033[1;33m" << std::fixed << std::setprecision(1) << memMB << "\033[0m";
    }
}

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui.h"
#include "AlertManager.h"
void ProcessMonitor::renderGUI() {
    ImGui::Text("Top Processes (by Memory)");
    
    // Interactive Task Killer
    if (ImGui::Button("End Task (Kill Process)", ImVec2(200, 0))) {
        if (selectedProcessIndex >= 0 && selectedProcessIndex < topProcesses.size()) {
            DWORD pidToKill = topProcesses[selectedProcessIndex].pid;
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pidToKill);
            if (hProcess != NULL) {
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
                AlertManager::getInstance().addAlert("[INFO] Terminated process PID: " + std::to_string(pidToKill));
            } else {
                AlertManager::getInstance().addAlert("[ERROR] Failed to terminate PID: " + std::to_string(pidToKill) + " (Access Denied?)");
            }
        }
    }

    if (ImGui::BeginTable("Processes", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 150))) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Memory (MB)");
        ImGui::TableHeadersRow();

        for (int i = 0; i < topProcesses.size(); i++) {
            const auto& proc = topProcesses[i];
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            
            char label[32];
            sprintf(label, "%lu##%d", proc.pid, i);
            if (ImGui::Selectable(label, selectedProcessIndex == i, ImGuiSelectableFlags_SpanAllColumns)) {
                selectedProcessIndex = i;
            }
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", proc.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%.1f", proc.memUsage / (1024.0 * 1024.0));
        }
        ImGui::EndTable();
    }
}
