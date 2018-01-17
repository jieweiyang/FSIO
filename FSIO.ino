#include "SerialProcess.h"
#include "IOControl.h"
#include "LEDControl.h"
#include <SPI.h>


#define pinLEDAnnunciator 6
#define pinInputToggleSwitch 7
#define pinInputENcoder 8
#define pinDigiSegment 9


SerialOut CommandOut;
SerialIn CommandIn;
OnOff595 LEDAnnunciator;
LEDControl SegDisplay;
Switch165 ToggleSwitch;
Encoder165 EncoderSwitch;



void checkToggle(Switch165 TargetSwitch)
{

	byte BitMask = 1;
	for (int i = 0; i < TargetSwitch.DeviceCount; i++) {
		BitMask = 1;
		if (TargetSwitch.chgMask[i] != 0)
		{
			for (int j = 0; j < 8; j++)   // Check for bit's that changed
			{

				if ((TargetSwitch.chgMask[i] & BitMask) != 0)         // did this bit change?
				{
					if ((TargetSwitch.curInState[i] & BitMask) == 0)
						CommandOut.SendCommand((i * 8) + j + TargetSwitch.getCSPin() * 100, 1);
					else
						CommandOut.SendCommand((i * 8) + j + TargetSwitch.getCSPin() * 100, 0);
				}

				BitMask = BitMask << 1;
			}
		}
	}

}

void checkEncoder(Encoder165 TargetEncoder)
{
	for (int i = 0; i < TargetEncoder.NumberOfEncoder; i++)
	{

		if (TargetEncoder.encoderState[i] != 0)
		{
			CommandOut.SendCommand(i + TargetEncoder.getCSPin() * 100, TargetEncoder.encoderState[i]);

			CommandOut.SendCommand(i + TargetEncoder.getCSPin() * 100, 0);
		}


	}

}



void setup()
{
	Serial.begin(115200);

	delay(500);

	LEDAnnunciator.begin(pinLEDAnnunciator);
	
	SegDisplay.begin(pinDigiSegment);
	SegDisplay.setDim(8);

	ToggleSwitch.begin(pinInputToggleSwitch);

	EncoderSwitch.begin(pinInputENcoder);


	Serial.println("Setup Completed");

}


void loop()
{

	if (Serial.available() > 0) {
		CommandIn.AddChar(Serial.read());

		if (CommandIn.GetIsCmdRecCompleted())
			CommandIn.Update();

	}

	if (CommandIn.GetIsCmdValueUpdated())
	{
		Serial.println();
		Serial.println("===FSIO===");
		Serial.print("CommandValue:");
		Serial.println(CommandIn.CmdValue);

		Serial.print("VarValue(int):");
		Serial.println(CommandIn.varValueInt);

		Serial.print("VarValue(String):");
		Serial.println(CommandIn.varValueString);

		int CmdType = CommandIn.CmdValue / 100;

		Serial.print("CommandType:");
		Serial.println(CmdType);

		switch (CmdType)
		{

		case pinLEDAnnunciator:
			LEDAnnunciator.setLed(CommandIn.CmdValue, CommandIn.varValueInt);
			break;

		case pinDigiSegment:
			SegDisplay.CmdInput(CommandIn.CmdValue, CommandIn.varValueString);
			break;

		}

		CommandIn.Reset();
	}

	SegDisplay.doFlash();


	ToggleSwitch.updateDB();
	checkToggle(ToggleSwitch);


	EncoderSwitch.updateDB();
	checkEncoder(EncoderSwitch);



}
