/*
   excontrol_i2c_io.h - Library for excontrol i2c io.
  Copyright (c) 2019 ExControl. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "Arduino.h"
#include <Wire.h> 
#include <excontrol_i2c_io.h>


/* Constructor */
I2cIO::I2cIO(int addr)
{	
	_addr = addr;	
}

void I2cIO::begin()
{
	TimI2CFilter = 0;
	Rcycle = 0;
	InitMCP23017();
	if (IoBoards.Online) { IoBoards.Inputs = ReadMCP23017(); IoBoards.Inputs2 = IoBoards.Inputs; IoBoards.Inputs3 = IoBoards.Inputs; }
	else { IoBoards.Inputs = 0; IoBoards.Inputs2 = 0; IoBoards.Inputs3 = 0; }
	IoBoards.Outputs = 0;
}
void I2cIO::update()
{
	unsigned long TimNow = millis();
	if (TimNow < TimI2CFilter) {TimI2CFilter = TimNow; }
	else { 
		if ((TimNow - TimI2CFilter) >= 30) {
			TimI2CFilter = TimNow; 
			ReadI2Cinputs();
			Rcycle++;			
		} 
	}
	if (Rcycle >= 3) { 
		WriteMCP23017(IoBoards.Outputs);
		Rcycle = 0;
	}	
}
int I2cIO::ReadInput(byte InputNumber) {
	if (bitRead(IoBoards.Inputs, InputNumber) == 1) { return LOW; }
	else {return HIGH;}
}
void I2cIO::SetRelay(byte OtNumber, int Value) {
	if (Value == HIGH) { bitWrite(IoBoards.Outputs, OtNumber, 1); }
	else { bitWrite(IoBoards.Outputs, OtNumber, 0); }
}
void I2cIO::InitMCP23017() {
	
	byte R = 0;
	while (R < 2) {//Configure Output
		Wire.beginTransmission(_addr);
		Wire.write(0x00); // IODIRA register
		Wire.write(0x00); // set all of port A to outputs
		Wire.write(0xFF); // set all of port b to input    
		if (Wire.endTransmission() == 0) {
			IoBoards.Online = true; return;
		}
		else {R++;}
	}
	IoBoards.Online = false;
}
byte I2cIO::ReadMCP23017() {
	if (IoBoards.Online == true) {
		byte R = 0;
		while (R < 3) {
			delay(1);
			Wire.beginTransmission(_addr);
			Wire.write(0x13);
			if (Wire.endTransmission() == 0) {
				Wire.requestFrom(_addr, 1);//Wire.requestFrom(IC24C32_I2C_ADDRESS,1);
				if (Wire.available() == 1) { return Wire.read(); }
			}
			delay(10);
			R++;
		}
		IoBoards.Online = false;
	}
	else { InitMCP23017(); }
	return IoBoards.Inputs;
}
void I2cIO::WriteMCP23017(byte  regValue) {
	if (IoBoards.Online == true) {
		byte R = 0;
		while (R < 3) {
			delay(1);
			Wire.beginTransmission(_addr);
			Wire.write(0x12);
			Wire.write(regValue);
			if (Wire.endTransmission() == 0) { return; }
			else {R++;}
		}
		IoBoards.Online = false;
	}
	else { InitMCP23017(); }
}
void I2cIO::ReadI2Cinputs() {
	byte rst = ReadMCP23017();
	if (rst != IoBoards.Inputs) {
		if (rst != IoBoards.Inputs2) { IoBoards.Inputs2 = rst; }
		else if (rst != IoBoards.Inputs3) { IoBoards.Inputs3 = rst; }
		else { IoBoards.Inputs = rst; }
	}
	else { IoBoards.Inputs2 = rst; IoBoards.Inputs3 = rst; }	
}