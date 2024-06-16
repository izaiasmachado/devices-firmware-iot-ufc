#include <ESP8266WiFi.h>

#define PIN_SENSOR D7

void setup()
{
    pinMode(PIN_SENSOR, INPUT);
    Serial.begin(115200);
}

void loop()
{
    int sensorSignal = digitalRead(PIN_SENSOR);

    // print time and sensor signal
    Serial.print(millis());

    if (sensorSignal == HIGH)
    {
        Serial.println(" Motion detected!");
    }
    else
    {
        Serial.println(" No motion detected!");
    }

    delay(1000);
}
