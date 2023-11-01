#include <Arduino.h>
#include <robot.h>

/**
 * Most of the accessories, we put in robot.h/.cpp. We put the IR remote here because it's 
 * responsible for overall command and control -- it's not something that the robot uses directly
 * for control
*/
#include <ir_codes.h>
#include <IRdecoder.h>
#define IR_PIN 1
IRDecoder decoder(IR_PIN);

void setup() 
{
    Serial.begin(115200);
    delay(500);

    Serial.println("setup()");

    decoder.init();

    initialize();

  Serial.println("/setup()");
}

void loop() 
{
    /**
     * Chassis::loop() returns true when the motor control loop fires. We can use that timer to trigger
     * any number of processes that we want to run on the same schedule, for example, the line following
     * controller.
    */
    chassis.loop();

    /**
     * But we can also process asynchronous events, such as IR remote presses or distance sensor readings.
    */
    int16_t keyCode = decoder.getKeyCode();
    if(keyCode != -1) handleKeyCode(keyCode);

    processDistanceSensors();
}