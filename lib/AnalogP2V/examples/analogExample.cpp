#include "Analog.h"

Analog pot1(2);
Analog pot2(3);

void setup()
{
    Serial.begin(115200);

    pot1.setFilterMovingAvg(10);
    pot2.setFilterComplementary(0.2);

    pot1.setScale(0, 4095, 0, 1);   // normalized 0–1
    pot2.setScale(0, 4095, 0, 100); // 0–100 range

    pot1.setAutoCalibrate(true); // learns min/max automatically
    pot2.onChange([](float val)
                  {
    Serial.print("Pot2 changed: ");
    Serial.println(val); }, 0.05f);
}

void loop()
{
    pot1.update();
    pot2.update();

    Serial.print("Pot1: ");
    Serial.print(pot1.getValue(), 3);
    Serial.print("\tPot2: ");
    Serial.println(pot2.getValue(), 2);

    delay(50);
}
