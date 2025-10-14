```c++
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

// === Wi-Fi credentials ===
const char *ssid = "Dialog 4G";
const char *password = "Q06YLA0QED3";

// === OLED setup ===
U8G2_SH1107_PIMORONI_128X128_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 9, 8); // SCL - 9 | SDA - 8

void setup(void)
{
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setContrast(255);

  // Connect Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  // OTA setup
  ArduinoOTA.setHostname("ESP32Clock");
  // Optional password (set if you want OTA upload protection)
  // ArduinoOTA.setPassword("your_ota_password");

  ArduinoOTA
      .onStart([]()
               { Serial.println("Start updating sketch"); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void loop()
{
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle(); // check for OTA updates

  u8g2.firstPage(); // select the first page of the display (page is 128x8px), since we are using the page drawing method of the u8g2 library
  do
  { // draw
    int Layer_1_radius = 4;
    int Layer_5_radius = 1;

    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.drawRFrame(0, 0, 120, 19, Layer_1_radius);

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(30, 14, "Menu Item 1");

    u8g2.drawLine(125, 0, 125, 128);

    u8g2.drawRFrame(0, 22, 120, 19, 4);

    u8g2.drawRFrame(0, 44, 120, 19, 4);

    u8g2.drawRBox(123, 48, 5, 16, Layer_5_radius);

    u8g2.setFont(u8g2_font_profont15_tr);
    u8g2.drawStr(30, 58, "Menu Item 3");

    u8g2.drawRFrame(0, 66, 120, 19, 4);

    u8g2.drawRFrame(0, 88, 120, 19, 4);

    u8g2.drawRFrame(0, 109, 120, 19, 4);

    u8g2.setFont(u8g2_font_t0_16_tr);
    u8g2.drawStr(30, 36, "Menu Item 2");

    u8g2.setFont(u8g2_font_t0_16b_tr);
    u8g2.drawStr(30, 80, "Menu Item 4");

    u8g2.setFont(u8g2_font_timR14_tr);
    u8g2.drawStr(19, 104, "Menu Item 5");

    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(30, 121, "Menu Item 6");

    u8g2.drawRFrame(4, 3, 13, 13, 3);

    u8g2.drawRFrame(4, 25, 13, 13, 3);

    u8g2.drawEllipse(7, 6, 1, 1);

    u8g2.drawEllipse(9, 28, 1, 1);

    u8g2.drawRFrame(4, 47, 13, 13, 3);

    u8g2.drawEllipse(12, 50, 1, 1);

    u8g2.drawRFrame(4, 69, 13, 13, 3);

    u8g2.drawEllipse(8, 74, 1, 1);

    u8g2.drawRFrame(4, 91, 13, 13, 3);

    u8g2.drawEllipse(11, 97, 1, 1);

    u8g2.drawRFrame(4, 112, 13, 13, 3);

    u8g2.drawEllipse(13, 121, 1, 1);

    u8g2.sendBuffer();

  } while (u8g2.nextPage()); // go over all the pages until the whole display is updated
}

```