/*
  excontrol_ads7828.h - Library for I2cIO.
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

#ifndef excontrol_i2cIo_h
#define excontrol_i2cIo_h

#include "Arduino.h"
#include <Wire.h> 



class I2cIO
{
public:
	I2cIO(int addr);
	void begin();
	void update();
	int ReadInput(byte InputNumber);
	void SetRelay(byte OtNumber, int Value);
private:
	struct I2cBoard { byte Inputs; byte Inputs2; byte Inputs3; byte Outputs; boolean Online; };
	I2cBoard IoBoards;
	int _addr;
	byte Rcycle;
	unsigned long TimI2CFilter;
	void InitMCP23017();
	byte ReadMCP23017();
	void WriteMCP23017(byte  regValue);
	void ReadI2Cinputs();
};


#endif
