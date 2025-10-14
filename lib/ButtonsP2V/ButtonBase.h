#ifndef BUTTON_BASE_H
#define BUTTON_BASE_H

#include <Arduino.h>

class ButtonBase
{
protected:
    static const int debounce = 60;
    long lastPressed;
    byte flags; // bits: 0=State, 1=PreState, 2=Toggle, 3=RisingEdge, 4=FallingEdge

public:
    ButtonBase();
    virtual void update();
    virtual bool readRaw() = 0; // abstract: must be implemented in derived classes

    String getFlagString(byte bits = 5);

    bool state();
    bool preState();
    bool toggleState();
    bool risingEdge();
    bool fallingEdge();
};

#endif
