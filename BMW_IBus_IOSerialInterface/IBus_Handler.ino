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


void ibus_msg_handle(byte &src, byte &len, byte &dst, byte data[], byte &ck) {
  
  // parse the message
  // Example for ignition state
  if (dst == 0xBF) {
        if (data[0] == 0x11) {
          
          //ign_position_set(data); //void to do something with data
          ioif_send_command(F("HANDLE IGN POSITION"));
          goto print_msg;
        }
      }
  
  // send message to IO Interface
  print_msg:
  IoIf.print(F("RX:"));
  IoIf.print(decToHex(src)); //SRC
    
  IoIf.print(decToHex(len)); //LL
  
  IoIf.print(decToHex(dst)); //DST
  
  for (int i=0; i < len-2; i++) {
      IoIf.print(decToHex(data[i])); //DATA
  }

  IoIf.println(decToHex(ck)); //CK
  
}


