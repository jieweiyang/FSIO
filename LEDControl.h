#pragma once
#include <Arduino.h>

/*Define modes as per max7219.pdf*/

#define Decode_Mode         0x09
#define Intensity_Mode      0x0A
#define Scan_Limit_Mode     0x0B
#define Shut_Down_Mode      0x0C
#define Display_Test_Mode   0x0F
#define Decode_All          0xFF      //it is for displays boards
#define No_Decode           0x00      //it is for LEDs boards

#define All_Digits_On       0x07      //it is for scan limit
#define All_Groups_On       0x07      //it is for scan limit

#define Not_Shut_Down       0x01
#define Shut_Down           0x00

#define Blank_All_Segments  0x0F
#define Blank_All_Leds      0x00

#define Test_Mode           0x01
#define No_Test_Mode        0x00

#define Minus_Sign      0x0A
#define E               0x0B
#define H               0x0C
#define L               0x0D
#define P               0x0E
#define Clear_Display   0x0F

const byte segments[20] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x1F, 0x70, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x00, 0x01, 0x5B, 0x0F, };
const byte CharTable[20] ={ 48   ,49   ,50 ,  51  , 52  , 53   ,54   ,55 ,  56 ,  57  , 65   ,98   ,67  , 100  ,69   ,70 ,  32 ,  45   ,83 ,  116 };
//                          0     1     2     3     4     5     6     7     8     9     A     b     C     d     E     F     blank -     S     t
//Index                     0                                                           10    11    12    13    14    15    16    17    18    19

class LEDControl
{
public:
	LEDControl(int csPin);
	~LEDControl();
private:
	int pinCS;
	int DeviceCount = 8;
	byte DisplayBuffer[64];
	byte FlashState[8];

public:
	void SendCmdToDisplay(byte ChipNumber, int Addr, byte Data);

	void refresh();
	void begin();


	void CmdInput(int cmdValue, String Segments);
	
	void setDim(int Dim);

	void setFlash(byte pos);
	void doFlash();


private:
	byte CharToSegments(byte CharIn);
	void setDP(byte pos, byte isOn);
	void setDigit(int pos, byte digit);

	
};

