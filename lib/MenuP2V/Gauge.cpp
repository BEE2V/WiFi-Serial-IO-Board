#include "Gauge.h"
#include <math.h>

// ------------------------------------
// Gauge parameter definitions
// ------------------------------------
GaugeParameters smallGauge = {
    64, 10, 4, 3,
    u8g2_font_6x13B_tr, 6, "%.2f",
    u8g2_font_6x12_tr, 8};

GaugeParameters largeGauge = {
    128, 5, 6, 5,
    u8g2_font_profont22_tr, 10, "%.3f",
    u8g2_font_profont17_tr, 15};

// ------------------------------------
// Gauge Constructor
// ------------------------------------
Gauge::Gauge(const char *lbl, float minV, float maxV, SizeOption sz)
    : MenuItem(nullptr), label(lbl), minVal(minV), maxVal(maxV), value(0)
{
    params = (sz == SMALL) ? &smallGauge : &largeGauge;
}

// ------------------------------------
// Update value (clamped to min/max)
// ------------------------------------
void Gauge::updateValue(float val)
{
    value = constrain(val, minVal, maxVal);
}

// ------------------------------------
// Draw gauge using parameters
// ------------------------------------
void Gauge::draw(U8G2 &u8g2, int16_t x, int16_t y)
{
    int size = params->size;

    // --- Center and radius setup ---
    int centerX = x + size / 2;
    int centerY = y + (size * 3 / 5);
    int radiusOuter = size / 2 - 2;
    int radiusInner = radiusOuter - params->bandWidth;

    // --- Clamp and map value ---
    float ratio = (value - minVal) / (maxVal - minVal);
    float startAngle = 150; // bottom-left
    float totalSpan = 2 * (270 - startAngle);
    float currentAngle = startAngle + (totalSpan * ratio);

    // --- Draw arc band ---
    for (float a = startAngle; a < currentAngle - params->arcStep; a += params->arcStep)
    {
        float rad = a * M_PI / 180.0;
        int x1 = centerX + cos(rad) * radiusInner;
        int y1 = centerY + sin(rad) * radiusInner;
        int x2 = centerX + cos(rad) * radiusOuter;
        int y2 = centerY + sin(rad) * radiusOuter;
        u8g2.drawLine(x1, y1, x2, y2);
    }

    // --- Draw tip circle ---
    {
        float rad = currentAngle * M_PI / 180.0;
        int tipX = centerX + cos(rad) * ((radiusOuter + radiusInner) / 2);
        int tipY = centerY + sin(rad) * ((radiusOuter + radiusInner) / 2);
        u8g2.drawCircle(tipX, tipY, params->tipRadius, U8G2_DRAW_ALL);
    }

    // --- Label ---
    u8g2.setFont(params->label_font);
    u8g2.setCursor(centerX - (u8g2.getStrWidth(label) / 2), centerY + params->labelOffset);
    u8g2.print(label);

    // --- Numeric value ---
    char valStr[10];
    sprintf(valStr, params->precision, value);
    u8g2.setFont(params->value_font);
    u8g2.setCursor(centerX - (u8g2.getStrWidth(valStr) / 2), centerY - params->valueOffset);
    u8g2.print(valStr);
}
