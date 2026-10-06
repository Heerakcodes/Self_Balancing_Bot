#include <Arduino.h>
#include "IMUSensor.h"

IMUSensor imu;

void setup()
{
    Serial.begin(115200);

    delay(1000);

    imu.begin();
}

void loop()
{
    imu.update();

    delay(200);
}