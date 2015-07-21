#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10 
#define _dc 9
#define _rst 8

// Use hardware SPI
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

#define CHANNELS 8
#define TEXTSIZE 3
#define BAUD 115200
#define LED  13
#define PACKETSIZE  18 // channel * 2 + STARTBYTE + STOPBYTE
#define STARTBYTE  0x7B // {
#define STOPBYTE   0x7D // }

#define default_servo_value 1000  //set the default servo value
#define PPM_FrLen 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 2  //set PPM signal output pin on the arduino


typedef struct PACKET{
int16_t channels[CHANNELS];
int checksum;
char startByte;
char stopByte;
};

PACKET packet;
char input[2];

void setup() {
  // put your setup code here, to run once:

  setToDefault();

  // pinMode(sigPin, OUTPUT);
  // digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  Serial.begin(BAUD);
  // cli();
  // TCCR1A = 0; // set entire TCCR1 register to 0
  // TCCR1B = 0;
  
  // OCR1A = 100;  // compare match register, change this
  // TCCR1B |= (1 << WGM12);  // turn on CTC mode
  // TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  // TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  // sei();
  
  char buffer[CHANNELS + 2];
  pinMode(LED,OUTPUT);
   tft.begin();
   delay(1000);
  tft.fillScreen(ILI9340_BLACK);

  // set initial channel values to zero
  for (int i = 0; i < CHANNELS; i++)
  {
    displayChannel(i,0);
  }
}

static int noData = 0;

void loop() {
  // testLoop();
  // put your main code here, to run repeatedly:
  if (Serial.available() >= PACKETSIZE)
  {
    packet.startByte = Serial.read();
    if (packet.startByte == STARTBYTE) // check for start of packet
    {  // read the data
      for (int i = 0; i < CHANNELS; ++i)
      {
        packet.channels[i] = readInt16();
      }
      // implement checksum later
      packet.stopByte = Serial.read(); // read stopbyte
      if (packet.stopByte != STOPBYTE) // check for complete packet
        ; // throw away the packet and reset the channels if packet is incomplete
      else // data is good, reset the counter
        noData = 0;
    }
    else // just flush the buffer and wait for clean data
      while(Serial.read() != -1);
  }

  else // if no data is received
  {
    noData++; // if this number gets above a certain limit, meaning no data received for a given period, reset to default values.
  }
  
  for (int i = 0; i < CHANNELS; ++i)
  {
  	updateChannel(i,packet.channels[i]);
  }

}

void setToDefault()
{
  for (int i = 0; i < CHANNELS; ++i)
  {
    packet.channels[i] = default_servo_value;
  }
}

void displayChannel(unsigned int channel, int value)
{
  tft.setTextSize(TEXTSIZE);
  tft.setTextColor(ILI9340_RED,ILI9340_BLACK);
  tft.setCursor(0,channel*TEXTSIZE*10);
  tft.print("Ch ");
  tft.print(channel+1);
  tft.print(" = ");
  tft.println(value);

}

void updateChannel(int channel, int value)
{
	// set the cursor
	tft.setTextColor(ILI9340_RED,ILI9340_BLACK);
	tft.setCursor(120,channel*TEXTSIZE*10);
	tft.println(value);
}

/*
*  Alternate main loop to test if we are successfully reading in ints
*/
void testLoop()
{
  int temp;
  displayChannel(1,0);
  while(1)
  {
    if (Serial.available() == 2) // make sure there are at least 2 bytes to read
    {
      temp = readInt16();
      // Serial.println("Attempting to read");
      // Serial.println(temp);
      displayChannel(1,temp);
    }
  }
  // updateChannel(3,temp);
}

int16_t readInt16()
{
  int upper = 0;
  int lower = 0;
  upper = Serial.read();
  lower = Serial.read();
  upper = upper * 256;
  lower += upper;
  return int16_t(lower);
}


// Interrupt routine to generate the outgoing signal
// ISR(TIMER1_COMPA_vect){  //leave this alone
//   static boolean state = true;
  
//   TCNT1 = 0;
  
//   if(state) {  //start pulse
//     digitalWrite(sigPin, onState);
//     OCR1A = PPM_PulseLen * 2;
//     state = false;
//   }
//   else{  //end pulse and calculate when to start the next pulse
//     static byte cur_chan_numb;
//     static unsigned int calc_rest;
  
//     digitalWrite(sigPin, !onState);
//     state = true;

//     if(cur_chan_numb >= CHANNELS){
//       cur_chan_numb = 0;
//       calc_rest = calc_rest + PPM_PulseLen;// 
//       OCR1A = (PPM_FrLen - calc_rest) * 2;
//       calc_rest = 0;
//     }
//     else{
//       OCR1A = (packet.channels[cur_chan_numb] - PPM_PulseLen) * 2;
//       calc_rest = calc_rest + packet.channels[cur_chan_numb];
//       cur_chan_numb++;
//     }     
//   }
// }