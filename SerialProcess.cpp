#include "SerialProcess.h"
#include <Arduino.h>



SerialOut::SerialOut()
{
}


SerialOut::~SerialOut()
{
}


void SerialOut::SendCommand()
{
	Serial.println(CommandToSend + String("=") + ValueToSend);
}


SerialIn::SerialIn()
{
	ndx = 0;
	InputBuffer = 0;
	InputBuffer = new char[InputBufferSize];

}


SerialIn::~SerialIn()
{
}

void SerialIn::AddChar(char ReadChar)
{
	InputBuffer[ndx] = ReadChar;
	ndx++;

	if (ReadChar == '\n') {
		isCmdRecCompleted = true;

	}

	if (ndx > InputBufferSize) ndx = InputBufferSize;

}

void SerialIn::Update()
{
	char *varName = new char[5];
	char *varValue = new char[8];

	byte varIndex = 0;
	byte InputBufferIndex = 0;

	//char digit;
	
	//Everything before '='sign.
	do
	{
		varName[varIndex] = InputBuffer[InputBufferIndex];
		varIndex++;
		InputBufferIndex++;


		// To avoid crash if input is wrong.
		if ((InputBufferIndex > InputBufferSize) 
			|| (varIndex > 5))
		{
			Reset();
			return;
		}

		
	} while (InputBuffer[InputBufferIndex] != '=');
	varName[varIndex] = 0;

	CmdValue = atoi(varName);
	
	InputBufferIndex++; // Skip the '='
	varIndex = 0;

	do
	{
		// Everything after the '=' sign
		varValue[varIndex] = InputBuffer[InputBufferIndex];
		varIndex++;
		InputBufferIndex++;
		
		// To avoid crash if input is wrong.
		if ((InputBufferIndex > InputBufferSize)
			|| (varIndex > 8))
		{
			Reset();
			return;
		}

	} while (InputBuffer[InputBufferIndex] != '\n');
	varValue[varIndex] = 0;

	varValueInt = atoi(varValue);
	varValueString = String(varValue);
	varLength = varIndex - 1;

	if (CmdValue > 0) {
		isCmdValueUpdated = true;
	}
	else {
		Reset();
	}
	
	if (varName) delete[] varName;
	if (varValue) delete[] varValue;
}

void SerialIn::Reset()
{
	ndx = 0;
	isCmdRecCompleted = false;
	isCmdValueUpdated = false;
}



bool SerialIn::GetIsCmdRecCompleted()
{
	return isCmdRecCompleted;
}


bool SerialIn::GetIsCmdValueUpdated()
{
	return isCmdValueUpdated;
}
