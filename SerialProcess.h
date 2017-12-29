#pragma once
#include <Arduino.h>

class SerialOut
{
public:
	SerialOut();
	~SerialOut();
	String CommandToSend;
	String ValueToSend;
	void SendCommand();
};

class SerialIn
{
public:
	SerialIn();
	~SerialIn();
	void AddChar(char ReadChar);
	void Update();
	void Reset();


	//void Initial();

	int CmdValue = 0;

	int varValueInt = 0;
	String varValueString;
	int varLength = 0;

		
private:
	
	int ndx;

	const byte InputBufferSize = 32;
	//char InputBuffer[32];
	char *InputBuffer;
	
	//char Vbuffer[32];

	boolean isCmdRecCompleted = false;
	boolean isCmdValueUpdated = false;
	
public:
	bool GetIsCmdRecCompleted();
	bool GetIsCmdValueUpdated();
};

