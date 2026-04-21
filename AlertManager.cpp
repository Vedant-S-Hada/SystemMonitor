#include "AlertManager.h"
#include <windows.h>

AlertManager& AlertManager::getInstance() {
    static AlertManager instance;
    return instance;
}

void AlertManager::addAlert(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    // Prevent spamming the exact same alert multiple times per second
    if (!alerts.empty() && alerts.back().message == msg) {
        if (GetTickCount64() - alerts.back().timestamp < 3000) {
            return; // Only log the same message once every 3 seconds
        }
    }
    
    alerts.push_back({msg, (long long)GetTickCount64()});
    
    // Keep max 50 alerts
    if (alerts.size() > 50) {
        alerts.erase(alerts.begin());
    }
}

std::vector<Alert> AlertManager::getAlerts() const {
    return alerts; // Returns a copy (safe for multithreading though not strictly needed here)
}

void AlertManager::clearAlerts() {
    std::lock_guard<std::mutex> lock(mtx);
    alerts.clear();
}
