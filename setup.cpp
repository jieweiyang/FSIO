#include "setup.h"
#include <EEPROM.h>



PinFunction::PinFunction(byte NumberOfFunc)
{
	Functions = NumberOfFunc;

	PinFunc = new byte[7];

	for (int i = 0;i <= 6;i++)
	{
		PinFunc[i] = EEPROM.read(i);
	}
}


PinFunction::~PinFunction()
{
}


byte PinFunction::getPinFunc(byte Pin)
{
	if (Pin>=0 && Pin <=7)
		return PinFunc[Pin];
}


void PinFunction::setPinFunc(byte Pin, byte Func)
{

}


String PinFunction::getAllFunc()
{
	
	return String();
}


byte PinFunction::getNumberOfPins()
{
	return Functions;
}
