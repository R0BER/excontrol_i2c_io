// excontrol ads7828
// by excontrol <https://excontrol.es/shop/producto/industrial-extension-i2c-analog/>

// Demonstrates use of excontrol i2c io board
// Created 19  April 2019

// This example code is in the public domain.


#include <excontrol_i2c_io.h>

I2cIO IoBoard(0x20); //Start with in default address
void setup()
{
  
  Wire.begin();
  IoBoard.begin();
  
}

void loop()
{
  IoBoard.update();//Update all

  //Read Input
  int Input0=IoBoard.ReadInput(0);//Read input A0, value off LOW, value on = HIGH
  int Input1=IoBoard.ReadInput(1);//Read input A1, value off LOW, value on = HIGH
  int Input2=IoBoard.ReadInput(2);//Read input A2, value off LOW, value on = HIGH
  int Input3=IoBoard.ReadInput(3);//Read input A2, value off LOW, value on = HIGH

  //Write input in Output
  IoBoard.SetRelay(0, Input0);//Write Input 0 In  Output 0
  IoBoard.SetRelay(1, Input1);//Write Input 1 In  Output 1
  IoBoard.SetRelay(2, Input2);//Write Input 2 In  Output 2
  IoBoard.SetRelay(3, Input3);//Write Input 3 In  Output 3

 // if (IoBoard.ReadInput(1)==HIGH){Serial.println("on");delay(1000);}
  
}
