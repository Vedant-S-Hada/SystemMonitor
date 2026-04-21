# 💻 Real-Time System Resource Monitor

A high-performance, native Windows system resource monitor built in **C++** using **Object-Oriented Design Patterns (OODP)**. 

This application fetches live hardware metrics (CPU, RAM, Disk, and Process memory) using deep Windows APIs and renders them in a beautiful, hardware-accelerated GUI using **Dear ImGui** and **DirectX 11**.

## ✨ Features
* **Live Hardware Telemetry:** Accurately tracks CPU load, Physical RAM consumption, Network Traffic, and Disk Space.
* **Top Processes Tracking:** Scans running applications to display the top memory-heavy processes in real-time.
* **Smart Suggestion Engine:** Automatically identifies and suggests closing applications that consume massive amounts of RAM (e.g., > 500MB).
* **Interactive Task Killer:** Allows the user to select and securely terminate processes directly from the UI using Windows kernel APIs.
* **Anomaly Alert Console:** An integrated scrolling console that alerts the user when hardware thresholds (like 80% CPU/RAM) are exceeded.
* **Hardware-Accelerated UI:** Replaced standard console output with a fluid, 60fps graphical interface.
* **Zero Flicker:** Uses ImGui's immediate-mode rendering instead of legacy terminal clearing.
* **Standalone Executable:** Fully statically linked—runs out of the box with zero dependencies.

## 🏗️ Architecture & OOP Design
This project was strictly designed using Object-Oriented principles to ensure extensibility and clean code:

* **Abstraction (`Monitor` Base Class):** Defines a strict contract with pure virtual functions like `refresh()` and `renderGUI()`.
* **Inheritance:** Specific hardware components (`CPUMonitor`, `RAMMonitor`, `DiskMonitor`, `ProcessMonitor`, `NetworkMonitor`) inherit from the base class and implement their own OS-specific fetching logic.
* **Polymorphism & Open-Closed Principle:** The `Dashboard` orchestrator manages a polymorphic container (`std::vector<std::unique_ptr<Monitor>>`). We extended the system with a `NetworkMonitor` without modifying the core drawing loop, relying purely on dynamic dispatch.
* **Observer Pattern:** Implemented an `AlertManager` singleton. Hardware monitors act as Subjects and notify the AlertManager when specific thresholds are breached or when the Suggestion Engine flags a process.
* **Encapsulation:** All underlying Windows API calls (like `GetSystemTimes`, `GlobalMemoryStatusEx`, `EnumProcesses`, `GetIfTable`, `TerminateProcess`) are safely hidden inside their respective classes.

## 🛠️ Tech Stack
* **Language:** C++17
* **Compiler:** MinGW-w64 (MSYS2) / MSVC
* **Graphics API:** DirectX 11
* **UI Framework:** [Dear ImGui](https://github.com/ocornut/imgui)
* **System APIs:** Windows API (`<windows.h>`, `psapi.h`, `pdh.h`, `dwmapi.h`)

## 🚀 How to Run
1. Clone this repository.
2. Run the included `sysmon_gui.exe` file.
*(Note: Because the application queries deep system-level process information, some antivirus software may flag the executable. This is normal for system monitors).*

## 🔨 Compiling from Source
If you wish to compile it yourself using MSYS2 MinGW-w64:
```bash
g++ -std=c++17 -D_WIN32_WINNT=0x0600 -Ivendor/imgui -Ivendor/imgui/backends gui_main.cpp CPUMonitor.cpp RAMMonitor.cpp DiskMonitor.cpp ProcessMonitor.cpp NetworkMonitor.cpp AlertManager.cpp vendor/imgui/imgui.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_tables.cpp vendor/imgui/imgui_widgets.cpp vendor/imgui/backends/imgui_impl_win32.cpp vendor/imgui/backends/imgui_impl_dx11.cpp -o sysmon_gui.exe -static -lpsapi -ld3d11 -ld3dcompiler -ldxgi -ldwmapi -liphlpapi -mwindows
```


```mermaid
flowchart LR
    %% Actor Definition
    User((User))

    %% Detailed System Boundary
    subgraph "System Resource Monitor"
        CPU_View([View CPU Overview])
        CPU_Cores([View Detailed CPU Cores])
        CPU_Hist([View CPU Load History])
        RAM_View([View RAM Total/Used])
        Swap_View([View Swap Usage])
        Disk_IO([View Disk I/O Read/Write])
        Disk_Lat([View Disk Latency])
        Net_View([View Network Usage])
        Proc_View([View Processes])
        Proc_Filter([Filter Processes])
        Kill_Proc([Kill Process])
        Log_Usage([Log Resource Usage])
        Hist_View([View Historical Data])
        Config_Sample([Configure Sampling Rate])
        Config_Log([Enable/Disable Logging])
    end

    %% Main Relationships
    User --- CPU_View
    User --- RAM_View
    User --- Disk_IO
    User --- Net_View
    User --- Proc_View
    User --- Kill_Proc
    User --- Log_Usage
    User --- Hist_View
    User --- Config_Sample
    User --- Config_Log

    %% Include & Extend Relationships
    Kill_Proc -. "<<include>>" .-> Proc_View
    Proc_Filter -. "<<extend>>" .-> Proc_View
    CPU_Cores -. "<<extend>>" .-> CPU_View
    CPU_Hist -. "<<extend>>" .-> CPU_View
    Swap_View -. "<<extend>>" .-> RAM_View
    Disk_Lat -. "<<extend>>" .-> Disk_IO
    Config_Sample -. "<<extend>>" .-> Log_Usage
    Hist_View -. "<<extend>>" .-> Log_Usage
```


