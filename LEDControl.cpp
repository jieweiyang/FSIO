#include "LEDControl.h"
#include <Arduino.h>
#include <SPI.h>



LEDControl::LEDControl()
{

}

LEDControl::~LEDControl()
{
}


void LEDControl::SendCmdToDisplay(byte ChipNumber, int Addr, byte Data)
{
	int i;
	digitalWrite(pinCS, LOW);    //Select the display for SPI_Out

	for (i = DeviceCount - 1; i >= 0; i--)
	{
		if (ChipNumber == i)
		{
			SPI.transfer(Addr); SPI.transfer(Data); // Write address + Data to selected chip
		}
		else
		{
			SPI.transfer(0); SPI.transfer(0);       // Write NOP to all other chips in chain.
		}
	}

	digitalWrite(pinCS, HIGH); // pinCS=1;     // Ready, deselect

}

void LEDControl::begin(int csPin)
{

	SPI.begin();
	pinCS = csPin;
	pinMode(pinCS, OUTPUT);
	digitalWrite(pinCS, HIGH);

	byte i;

	for (i = 0; i < DeviceCount; i++)
	{
		SendCmdToDisplay(i, Display_Test_Mode, No_Test_Mode);
		SendCmdToDisplay(i, Decode_Mode, No_Decode);
		SendCmdToDisplay(i, Intensity_Mode, 0);
		SendCmdToDisplay(i, Scan_Limit_Mode, All_Groups_On);
		SendCmdToDisplay(i, Shut_Down_Mode, Not_Shut_Down);
	}

	for (i = 0; i < 64; i++)
	{
		DisplayBuffer[i] = 0;
		FlashBuffer[i] = 0;
	}

	refresh();
}

void LEDControl::setDim(int Dim)
{
	if (Dim < 0 || Dim >15) return;

	for (int i = 0; i < DeviceCount; i++)
	{
		SendCmdToDisplay(i, Intensity_Mode, Dim);
	}
}

void LEDControl::setFlashInterval(int interval)
{
	FlashInterval = interval;
}


void LEDControl::CmdInput(int cmdValue, String Segments)
{
	//cmdValue = 9xx
	//Segments = Strings.

	byte addr = ((cmdValue % 100) / 10);
	byte posStart = (cmdValue % 10);

	byte StringLength = Segments.length() - 1;

	if ((cmdValue % 100) == 99)  // 99 to be dim
	{
		setDim(Segments.toInt());
	}


	if ((cmdValue % 100) == 91)  // 91 to be flash on
	{
		setFlash(Segments.toInt() - 1, 1);
	}

	if ((cmdValue % 100) == 90)  // 90 to be flash off
	{
		setFlash(Segments.toInt() - 1, 0);
	}




	// String should not more then 8 digit per MAX.
	if (StringLength > 8) return;

	// Address should not exceed count of MAX.
	if (addr > DeviceCount) return;

	// Start position should not after 8.
	if (posStart > 8) return;

	// Input string length should not exceed total digits sum by all MAX.
	if (((addr - 1) * 8 + (posStart - 1) + StringLength) > (8 * DeviceCount)) return;



	char CharString[8];
	Segments.toCharArray(CharString, StringLength + 1);

	byte CharStringIndex = 0;

	byte dpos = ((addr - 1) * 8 + (posStart - 1));

	for (int i = 0;i < StringLength;i++)
	{

		setDigit(dpos - i, CharString[CharStringIndex]);

		if (StringLength - i > 1) // must be two chars with DP.
		{
			if ((CharString[CharStringIndex + 1]) == '.')
			{
				setDP(dpos - i, 1);

				// Skip one char for '.'
				CharStringIndex++;

				// Shorten StringLength as Digit+DP = 1 char.
				StringLength--;
			}
			else
				setDP(dpos - i, 0);
		}
		CharStringIndex++;

	}


	refresh();

}

void LEDControl::setDigit(int pos, byte digit)
{

	/*
	Serial.println("===LED::SetDigit===");
	Serial.print("Pos(0 Based):");
	Serial.print(pos, DEC);
	Serial.print("=");
	Serial.println(digit, DEC);
	Serial.print("ConvertSegment=");
	Serial.println(CharToSegments(digit),BIN);
	*/
	//Need to update Display Buffer


	//byte DPStatus = DisplayBuffer[pos] >> 8;

	if (FlashBuffer[pos] == DisplayBuffer[pos] && DisplayBuffer[pos] != 0)
		FlashBuffer[pos] = CharToSegments(digit);

	DisplayBuffer[pos] = CharToSegments(digit);



	//setDP(pos, DPStatus);

	/*
	Serial.print("BufferStatus(DG)=");
	Serial.println(DisplayBuffer[pos], BIN);
	*/
}

void LEDControl::setDP(byte pos, byte isOn)
{


	//Need to update Display Buffer

	byte DPStatus = 1 << 7;


	if (isOn == 1)
		DisplayBuffer[pos] |= DPStatus;
	else
		DisplayBuffer[pos] &= (~(DPStatus));

}

void LEDControl::refresh()
{
	byte Maxim, Digit;
	for (Digit = 0; Digit < 8; Digit++) // Loop 8 times for every digit position
	{
		digitalWrite(pinCS, LOW);
		// Send to all Maxims, 0 last! (all Maxims receive command for DIGIT n)
		for (Maxim = DeviceCount; Maxim > 0; Maxim--)
		{

			//SPI.transfer(Digit + 1);              // Address, standard match with Max Pin.
			SPI.transfer(8 - Digit);              // Address, Backward match with Left hand order 0.
			SPI.transfer(DisplayBuffer[((Maxim - 1) * 8) + Digit]); // Data
		}
		delay(2);
		digitalWrite(pinCS, HIGH);
	}
}

byte LEDControl::CharToSegments(byte CharIn)
{
	for (int i = 0;i < 20;i++)
	{

		if (CharIn == CharTable[i]) return segments[i];
	}
	return segments[16];

}


void LEDControl::setFlash(byte pos, byte isOn)
{
	if (isOn == 1)
		FlashBuffer[pos] = DisplayBuffer[pos];
	else
	{
		DisplayBuffer[pos] = FlashBuffer[pos];
		FlashBuffer[pos] = 0;
	}

}


void LEDControl::doFlash()
{

	if ((millis() - FlashMillis) > FlashInterval)
	{
		FlashMillis = millis();
		for (int i = 0; i < 64; i++)
		{
			if (FlashBuffer[i] != 0)
			{
				if (DisplayBuffer[i] == FlashBuffer[i])
					DisplayBuffer[i] = 0;
				else
					DisplayBuffer[i] = FlashBuffer[i];
			}

		}

		refresh();

	}

}


