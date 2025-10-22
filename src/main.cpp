#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include <Buttons165.h>
#include <Analog.h>
#include <MenuItem.h>
#include <math.h>

U8G2_SH1107_PIMORONI_128X128_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 9, 8); // SCL - 9 | SDA - 8

Buttons165 serialButtons(4, 7, 6, 2);

Button165 *up = &serialButtons.get(1);
Button165 *down = &serialButtons.get(4);
Button165 *left = &serialButtons.get(3);
Button165 *right = &serialButtons.get(0);
Button165 *ok = &serialButtons.get(2);

Analog pot1(3);
Analog pot2(2);

MenuItem itemRun(&ICON_RUN_MODE);
MenuItem itemCalibrate(&ICON_CALIBRATE);
MenuItem itemPID(&ICON_PID_TUNE);
MenuItem itemDash(&ICON_DASHBOARD);

MenuItem itemRunRow(&ICON_RUN_MODE_ROW);
MenuItem itemInfoRow(&ICON_INFO_ROW);

void draw_gauge(int x, int y, float minVal, float maxVal, float value, const char *label, int size = 128)
{
  // --- Scaling parameters ---
  bool small = (size <= 64);
  byte arcStep = small ? 10 : 5;  // fewer lines on smaller display
  byte bandWidth = small ? 4 : 6; // thickness of arc
  byte tipRadius = small ? 3 : 5; // circle at the end of arc
  const char *precision = small ? "%.2f" : "%.3f";
  const uint8_t *value_font = small ? u8g2_font_6x13B_tr : u8g2_font_profont22_tr;
  const uint8_t *label_font = small ? u8g2_font_6x12_tr : u8g2_font_profont17_tr;
  int valueOffset = small ? 6 : 10; // vertical spacing for value
  int labelOffset = small ? 8 : 15; // vertical spacing for label

  // --- Center and radius setup ---
  int centerX = x + size / 2;
  int centerY = y + (size * 3 / 5);
  int radiusOuter = size / 2 - 2;
  int radiusInner = radiusOuter - bandWidth;

  // --- Clamp and map value ---
  if (value < minVal)
    value = minVal;
  if (value > maxVal)
    value = maxVal;
  float ratio = (value - minVal) / (maxVal - minVal);

  // --- Arc range setup ---
  float startAngle = 150; // bottom-left
  float totalSpan = 2 * (270 - startAngle);
  float currentAngle = startAngle + (totalSpan * ratio);

  // --- Draw arc ---
  for (float a = startAngle; a < currentAngle - arcStep; a += arcStep)
  {
    float rad = a * M_PI / 180.0;
    int x1 = centerX + cos(rad) * radiusInner;
    int y1 = centerY + sin(rad) * radiusInner;
    int x2 = centerX + cos(rad) * radiusOuter;
    int y2 = centerY + sin(rad) * radiusOuter;
    u8g2.drawLine(x1, y1, x2, y2);
  }

  // --- Draw small outline circle at the end of the arc ---
  {
    float rad = currentAngle * M_PI / 180.0;
    int tipX = centerX + cos(rad) * ((radiusOuter + radiusInner) / 2);
    int tipY = centerY + sin(rad) * ((radiusOuter + radiusInner) / 2);
    u8g2.drawCircle(tipX, tipY, tipRadius, U8G2_DRAW_ALL);
  }

  // --- Label ---
  u8g2.setFont(label_font);
  u8g2.setCursor(centerX - (u8g2.getStrWidth(label) / 2), centerY + labelOffset);
  u8g2.print(label);

  // --- Numeric value ---
  char valStr[10];
  sprintf(valStr, precision, value);
  u8g2.setFont(value_font);
  u8g2.setCursor(centerX - (u8g2.getStrWidth(valStr) / 2), centerY - valueOffset);
  u8g2.print(valStr);
}

void setup()
{
  Serial.begin(115200);

  pot1.setScale(20, 3880, 0, 100); // scale to 0-100
  pot2.setScale(40, 3900, 0, 100); // scale to 0-100
  pot1.setFilterMovingAvg(15);     // 15-sample moving average
  pot2.setFilterMovingAvg(15);     // 15-sample moving average

  u8g2.begin();
  u8g2.setContrast(255);
}

void loop()
{
  serialButtons.update();

  pot1.update();
  pot2.update();

  pot1.update();
  pot2.update();

  Serial.print("Pot1: ");
  Serial.print(pot1.getValue(), 3);
  Serial.print("\tPot2: ");
  Serial.print(pot2.getValue(), 2);

  u8g2.firstPage(); // select the first page of the display (page is 128x8px), since we are using the page drawing method of the u8g2 library
  do
  { // draw

    // itemRun.draw_tile(u8g2, 0, 0, true); // selected
    // itemCalibrate.draw_tile(u8g2, 64, 0);
    // // itemPID.draw_tile(u8g2, 0, 60);
    // // itemDash.draw_tile(u8g2, 64, 60);

    // itemRunRow.draw_row(u8g2, 0, 70, true);
    // itemInfoRow.draw_row(u8g2, 0, 94);

    draw_gauge(0, 0, 0, 100, pot1.getValue(), "POT 1", 64);
    draw_gauge(64, 0, 0, 100, pot2.getValue(), "POT 2", 64);
    draw_gauge(0, 64, 0, 100, pot2.getValue(), "POT 2", 64);
    draw_gauge(64, 64, 0, 100, pot1.getValue(), "POT 1", 64);
    u8g2.drawRFrame(0, 0, 64, 64, 8);
    u8g2.drawRFrame(64, 0, 64, 64, 8);
    u8g2.drawRFrame(0, 64, 64, 64, 8);
    u8g2.drawRFrame(64, 64, 64, 64, 8);

    // draw_gauge(0, 0, 0, 100, pot2.getValue(), "POT 2", 128);
    // u8g2.drawRFrame(0, 0, 128, 128, 16);

  } while (u8g2.nextPage()); // go over all the pages until the whole display is updated

  Serial.println();
  // delay(100);
}