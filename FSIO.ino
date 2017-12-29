#include "SerialProcess.h"
#include "IOControl.h"
#include "LEDControl.h"
#include <SPI.h>


#define pinLEDAnnunciator 6
#define pinInputButton 7
#define pinInputENcoder 8
#define pinDigiSegment 9


SerialOut CommandOut;
SerialIn CommandIn;
ShiftRegister595 LEDAnnunciator(pinLEDAnnunciator);
LEDControl SegDisplay(pinDigiSegment);



void setup()
{
	Serial.begin(115200);
	
	delay(2000);
	LEDAnnunciator.begin();
	SegDisplay.begin();
	SegDisplay.setDim(8);

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
			//LEDAnnunciator.setLed(CommandIn.CmdValue, CommandIn.varValueInt);
			break;

		case pinDigiSegment:
			SegDisplay.CmdInput(CommandIn.CmdValue, CommandIn.varValueString);
			break;
	
		}

		CommandIn.Reset();
	}
	

}
