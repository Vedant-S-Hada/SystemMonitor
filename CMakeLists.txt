cmake_minimum_required(VERSION 3.10)
project(SystemMonitorGUI)

set(CMAKE_CXX_STANDARD 17)

# Include ImGui headers
include_directories(vendor/imgui vendor/imgui/backends)

# Source files
set(SOURCES
    gui_main.cpp
    CPUMonitor.cpp
    RAMMonitor.cpp
    DiskMonitor.cpp
    ProcessMonitor.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_win32.cpp
    vendor/imgui/backends/imgui_impl_dx11.cpp
)

add_executable(SystemMonitorGUI WIN32 ${SOURCES})

# Define _WIN32_WINNT for ProcessMonitor APIs
target_compile_definitions(SystemMonitorGUI PRIVATE _WIN32_WINNT=0x0600)

# Link required Windows libraries
target_link_libraries(SystemMonitorGUI psapi d3d11 d3dcompiler dxgi dwmapi)
target_link_options(SystemMonitorGUI PRIVATE -static)
