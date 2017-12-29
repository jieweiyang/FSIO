#pragma once
#include <Arduino.h>


class ShiftRegister595
{

private:

	byte bitLedCol[8] = { 1,2,4,8,16,32,64,128 };

	byte pinCS;
	byte DeviceCount = 8;
	byte LedStatus[8];  // Maximum 8 pcs of 74HC595.

	//bool isUpdated = false;
	
public:
	ShiftRegister595(int csPin);
	~ShiftRegister595();
	

	/*
	Set LED status
	addr - number of chips
	pos - pin on chip
	isOn - 1 to light up.
	*/
	void setLed(int addr, int pos, int isOn);

	void setLed(int cmd, int isOn);

	void refresh();
	
	void testAll();
	void begin();
};
