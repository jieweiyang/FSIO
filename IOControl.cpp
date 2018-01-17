#include "IOControl.h"
#include <SPI.h>



OnOff595::OnOff595()
{


}

OnOff595::~OnOff595()
{
}

void OnOff595::setLed(int addr, int pos, int isOn)
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

void OnOff595::setLed(int cmd, int isOn)
{
	byte addr = ((cmd % 100) / 10);
	byte pos = (cmd % 10);

	if (addr == 9 && pos == 9)
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

void OnOff595::refresh()
{

	digitalWrite(pinCS, LOW);
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

	for (int i = DeviceCount;i >= 0;i--)
	{
		SPI.transfer(LedStatus[i]);
		//Serial.println(LedStatus[i], BIN);
	}

	SPI.endTransaction();
	digitalWrite(pinCS, HIGH);
	//Serial.println("595 Refresh Completed");

}

void OnOff595::testAll()
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

void OnOff595::begin(int csPin)
{
	SPI.begin();
	pinCS = csPin;
	pinMode(pinCS, OUTPUT);
	digitalWrite(pinCS, HIGH);

	for (int i = 0;i < DeviceCount;i++)
		LedStatus[i] = 0;

	testAll();
	delay(2000);
	refresh();
}



Switch165::Switch165()
{
}


Switch165::~Switch165()
{
}




void Switch165::begin(int csPin)
{
	SPI.begin();
	pinCS = csPin;
	pinMode(pinCS, OUTPUT);
	digitalWrite(pinCS, HIGH);

	for (int i = 0;i < DeviceCount;i++)
	{
		curInState[i] = 1;
		preInState[i] = 1;
		chgMask[i] = 0;
	}
}


int Switch165::getCSPin()
{
	return pinCS;;
}


void Switch165::updateDB()
{

	for (int i = 0;i < DeviceCount;i++)
	{
		chgMask[i] = 0;
	}

	//byte BitMask;
	//byte ChgMask = 0;

	int row = 0;
	int col = 0;
	byte ANDresult = 0;
	byte ORresult = 0;

	digitalWrite(pinCS, LOW);

	for (col = 0; col < DeviceCount; col++)
	{
		preInState[col] = curInState[col];
		// PUSH DOWN And Read new to Row 0
		/*
		Data structured in DebounceBuffer[], is  [Byte 1 S(state)0][Byte 2 S0].....[Byte Byte 1 S1][Byte 2 S1]
		Moving data of State 0~3 to State 1~4.
		*/
		for (row = DEBOUNCE_BUFFER_DEPTH - 1; row > 0; row--)
			DebounceBuffer[row * DeviceCount + col] = DebounceBuffer[(row - 1) * DeviceCount + col];

		DebounceBuffer[col] = SPI.transfer(0x00); // ReadSPI(); //ReadSPI() reads a single byte from the SPI

												  // Logig manipulation --------------------------------------------------
		ANDresult = DebounceBuffer[col]; // Start with top row to look for ALL 1
		ORresult = DebounceBuffer[col]; // Start with top row to look for ALL 0
		for (row = 1; row < DEBOUNCE_BUFFER_DEPTH; row++)
		{
			ANDresult &= DebounceBuffer[row * DeviceCount + col];
			ORresult |= DebounceBuffer[row * DeviceCount + col];
		}
		curInState[col] |= ANDresult; // OR the result from AND (Set Bits)
		curInState[col] &= ORresult;  // AND the resukt from OR (Clear Bits)
	}

	digitalWrite(pinCS, HIGH);


	for (int i = 0; i < DeviceCount; i++)
	{
		chgMask[i] = preInState[i] ^ curInState[i];
	}

}



Encoder165::Encoder165()
{




}


Encoder165::~Encoder165()
{
}


void Encoder165::begin(int csPin)
{
	SPI.begin();
	pinCS = csPin;
	pinMode(pinCS, OUTPUT);
	digitalWrite(pinCS, HIGH);

	for (int i = 0;i < DeviceCount;i++)
	{
		curInState[i] = 1;
		preInState[i] = 1;
	}
}


int Encoder165::getCSPin()
{
	return pinCS;;
}


void Encoder165::updateDB()
{

	for (int i = 0;i < NumberOfEncoder;i++)
	{
		encoderState[i] = 0;
	}


	int row = 0;
	int col = 0;
	byte ANDresult = 0;
	byte ORresult = 0;

	digitalWrite(pinCS, LOW);

	for (col = 0; col < DeviceCount; col++)
	{
		preInState[col] = curInState[col];

		// PUSH DOWN And Read new to Row 0
		/*
		Data structured in DebounceBuffer[], is  [Byte 1 S(state)0][Byte 2 S0].....[Byte Byte 1 S1][Byte 2 S1]
		Moving data of State 0~3 to State 1~4.
		*/
		for (row = DEBOUNCE_BUFFER_DEPTH - 1; row > 0; row--)
			DebounceBuffer[row * DeviceCount + col] = DebounceBuffer[(row - 1) * DeviceCount + col];

		DebounceBuffer[col] = SPI.transfer(0x00); // ReadSPI(); //ReadSPI() reads a single byte from the SPI

												  // Logig manipulation --------------------------------------------------
		ANDresult = DebounceBuffer[col]; // Start with top row to look for ALL 1
		ORresult = DebounceBuffer[col]; // Start with top row to look for ALL 0
		for (row = 1; row < DEBOUNCE_BUFFER_DEPTH; row++)
		{
			ANDresult &= DebounceBuffer[row * DeviceCount + col];
			ORresult |= DebounceBuffer[row * DeviceCount + col];
		}
		curInState[col] |= ANDresult; // OR the result from AND (Set Bits)
		curInState[col] &= ORresult;  // AND the resukt from OR (Clear Bits)
	}

	digitalWrite(pinCS, HIGH);

	byte BitMask;
	byte ChgMask = 0;
	byte now, pre;


	for (int i = 0; i < DeviceCount; i++) {
		BitMask = 1;
		ChgMask = preInState[i] ^ curInState[i];
		if (ChgMask != 0)
		{
			for (int j = 0; j < 8; j++)   // Check for bit's that changed
			{

				BitMask = BitMask << 1;
				now = (curInState[i] >> j) & B11; //0x03; Get current two pin state..
				pre = (preInState[i] >> j) & B11; //0x03; Get previous two pin state..

				if (((now ^ pre) & 0X03) != 0) // Did one of the pin change
				{
					if (now == 0 && pre < 3 && pre > 0 )       // Yes, is it middle (AB) position ?
						encoderState[((i * 8) + j) / 2] = pre;
				} // End If - Encoder Changed

				j++;  // Skip next pin as Encoder used two pins.

				BitMask = BitMask << 1;
			}
		}
	}

}




