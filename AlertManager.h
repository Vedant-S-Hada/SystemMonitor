#pragma once
#include <string>
#include <vector>
#include <mutex>

struct Alert {
    std::string message;
    long long timestamp;
};

// Singleton AlertManager implementing the Observer Pattern
class AlertManager {
private:
    std::vector<Alert> alerts;
    std::mutex mtx;

    AlertManager() {} // Private constructor

public:
    // Delete copy/move constructors to ensure Singleton property
    AlertManager(const AlertManager&) = delete;
    AlertManager& operator=(const AlertManager&) = delete;

    static AlertManager& getInstance();

    void addAlert(const std::string& msg);
    std::vector<Alert> getAlerts() const;
    void clearAlerts();
};
