#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include "Icons.h"

class MenuItem
{
private:
    static uint16_t nextId; // auto-incrementing ID
    uint16_t id;            // unique ID
    const IconDef *icon;    // pointer to associated icon
    MenuItem *parent;       // parent item (optional)
    MenuItem *children[8];  // up to 8 children (extend if needed)
    uint8_t childCount;     // number of children

public:
    MenuItem(const IconDef *iconDef);

    void addChild(MenuItem *child);
    MenuItem *getChild(uint8_t index);
    MenuItem *getParent();

    uint16_t getId() const { return id; }
    const char *getLabel() const { return icon->label; }

    void draw_tile(U8G2 &u8g2, int16_t x, int16_t y, bool selected = false);
    void draw_row(U8G2 &u8g2, int16_t x, int16_t y, bool selected = false);
};
