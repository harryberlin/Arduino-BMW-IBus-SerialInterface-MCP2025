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


void ibus_send_hex(String message) 
{  
  write_buffer_index += 1;
  write_buffer[write_buffer_index].msg[0] = WRITE_COUNTER;

  write_buffer[write_buffer_index].msg[1] = hexToDec(message.substring(0,2));
  write_buffer[write_buffer_index].msg[2] = (message.length())/2-2;
  
  byte s = 3;
  for (int i = 4; i < message.length()-2; i+=2) {
    write_buffer[write_buffer_index].msg[s] = hexToDec(message.substring(i,i+2));
    s += 1;
  }    
}


void ibus_write()
{
  if (write_buffer_index < 0 ) { 
    return; 
  }    
    
  byte arr_len = write_buffer[0].msg[2]+2;
  write_buffer[0].msg[write_buffer[0].msg[2]+2] = 0x00;
 
  bool write_success = true;
  
  if (ibus_clear_to_send()) {
    for (byte i = 1; i <= arr_len; i++) {
      IBus.write(write_buffer[0].msg[i]);

      // compare written bytes
      last_tx = millis();
      
      while (!IBus.available()) {   // WAIT FOR ECHO
        if (millis() - last_tx > 20) { //timeout
          write_success = false;
          goto finish;   
        }
      }
      
      if (write_buffer[0].msg[i] != byte(IBus.read())) { // WRITE ERROR
        write_success = false;
        break;
      }

      // calc checksum
      if (i < arr_len) {
        write_buffer[0].msg[write_buffer[0].msg[2]+2] ^= write_buffer[0].msg[i];
      }
      
    }
    
    last_rx = millis();
    finish:
    
    if (write_success) {
      
      IoIf.print(F("TX:"));
      for (byte i = 1; i <= arr_len; i++) {
        IoIf.print(decToHex(write_buffer[0].msg[i]));
      }
      IoIf.println(F(""));
      
      cut_write_buffer();
    } else if (write_buffer[0].msg[0] <= 1) {
      
      IoIf.print(F("NX:"));  // Not Sent
      for (byte i = 1; i < arr_len; i++) {
          IoIf.print(decToHex(write_buffer[0].msg[i]));
        }
      IoIf.println(F("CK"));
      
      cut_write_buffer();
    } else {
      write_buffer[0].msg[0] = 0;
      last_tx = millis() + 30; //take a break
    }
  }
}


bool ibus_clear_to_send() 
{
  unsigned long current = millis();
  if (current - last_tx < LAST_TX_MS ||  current - last_rx < LAST_RX_MS  || IBus.available()) { 
    return false;
  } 
  if (!digitalRead(RX_PIN)) {
    last_rx = millis();  
  } else {
    return true;
  }
}


void cut_write_buffer() 
{
  int len_to_cut = 1;
  write_buffer_index -= len_to_cut;
  // transfer the values
  for (int i=0; i < WRITE_BUFFER_SIZE-len_to_cut; i++) {
      write_buffer[i] = write_buffer[i+len_to_cut];
  }
}


