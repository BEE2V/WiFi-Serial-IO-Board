#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include <Buttons165.h>
#include <Analog.h>
#include <MenuItem.h>

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

void setup()
{
  Serial.begin(115200);

  pot1.setScale(20, 3880, 0, 100); // scale to 0-100
  pot2.setScale(40, 3860, 0, 100); // scale to 0-100
  pot1.setFilterMovingAvg(10);     // 10-sample moving average
  pot2.setFilterMovingAvg(10);     // 10-sample moving average

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

    itemRun.draw_tile(u8g2, 0, 0, true); // selected
    itemCalibrate.draw_tile(u8g2, 64, 0);
    // itemPID.draw_tile(u8g2, 0, 60);
    // itemDash.draw_tile(u8g2, 64, 60);

    itemRunRow.draw_row(u8g2, 0, 70, true);
    itemInfoRow.draw_row(u8g2, 0, 94);

  } while (u8g2.nextPage()); // go over all the pages until the whole display is updated

  Serial.println();
  // delay(100);
}
