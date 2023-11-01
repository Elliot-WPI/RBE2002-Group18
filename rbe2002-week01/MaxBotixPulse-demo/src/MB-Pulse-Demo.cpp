/**
 * Unlike previous libraries, this one does not make use of extern variables, which are one
 * way to deal with the 'chicken and egg' problem of setting up interrupts. Because the timing
 * methods are part of a MaxBotixPulse class, they cannot be passed to Arduino's attachInterrupt()
 * function, which takes a `void fxn(void)` pointer as an argument.
 * 
 * What we'll do instead is declare the object (in this case mbPulse) and then call it from a global
 * function (in this case MB_ISR). We'll then pass the function to the init() method, which will set
 * things up for us.
 * 
 * There is a little more overhead for you; however, doing it this way is more proper and _it also
 * allows your to set up multiple sensors_ without having all the interrupt conflict.
*/

#include <Arduino.h>
#include <MaxBotix.h>
#include <Romi32U4Buttons.h>

MaxBotixPulse mbPulse(-1); //TODO: Choose your ECHO pin cxn

/**
 * MB_ISR() is a wrapper function that calls the member function for the object.
*/
void MB_ISR(void)
{
  mbPulse.mbISR();
}

void setup() 
{
  Serial.begin(115200);
  Serial.print("Welcome!\n");
  
  mbPulse.init(MB_ISR); // Pass the wrapper ISR to init()
}

void loop()
{
  float distance = 0;
  if(mbPulse.getDistance(distance))
  {
    Serial.println(distance);
  }
}
