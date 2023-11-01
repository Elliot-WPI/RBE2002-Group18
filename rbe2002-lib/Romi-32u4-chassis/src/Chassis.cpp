#include <Arduino.h>
#include <Chassis.h>
#include <Romi32U4Motors.h>

// We'll declare motors as global to make the ISRs happier, but we'll put them in Chassis.cpp
// to keep things organized

LeftMotor leftMotor;
RightMotor rightMotor;

Chassis::Chassis(void) {}

void Chassis::init(void)
{  
    noInterrupts(); //disable interupts while we mess with the Timer4 registers
  
    //sets up timer 4
    TCCR4A = 0x00; //disable some functionality -- no need to worry about this
    TCCR4B = 0x0A; //sets the prescaler -- look in the handout for values
    TCCR4C = 0x04; //toggles pin 6 at the timer frequency
    TCCR4D = 0x00; //normal mode

    /**
     * Here we do a little trick to allow full 10-bit register access. 
     * We have 2 bytes in TC4H that we can use to add capacity to TOP. 
     * In the end, this sets TOP = 2 * 256 + 112 = 624
    */
    TC4H = 2; OCR4C = 112;

    TIMSK4 = 0x04; //enable overflow interrupt

    interrupts(); //re-enable interrupts

    // init the motors
    Romi32U4Motor::init();

    //pinMode(6, OUTPUT); //COMMENT THIS OUT TO SHUT UP THE PIEZO!!!
}

bool Chassis::loop(void)
{
    bool retVal = false;
    if(readyToPID)
    {
        if(readyToPID > 1) Serial.println("Missed update in Chassis::loop()");

        update();
        readyToPID = 0;
        retVal = true;
    }

    return retVal;
}

void Chassis::update(void)
{
    leftMotor.update();
    rightMotor.update();
#ifdef __MOTOR_DEBUG__
    Serial.print('\n');
#endif
}

void Chassis::setMotorTargetSpeeds(float leftTicksPerInterval, float rightTicksPerInterval)
{
    leftMotor.setTargetSpeed(leftTicksPerInterval);
    rightMotor.setTargetSpeed(rightTicksPerInterval);
}

/*
 * ISR for timing. On overflow, it takes a 'snapshot' of the encoder counts and raises a flag to let
 * the main program it is time to execute the PID calculations.
 */
ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  leftMotor.calcEncoderDelta();
  rightMotor.calcEncoderDelta();

  chassis.readyToPID++;
}
