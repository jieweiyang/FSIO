#pragma once
#include <Arduino.h>


class OnOff595
{

private:

	byte bitLedCol[8] = { 1,2,4,8,16,32,64,128 };

	byte pinCS;
	byte DeviceCount = 8;
	byte LedStatus[8];  // Maximum 8 pcs of 74HC595.

	//bool isUpdated = false;

public:
	OnOff595();
	~OnOff595();


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
	void begin(int csPin);
};



class Switch165
{
public:
	Switch165();
	~Switch165();

	// Maximum 8 pcs of 74HC165.
	byte DeviceCount = 8;
	byte curInState[8];
	byte chgMask[8];



	void begin(int csPin);
	int getCSPin();
	void updateDB();


private:

	byte preInState[8];
	byte pinCS;

	byte DEBOUNCE_BUFFER_DEPTH = 4;
	byte DebounceBuffer[8 * 4];

};


class Encoder165
{
public:
	Encoder165();
	~Encoder165();

	// Maximum 8 pcs of 74HC165.
	byte DeviceCount = 8;
	byte curInState[8];

	//byte chgMask[8];

	byte NumberOfEncoder = DeviceCount * 8 / 2;
	byte encoderState[8 * 8 / 2];


	void begin(int csPin);
	int getCSPin();
	void updateDB();


private:

	byte preInState[8];
	byte pinCS;

	byte DEBOUNCE_BUFFER_DEPTH = 4;
	byte DebounceBuffer[8 * 4];

};

