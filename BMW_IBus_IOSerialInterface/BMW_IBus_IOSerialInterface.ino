/*
  Copyright (c) 2018 Harry Berlin.  All right reserved.

  avr-ibus@gmx.net

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#define DEBUG   //Uncomment for Debug.

const byte SHUT_DOWN_MINUTES = 3;

const byte READ_BUFFER_SIZE = 64;                  
byte read_buffer[READ_BUFFER_SIZE];
int read_buffer_index = -1;

const int16_t WRITE_BUFFER_SIZE = 300;
byte write_buffer[WRITE_BUFFER_SIZE];
int16_t write_buffer_index = -1;


const byte WAKE_PIN = 13;
#define wake_set(...) digitalWrite(WAKE_PIN, __VA_ARGS__)


String ioif_read_buffer;


#if defined(ARDUINO_AVR_NANO)
  #include <AltSoftSerial.h>
  #define RX_PIN 8
  #define TX_PIN 9
  #define IoIf  Serial
  AltSoftSerial IBus(RX_PIN, TX_PIN); //RX, TX
#elif defined(ARDUINO_AVR_MEGA2560)
  //Mega 2560 specific code
  #define RX_PIN 19
  #define TX_PIN 18
  #define IoIf Serial
  #define IBus Serial1
#else
  #error Unsupported hardware
#endif


#ifdef DEBUG    //Macros are usually in all capital letters.
  #define debug_print(...)    IoIf.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define debug_println(...)  IoIf.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define debug_print(...)     //now defines a blank line
  #define debug_println(...)   //now defines a blank line
#endif

const int IBUS_WRITE_COUNTER = 10;
const unsigned long LAST_TX_MS = 10;
const unsigned long LAST_RX_MS = 10;

unsigned long last_rx;
unsigned long last_tx;


void setup() {
  
  pinMode(WAKE_PIN, OUTPUT);
  wake_set(HIGH);
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  
  IBus.begin(9600, SERIAL_8E1);
  IoIf.begin(38400);

  
  debug_println(F("IBus begins"));

}


void loop() {
    
  ibus_read();
  
  ibus_write();
  
  ioif_read();

  shut_down();

}
