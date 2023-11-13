#include <Arduino.h>
#include <Romi32U4.h>
#include "Behaviors.h"

Behaviors robot;

void setup() {
  robot.Init();
}

void loop() {
  robot.Run();
}