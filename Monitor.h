#pragma once

class Monitor {
public:
    virtual ~Monitor() = default;
    
    // Core pure virtual methods defining the Monitor interface
    virtual void refresh() = 0;
    virtual void display(int startRow, int startCol) = 0;
    virtual void renderGUI() = 0;
};
