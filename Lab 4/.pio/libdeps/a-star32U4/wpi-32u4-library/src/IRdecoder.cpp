#include <IRdecoder.h>
#include <FastGPIO.h>
#include <pcint.h>

/* Interprets an IR remote with NEC encoding. See IRdecoder.h for more explanation. */

uint8_t digitalPinToPCInterrupt(uint8_t pin)
{
  uint8_t pcInt = NOT_AN_INTERRUPT;

#if defined(__AVR_ATmega32U4__)
  switch(pin)
  {
    case 17: pcInt = PCINT0; break;
    case 15: pcInt = PCINT1; break;
    case 16: pcInt = PCINT2; break;
    case 14: pcInt = PCINT3; break;
    case  8: pcInt = PCINT4; break;
    case  9: pcInt = PCINT5; break;
    case 10: pcInt = PCINT6; break;
    case 11: pcInt = PCINT7; break;
    default: break;
  }
#endif

  return pcInt;
}

void handleIRsensor(void)
{
  decoder.handleIRsensor();
}

void IRDecoder::init(void)
{
  pinMode(pin, INPUT);

  if(digitalPinToInterrupt(pin) != NOT_AN_INTERRUPT)
  {
    attachInterrupt(digitalPinToInterrupt(pin), ::handleIRsensor, CHANGE);
  }
  else if(digitalPinToPCInterrupt(pin) != NOT_AN_INTERRUPT)
  {
    attachPCInt(digitalPinToPCInterrupt(pin), ::handleIRsensor);
  }
  else
  {
    Serial.println("Not an interrupt pin!");
  }
}

void IRDecoder::handleIRsensor(void)
{
  uint32_t currUS = micros();

  //if(!FastGPIO::Pin<14>::isInputHigh()) // could use FastGPIO for speed
  if(!digitalRead(pin)) // FALLING edge
  {
    fallingEdge = currUS; 
  }

  else // RISING edge
  {
    risingEdge = currUS;

    //and process
    uint32_t delta = risingEdge - fallingEdge; //length of pulse, in us
    uint32_t codeLength = risingEdge - lastRisingEdge;
    lastRisingEdge = risingEdge;

    // bits[index] = delta; //used for debugging; obsolete
    
    if(delta > 8500 && delta < 9500) // received a start pulse
    {
      index = 0;
      state = IR_PREAMBLE;
      return;
    }
    
    //a pulse is supposed to be 562.5 us, but I found that it averaged 620us or so
    //with the sensor that we're using, which is NOT optimized for IR remotes --
    //it's actually optimized for sensitivity. So I set the maximum accepted pulse
    //length to 700us

    else if(delta < 500 || delta > 700) // pulse wasn't right length => error
    {
      state = IR_ERROR;
      currCode = -1;

      return;
    }

    else if(state == IR_PREAMBLE)
    {
      if(codeLength < 5300 && codeLength > 4800) //preamble
      {
        currCode = 0;
        state = IR_ACTIVE;
      }

      else if(codeLength < 3300 && codeLength > 2700) //repeat code
      {
        state = IR_REPEAT;
        if(((currCode ^ (currCode >> 8)) & 0x00ff0000) != 0x00ff0000) {state = IR_ERROR;} //but recheck code!
        lastReceiveTime = millis(); //not really used
      }
    }

    else if(state == IR_ACTIVE)
    {
      if(codeLength < 1300 && codeLength > 900) //short = 0
      {
        index++;
      }
      
      else if(codeLength < 2500 && codeLength > 2000) //long = 1
      {
        currCode += ((uint32_t)1 << index);
        index++;
      }
      
      else //error
      {
        state = IR_ERROR;
      }

      if(index == 32) //full set of bits
      {
        //first, check for errors
        if(((currCode ^ (currCode >> 8)) & 0x00ff0000) != 0x00ff0000) state = IR_ERROR;

        else //we're good to go
        {        
          state = IR_COMPLETE;
          lastReceiveTime = millis();
        }
      }
    }
  }
}
