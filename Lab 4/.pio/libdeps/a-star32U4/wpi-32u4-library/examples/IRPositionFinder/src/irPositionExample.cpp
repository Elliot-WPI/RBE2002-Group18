/*!
 * @file simpleDemo.ino
 * @brief DFRobot's Positioning ir camera
 * @n [Get the module here](http://www.dfrobot.com/index.php?route=product/product&product_id=1088)
 * @n This example get the four lightest positions of the IR sources.
 * @n [Connection and Diagram](http://www.dfrobot.com/wiki/index.php/Positioning_ir_camera#Connection_Diagram)
 *
 * @copyright	[DFRobot](http://www.dfrobot.com), 2016
 * @copyright	GNU Lesser General Public License
 *
 * @author [Angelo](Angelo.qiao@dfrobot.com)
 * @version  V1.0
 * @date  2016-02-17
 */

#include <Arduino.h>
#include <Wire.h>

/*!
 * @brief Library for DFRobot's IR Position Camera
 * @author [Angelo](Angelo.qiao@dfrobot.com)
 */
#include <IRDirectionFinder.h>

IRDirectionFinder irFinder;

void setup()
{
  Serial.begin(115200);
  
  /*!
   *  @brief initailize the module.
   */
  irFinder.begin();
}

void loop()
{
  /*!
   *  @brief request the position
   */
  irFinder.requestPosition();
  
  /*!
   *  @brief If there is data available, print it. Otherwise show the error message.
   */
  if (irFinder.available()) {
    for (int i=0; i<4; i++) 
    {
      Point point = irFinder.ReadPoint(i);
      Serial.print(point.x);
      Serial.print(",");
      
      Serial.print(point.y);
      Serial.print(";");
    }
    Serial.print('\n');
  }
  else{
    Serial.println("Device not available!");
  }
  
  delay(50); //this is just a test program, so we'll allow a delay here
}
