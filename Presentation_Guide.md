# Presentation Guide: Real-Time System Resource Monitor

This guide maps the core Object-Oriented Programming (OOP) concepts directly to your C++ codebase. Use this script and the provided code snippets to confidently explain your project to your teacher.

---

## 1. Introduction (The Hook)
**What to say:**
> *"For my project, I built a Native Real-Time System Resource Monitor. Instead of a simple menu-driven console app, I wanted to build something practical that interacts directly with the Windows Operating System kernel to fetch live hardware metrics. I used DirectX 11 and Dear ImGui for a hardware-accelerated GUI. However, the core focus of the project was designing a highly modular, Object-Oriented architecture to handle the incoming data streams."*

---

## 2. The Core Architecture (The 4 Pillars of OOP)

### A. Abstraction
**What to say:**
> *"To keep the code clean, I used **Abstraction** by creating a pure virtual base class called `Monitor`. This acts as an interface. It hides complex implementation details and enforces a rule: Any hardware monitor MUST know how to fetch its own data (`refresh()`) and MUST know how to draw itself (`renderGUI()`)."*

**Code Evidence (`Monitor.h`):**
```cpp
class Monitor {
public:
    virtual ~Monitor() = default;
    
    // Pure virtual methods defining the interface
    virtual void refresh() = 0;
    virtual void renderGUI() = 0;
};
```

### B. Inheritance
**What to say:**
> *"I utilized **Inheritance** to create specific hardware components like `CPUMonitor` and `RAMMonitor`. They inherit from the `Monitor` base class, allowing me to reuse the shared interface while giving each component its own unique behavior."*

**Code Evidence (`CPUMonitor.h`):**
```cpp
#include "Monitor.h"
#include <windows.h>

// CPUMonitor inherits from the Monitor base class
class CPUMonitor : public Monitor {
private:
    double currentUsage;
    int numCores;
public:
    CPUMonitor();
    void refresh() override;      // Implements specific CPU fetching
    void renderGUI() override;    // Implements specific CPU drawing
};
```

### C. Encapsulation
**What to say:**
> *"Fetching data from Windows requires messy, low-level C APIs. I used **Encapsulation** to wrap all of that ugly code safely inside the private methods of my classes. The main application loop has no idea what a `MEMORYSTATUSEX` struct is—it just calls `refresh()` and the class handles its internal state safely."*

**Code Evidence (`RAMMonitor.cpp`):**
```cpp
void RAMMonitor::refresh() {
    // Encapsulated Windows API logic (Hidden from main loop)
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    
    totalRAM_GB = memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
    double availRAM_GB = memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
    
    // Internal state updated securely
    usedRAM_GB = totalRAM_GB - availRAM_GB;
    usagePercent = (usedRAM_GB / totalRAM_GB) * 100.0;
}
```

### D. The Observer Pattern (The Anomaly System)
**What to say:**
> *"To make my project stand out, I implemented the Gang of Four **Observer Pattern**. I built an `AlertManager` singleton that acts as the Observer. The hardware monitors (like CPU and RAM) act as Subjects. Whenever they detect high usage, they automatically notify the AlertManager, which updates a live scrolling console on the dashboard. This proves the system isn't just passively reading data; it's actively analyzing it."*

**Code Evidence (`RAMMonitor.cpp`):**
```cpp
if (usagePercent > 80.0) {
    char buf[128];
    snprintf(buf, sizeof(buf), "[WARNING] High RAM Usage detected: %.1f%%", usagePercent);
    AlertManager::getInstance().addAlert(buf); // Notifies the Observer!
}
```

### E. Polymorphism & The Open-Closed Principle
**What to say:**
> *"My proudest achievement is the use of **Polymorphism**. In my Dashboard, I don't store separate variables for CPU, RAM, and Disk. I store a single `std::vector` of base `Monitor` pointers. During the render loop, I simply call `renderGUI()`. Because of **Dynamic Dispatch**, the program automatically knows what to draw. To prove the **Open-Closed Principle**, I easily added a brand new `NetworkMonitor` class. Because it inherited from `Monitor`, I just dropped it into the vector, and it worked instantly without modifying the core drawing loop!"*

**Code Evidence (`gui_main.cpp`):**
```cpp
// 1. Polymorphic Container
std::vector<std::unique_ptr<Monitor>> monitors;

// 2. Populating with derived classes (including the new NetworkMonitor)
monitors.push_back(std::make_unique<CPUMonitor>());
monitors.push_back(std::make_unique<RAMMonitor>());
monitors.push_back(std::make_unique<NetworkMonitor>());

// 3. Polymorphic loop (Dynamic Dispatch in action)
for (auto& m : monitors) {
    m->renderGUI(); // Automatically calls the correct derived method!
}
```

---

## 3. Interactive Functionality (The "Task Killer")
**What to say:**
> *"I also wanted to differentiate my project from a read-only dashboard. I updated the Process Monitor to be fully interactive. By clicking a process row and pressing 'End Task', my C++ code invokes `OpenProcess` and `TerminateProcess` via the Windows Kernel, acting as a fully functional Task Manager."*

---

## 3. Modern C++ Features (Bonus Points)

### Memory Management (RAII & Smart Pointers)
**What to say:**
> *"To prevent memory leaks, I strictly used Modern C++ memory management via **`std::unique_ptr`**. Because of the RAII (Resource Acquisition Is Initialization) design pattern, when the application closes, the vector is destroyed, and the smart pointers automatically clean up the heap memory."*

**Code Evidence (`gui_main.cpp`):**
```cpp
// Using std::unique_ptr ensures memory is automatically freed.
// No manual 'delete' required!
std::vector<std::unique_ptr<Monitor>> monitors;
monitors.push_back(std::make_unique<CPUMonitor>());
```

---

## 4. Conclusion
**What to say:**
> *"Ultimately, C++ was the perfect language for this. It gave me the low-level memory access required to query the Windows kernel, while simultaneously giving me the high-level Object-Oriented tools needed to structure a scalable, polymorphic application."*
