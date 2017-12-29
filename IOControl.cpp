#include "IOControl.h"
#include <SPI.h>



ShiftRegister595::ShiftRegister595(int csPin)
{
	SPI.begin();
	pinCS = csPin;
	pinMode(pinCS, OUTPUT);
	digitalWrite(pinCS, HIGH);

}

ShiftRegister595::~ShiftRegister595()
{
}

void ShiftRegister595::setLed(int addr, int pos, int isOn)
{

	if (addr<0 || addr >(DeviceCount - 1))
		return;
	if (pos < 0 || pos >7)
		return;

	if (isOn == 1)
		LedStatus[addr] |= bitLedCol[pos];
	else
		LedStatus[addr] &= ~(bitLedCol[pos]);

	refresh();
}

void ShiftRegister595::setLed(int cmd, int isOn)
{
	byte addr = ((cmd % 100) / 10 ) ;
	byte pos = (cmd % 10) ;

	if (addr ==9 && pos ==9)
	{
		testAll();
		return;
	}

	if (addr<0 || addr >(DeviceCount - 1)) 
		return;
	if (pos < 0 || pos >7) 
		return;

	if (isOn == 1)
		LedStatus[addr] |= bitLedCol[pos];
	else
		LedStatus[addr] &= ~(bitLedCol[pos]);

	
	refresh();
}

void ShiftRegister595::refresh()
{

	digitalWrite(pinCS, LOW);
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

	for (int i = DeviceCount;i>=0  ;i--)
	{
		SPI.transfer(LedStatus[i]);
		//Serial.println(LedStatus[i], BIN);
	}

	SPI.endTransaction();
	digitalWrite(pinCS, HIGH);
	//Serial.println("595 Refresh Completed");

}

void ShiftRegister595::testAll()
{

	digitalWrite(pinCS, LOW);
	//SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

	for (int i = DeviceCount;i >= 0;i--)
	{
		SPI.transfer(B11111111);
	}

	SPI.endTransaction();
	digitalWrite(pinCS, HIGH);

}


void ShiftRegister595::begin()
{
	for (int i = 0;i < DeviceCount;i++)
		LedStatus[i] = 0;

	testAll();
	delay(2000);
	refresh();
}

