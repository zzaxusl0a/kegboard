/*
 * Original code by Mike Cook April 2009
 * Modified for kegbot by C. Niggel June 2011
 * Supports an RFID reader outputing Wiegand code
 * Code is available for re-use, but please keep the attribution
 */
#include "Arduino.h"
#include <HardwareSerial.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <wiring.h>
#include <avr/pgmspace.h>




class Weigand_Card {
 

 
  private:
#define MAX_BITS 100                 // max number of bits 
#define WEIGAND_WAIT_TIME  1000      // time to wait for another weigand pulse. 
#define KB_INTERRUPT_BIT0 3
#define KB_INTERRUPT_BIT1 2
#define KB_READER_BIT0_PIN 20
#define KB_READER_BIT1_PIN 21

static unsigned char databits[MAX_BITS];    // stores all of the data bits
static unsigned char bitCount;              // number of bits currently captured
static unsigned char flagDone;              // goes low when data is currently being captured
static unsigned int weigand_counter;        // countdown until we assume there are no more bits
static unsigned long cardCode;            // decoded facility + card code
static void reader1Zero();
static void reader1One();
  
  public:
  Weigand_Card();
  unsigned long getdata();
};
