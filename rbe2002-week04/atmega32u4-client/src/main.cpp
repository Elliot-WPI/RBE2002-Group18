#include <Arduino.h>
#include <Button.h>

Button buttonA2(A2);

/**
 * sendMessage creates a string of the form
 *      topic:message
 * which is what the corresponding ESP32 code expects.
 * */
void sendMessage(const String& topic, const String& message)
{
    Serial1.println(topic + String(':') + message);
}

String serString1;
bool checkSerial1(void)
{
    while(Serial1.available())
    {
        char c = Serial1.read();
        serString1 += c;

        if(c == '\n')
        {
            return true;
        }
    }

    return false;
}

void setup() 
{
    Serial.begin(115200);
    delay(500);  //give it a moment to bring up the Serial

    Serial.println("setup()");

    Serial1.begin(115200);

    buttonA2.init();

    Serial.println("/setup()");
}

/**
 * This basic example sends the time (from millis()) to an MQTT broker every
 * five seconds. See the `readme.md` in the root directory of this repo for 
 * how to set up credentials. 
 * */
void loop() 
{
    static uint32_t lastSend = 0;
    uint32_t currTime = millis();
    if(currTime - lastSend >= 5000) //send every five seconds
    {
        lastSend = currTime;
        sendMessage("timer/time", String(currTime));
    }

    if(checkSerial1())
    {
        Serial.print("Rec'd:\t");
        Serial.print(serString1);
        serString1 = "";
    }
}