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


void ioif_read() {
  if (IoIf.available()) {

    char input = IoIf.read();

    if (input == '\n' || input == '\r') {
      ioif_read_buffer.trim();
      if (ioif_read_buffer.length()) {
        ioif_handle(ioif_read_buffer); // handle commands from IO Interface
      }
      ioif_read_buffer = "";  // reset

    } else if (input == '\0') {
      // ignore null char

    } else {
      // just add the char
      ioif_read_buffer += input;
    }

  }

}


void ioif_handle (String &message) {
  String command[10];
  getValues(message, ':', command);

  // Send IBus Messages
  if (command[0] == F("TX")) {
    ibus_send_hex(command[1]);
    goto finish;
  }

  // Alive Message
  if (command[0] == F("PING")) {
    ioif_send_command(F("PONG"));
    goto finish;
  }


  // Unknown
  ioif_send_command("UKN:" + message);

finish:
  ;
}


void ioif_send_command(String message) {
  IoIf.println(message);
}
