#include "ButtonBase.h"

ButtonBase::ButtonBase() : lastPressed(0), flags(0) {}

void ButtonBase::update()
{
    if (millis() - lastPressed > debounce)
    {
        bitWrite(flags, 0, readRaw());
        lastPressed = millis();
    }

    bitWrite(flags, 3, bitRead(flags, 0) & (!bitRead(flags, 1))); // Rising edge
    bitWrite(flags, 4, (!bitRead(flags, 0)) & bitRead(flags, 1)); // Falling edge

    if (bitRead(flags, 3))
    {
        bitWrite(flags, 2, !bitRead(flags, 2)); // Toggle
    }

    bitWrite(flags, 1, bitRead(flags, 0)); // Store prev state
}

String ButtonBase::getFlagString(byte bits)
{
    String s;
    for (int i = bits - 1; i >= 0; i--)
        s += bitRead(flags, i);
    return s;
}

bool ButtonBase::state() { return bitRead(flags, 0); }
bool ButtonBase::preState() { return bitRead(flags, 1); }
bool ButtonBase::toggleState() { return bitRead(flags, 2); }
bool ButtonBase::risingEdge() { return bitRead(flags, 3); }
bool ButtonBase::fallingEdge() { return bitRead(flags, 4); }
