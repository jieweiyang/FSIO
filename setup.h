#pragma once
#include <Arduino.h>

class PinFunction
{
public:
	PinFunction(byte NumberOfFunc);
	~PinFunction();

	

	byte getPinFunc(byte Pin);
	void setPinFunc(byte Pin, byte Func);
	
	String getAllFunc();
	byte getNumberOfPins();


private:
	byte *PinFunc;
	byte Functions;


};

