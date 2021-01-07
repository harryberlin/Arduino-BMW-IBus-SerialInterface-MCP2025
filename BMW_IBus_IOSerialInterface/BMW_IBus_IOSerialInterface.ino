/*
  Copyright (c) 2018 Harry Berlin.  All right reserved.

  debug.ibuscommunicator@gmx.de

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


#define SHUT_DOWN_MINUTES 3

#define READ_BUFFER_SIZE 64
                    
byte read_buffer[READ_BUFFER_SIZE];
int read_buffer_index = -1;

#define WRITE_BUFFER_SIZE 5
#define WRITE_COUNTER 40

struct IBus_Msg_Tx { byte msg[40]; };
IBus_Msg_Tx write_buffer[WRITE_BUFFER_SIZE];
int write_buffer_index = -1;

#define WAKE_PIN 13
#define wake_set(...) digitalWrite(WAKE_PIN, __VA_ARGS__)

String ioif_read_buffer;

#define LAST_TX_MS 10
#define LAST_RX_MS 10

unsigned long last_rx;
unsigned long last_tx;


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
#elif defined(ARDUINO_SAM_DUE)
  //Due specific code
#else
#error Unsupported hardware
#endif

#define DEBUG   //Uncomment for Debug.

#ifdef DEBUG    //Macros are usually in all capital letters.
  #define debug_print(...)    IoIf.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define debug_println(...)  IoIf.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define debug_print(...)     //now defines a blank line
  #define debug_println(...)   //now defines a blank line
#endif

void setup()
{
  
  pinMode(WAKE_PIN, OUTPUT);
  wake_set(HIGH);
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  
  IBus.begin(9600, SERIAL_8E1);
  IoIf.begin(38400);

  while (!IoIf) { 
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  debug_println(F("IBus begins"));

}


void loop() 
{
    
  ibus_read();
  
  ibus_write();
  
  ioif_read();

  shut_down();

}




