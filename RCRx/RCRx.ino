/* YourDuinoStarter Example: nRF24L01 Receive Joystick values

 - WHAT IT DOES: Receives data from another transceiver with
   2 Analog values from a Joystick or 2 Potentiometers
   Displays received values on Serial Monitor
 - SEE the comments after "//" on each line below
 - CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   
 - V1.00 11/26/13
   Based on examples at http://www.bajdi.com/
   Questions: terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include "MynRF24L01.h"
#include "MyRF24.h"
#include "printf.h"

// Jack
#include <Servo.h>
Servo servox;
Servo servoy;
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
// const uint64_t pipe = 0x000705044BLL; // Define the transmit pipe
// const uint64_t pipe = 0x4b04050700LL; // Define the transmit pipe  重要，原來文件定義的TX Address 是反方向的 XD
const uint64_t pipe = 0xd469f52a4bLL; // Define the transmit pipe  重要，原來文件定義的TX Address 是反方向的 XD


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[2];  // 2 element array holding Joystick readings
int led = 4;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  pinMode(led, OUTPUT);   
  Serial.begin(9600);
  delay(1000);
  printf_begin(); // 記得加上此命令，否則 printf & printDetails 都無法顯示
  // Jack
  servox.attach(5); 
  servoy.attach(6); 
  
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  // radio.setChannel(65);
  radio.setChannel(55);
  radio.setAutoAck(false);
  radio.setPayloadSize(32);
  radio.enableDynamicPayloads();
  radio.setRetries(0,0);
  radio.write_register(CONFIG, _BV(EN_CRC) | _BV(CRCO)); 	// 啟動 CRC & CRC 為 2 bytes
  radio.openReadingPipe(1,pipe);
  radio.startListening();;
  
  radio.printDetails();
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  // digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(1000);               // wait for a second
  // digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  // delay(1000);               // wait for a second
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done)
    {
      // Fetch the data payload
      done = radio.read( joystick, sizeof(joystick) );
      Serial.print("X = ");
      Serial.print(joystick[0]);
      Serial.print(" Y = ");      
      Serial.println(joystick[1]);
	  // Jack
	  joystick[0] = map(joystick[0], 0, 1023, 0, 179);
	  joystick[1] = map(joystick[1], 0, 1023, 0, 179);
	  servox.write(joystick[0]);
	  servoy.write(joystick[1]);
    }
  }
  else
  {    
      Serial.println("No radio available");
  }

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********

