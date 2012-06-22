/*
 * Original code by Mike Cook April 2009
 * Modified for kegbot by C. Niggel June 2011
 * Supports an RFID reader outputing Wiegand code
 * Code is available for re-use, but please keep the attribution
 */

#include "weigand.h"
#include <interrupt.h>

unsigned char Weigand_Card::databits[MAX_BITS];    // stores all of the data bits
unsigned char Weigand_Card::bitCount;              // number of bits currently captured
unsigned char Weigand_Card::flagDone;              // goes low when data is currently being captured
unsigned int Weigand_Card::weigand_counter;        // countdown until we assume there are no more bits
 unsigned long Weigand_Card::cardCode;   

Weigand_Card::Weigand_Card() {
  //Serial.begin(57600);
  // Attach pin change interrupt service routines from the Wiegand RFID readers
  Serial.println("in weigandinit");
  
  //initialize the reader pins
  pinMode(KB_READER_BIT0_PIN, INPUT);
  pinMode(KB_READER_BIT1_PIN, INPUT);
  //attach interrupts
  attachInterrupt(KB_INTERRUPT_BIT1, reader1One, FALLING);
  attachInterrupt(KB_INTERRUPT_BIT0, reader1Zero, FALLING);
  

  weigand_counter = WEIGAND_WAIT_TIME; 

  //digitalWrite(13, HIGH);  // show Arduino has finished initilisation
  
}

unsigned long Weigand_Card::getdata() {
  //Serial.println("in getdata");
  //intitalize output variable
  unsigned long lngCardData = 0;
 // This waits to make sure that there have been no more data pulses before processing data
  if (!flagDone) {
    if (--weigand_counter == 0)
      flagDone = 1;	
  }
  
  // if we have bits and we the weigand counter went out
  if (bitCount > 0 && flagDone) {
    unsigned char i;
    
    Serial.print("Read ");
    Serial.print(bitCount);
    Serial.print(" bits. ");
    
    // we will decode the bits differently depending on how many bits we have
    // see www.pagemac.com/azure/data_formats.php for mor info
    if (bitCount == 35)
    {
      // 35 bit HID Corporate 1000 format
      // facility code = bits 2 to 14, card code bits 15 to 34
      for (i=2; i<34; i++)
      {
         lngCardData <<=1;
         lngCardData |= databits[i];
      }
    }
    else if (bitCount == 26)
    {
      // standard 26 bit format
      // facility code = bits 2 to 9, card code = bits 10 to 23
      for (i=1; i<25; i++)
      {
         lngCardData <<=1;
         lngCardData |= databits[i];
      }
    }
    else if (bitCount == 37)
    {
      // standard 37 bit format
      // facility code = bits 2 to 13, card code = bits 14 to 36
      for (i=1; i<36; i++)
      {
         lngCardData <<=1;
         lngCardData |= databits[i];
      }  
    }
    else {
      // you can add other formats if you want!
     //Serial.println("Unable to decode.");
     lngCardData = 0;
    }

     // cleanup and get ready for the next card
     bitCount = 0;
     for (i=0; i<MAX_BITS; i++) 
     {
       databits[i] = 0;
     }
  }
  return lngCardData;
}

void Weigand_Card::reader1One() {
  Serial.print("1");
  databits[bitCount] = 1;
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
}

void Weigand_Card::reader1Zero() {
  Serial.print("0");
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
  
}

