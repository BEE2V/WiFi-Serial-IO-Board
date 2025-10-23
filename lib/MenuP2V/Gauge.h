#ifndef GAUGE_H
#define GAUGE_H

#include "MenuItem.h"
#include <U8g2lib.h>
#include <Arduino.h>

// -----------------------------
// Gauge configuration structure
// -----------------------------
struct GaugeParameters
{
    byte size;      // 64 or 128
    byte arcStep;   // Step between arc segments
    byte bandWidth; // Arc thickness
    byte tipRadius; // Tip circle radius

    const uint8_t *value_font;
    byte valueOffset;
    const char *precision;

    const uint8_t *label_font;
    byte labelOffset;
};

// -----------------------------
// Predefined parameter sets
// -----------------------------
extern GaugeParameters smallGauge;
extern GaugeParameters largeGauge;

// -----------------------------
// Size options
// -----------------------------
enum SizeOption
{
    SMALL,
    LARGE
};

// -----------------------------
// Gauge widget class
// -----------------------------
class Gauge : public MenuItem
{
private:
    float minVal;
    float maxVal;
    float value;
    const char *label;
    GaugeParameters *params;

public:
    Gauge(const char *lbl, float minV, float maxV, SizeOption sz);

    void updateValue(float val);
    void draw(U8G2 &u8g2, int16_t x, int16_t y);
};

#endif // GAUGE_H
