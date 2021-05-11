#include <Time.h>
#include <TimeAlarms.h>
#include <Arduino.h>
#include "BG96.h"

#define DebugSerial Serial
#define M1Serial	Serial1
#define PWR_PIN		2
#define STAT_PIN	3

BG96 BG96(M1Serial, DebugSerial, PWR_PIN, STAT_PIN);

void setup() {
	// put your setup code here, to run once:
	M1Serial.begin(115200);
	DebugSerial.begin(115200);
  EcgSerial.begin(9600);
  pinMode(10, INPUT);
  pinMode(11, INPUT);

	/* BG96 Power On Sequence */
	if ( BG96.isPwrON() )
	{
		DebugSerial.println("BG96 Power ON Status");
		if ( BG96.pwrOFF() ) {
			DebugSerial.println("BG96 Power Off Error");
		} else {
			DebugSerial.println("BG96 Power Off Success");
			DebugSerial.println("Module Power ON Sequence Start");
			if ( BG96.pwrON() ) {
				DebugSerial.println("BG96 Power ON Error");
			} else
				DebugSerial.println("BG96 Power ON Success");
		}
	}
	else
	{
		DebugSerial.println("BG96 Power OFF Status");
		if ( BG96.pwrON() ) {
			DebugSerial.println("BG96 Power ON Error");
		} else
			DebugSerial.println("BG96 Power ON Success");
	}

	/* BG96 Module Initialization */
	if (BG96.init()) {
		DebugSerial.println("BG96 Module Error!!!");
	}

	/* BG96 Module Power Saving Mode Disable */
	if (BG96.disablePSM()) {
		DebugSerial.println("BG96 PSM Disable Error!!!");
	}

	/* Network Regsistraiton Check */
	while (BG96.canConnect() != 0) {
		DebugSerial.println("Network not Ready !!!");
		delay(2000);
	}

}

void loop() {
  
  DebugSerial.println("BG96 Module Ready!!!");
  char _IP[] = "166.104.185.207";
  int  _PORT = 8087;
  // char sendBuffer[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  //char sendBuffer2[] = "Hello CodeZoo!!!";
  // char recvBuffer[32];
  // int  recvSize;    //Data size from BG96 module
  char recvBuffer[1024];
  int recvSize;
  String sendMsg = "test";


  if (BG96.actPDP() == 0) {
    DebugSerial.println("BG96 PDP Activation!!!");
  }

  
  if (BG96.socketCreate(1, _IP, _PORT) == 0)
    DebugSerial.println("TCP Socket Create!!!");

  if ( BG96.socketSend(sendMsg.c_str()) == 0 ) {
      DebugSerial.print("[TCP Send] >>  ");
      DebugSerial.println(sendMsg);
  } else {
      DebugSerial.println("Send Fail!!!");
  }

  if (BG96.socketRecv( recvBuffer, sizeof(recvBuffer), &recvSize, 10000 ) == 0) {
      DebugSerial.print(recvBuffer);
      DebugSerial.println(" <<------ Read Data");
      DebugSerial.println(recvSize, DEC);
      DebugSerial.println("Socket Read!!!");
  }

  if (BG96.socketClose() == 0) {
    DebugSerial.println("Socket Close!!!");
  }

  delay(10);

  if (BG96.deActPDP() == 0) {
    DebugSerial.println("BG96 PDP DeActivation!!!");
  }

	delay(10);

}
