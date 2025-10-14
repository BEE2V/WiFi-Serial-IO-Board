#include <Arduino.h>
#include <Buttons165.h>

Buttons165 serialButtons(4, 7, 6, 2);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    serialButtons.update();

    for (int i = 0; i < serialButtons.getButtonCount(); i++)
    {
        Serial.print(serialButtons.get(i).state());
    }
    Serial.println();
    delay(100);
}
