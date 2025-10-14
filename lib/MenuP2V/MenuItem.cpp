#include "MenuItem.h"

uint16_t MenuItem::nextId = 0;

MenuItem::MenuItem(const IconDef *iconDef)
    : id(nextId++), icon(iconDef), parent(nullptr), childCount(0)
{
    for (auto &child : children)
        child = nullptr;
}

void MenuItem::addChild(MenuItem *child)
{
    if (childCount < 8)
    {
        children[childCount++] = child;
        child->parent = this;
    }
}

MenuItem *MenuItem::getChild(uint8_t index)
{
    if (index < childCount)
        return children[index];
    return nullptr;
}

MenuItem *MenuItem::getParent()
{
    return parent;
}

void MenuItem::draw_tile(U8G2 &u8g2, int16_t x, int16_t y, bool selected)
{
    // Outer selection frame
    if (selected)
    {
        u8g2.drawRFrame(x, y, 64, 60, 5);
    }

    // Inner 40x40 icon frame
    const int16_t frameX = x + 11;
    const int16_t frameY = y + 5;
    u8g2.drawRFrame(frameX, frameY, 40, 40, 5);

    // Draw icon (transparent)
    u8g2.setBitmapMode(1); // <-- transparent draw mode
    int16_t iconX = frameX + (40 - icon->width) / 2;
    int16_t iconY = frameY + (40 - icon->height) / 2;
    u8g2.drawXBMP(iconX, iconY, icon->width, icon->height, icon->bitmap);

    // Label
    u8g2.setFontMode(1);
    u8g2.setFont(u8g2_font_6x12_tr);
    uint8_t textWidth = u8g2.getStrWidth(icon->label);
    int16_t textX = x + (64 - textWidth) / 2;
    int16_t textY = y + 57;
    u8g2.drawStr(textX, textY, icon->label);
}

void MenuItem::draw_row(U8G2 &u8g2, int16_t x, int16_t y, bool selected)
{
    // Outer selection frame
    if (selected)
    {
        u8g2.drawRFrame(x, y, 120, 22, 4);
    }

    // Draw icon (transparent)
    u8g2.setBitmapMode(1); // <-- transparent draw mode
    int16_t iconX = x + 3;
    int16_t iconY = y + 3;
    u8g2.drawXBMP(iconX, iconY, icon->width, icon->height, icon->bitmap);

    // Label
    u8g2.setFontMode(1);
    u8g2.setFont(u8g2_font_profont17_tr);
    int16_t textX = x + 24;
    int16_t textY = y + 16;
    u8g2.drawStr(textX, textY, icon->label);
}