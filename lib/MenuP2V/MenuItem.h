#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <vector>
#include "Icons.h"

class MenuItem
{
private:
    static uint16_t nextId;           // auto-incrementing ID
    uint16_t id;                      // unique ID
    const IconDef *icon;              // pointer to associated icon
    MenuItem *parent;                 // parent item (optional)
    std::vector<MenuItem *> children; // dynamic child list

public:
    // --- Constructor ---
    explicit MenuItem(const IconDef *iconDef);

    // --- Hierarchy functions ---
    void addChild(MenuItem *child);
    MenuItem *getChild(uint8_t index);
    MenuItem *getParent() const { return parent; }

    // --- Info accessors ---
    uint16_t getId() const { return id; }
    const char *getLabel() const { return icon->label; }
    size_t getChildCount() const { return children.size(); }

    // --- Drawing functions ---
    void draw_tile(U8G2 &u8g2, int16_t x, int16_t y, bool selected = false);
    void draw_row(U8G2 &u8g2, int16_t x, int16_t y, bool selected = false);
};
