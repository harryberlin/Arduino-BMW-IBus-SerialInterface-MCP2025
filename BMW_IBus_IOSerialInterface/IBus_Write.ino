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


bool ibus_send_hex(const String &message) {

  if ( write_buffer_index + (message.length() / 2) >= WRITE_BUFFER_SIZE )
  {
    if (write_buffer_index < -1) write_buffer_index = -1;

    return false;
  }

  write_buffer_index += 1;
  write_buffer[write_buffer_index] = IBUS_WRITE_COUNTER;
  write_buffer_index += 1;
  write_buffer[write_buffer_index] = hexToDec(message.substring(0, 2));
  write_buffer_index += 1;
  write_buffer[write_buffer_index] = message.length() / 2 - 2;

  for (int i = 4; i < message.length() - 2; i += 2) {
    write_buffer_index += 1;
    write_buffer[write_buffer_index] = hexToDec(message.substring(i, i + 2));
  }

  write_buffer_index += 1; // checksum

  return true;

}


void ibus_write() {

  if (write_buffer_index < 0 ) {
    return;
  }

  byte arr_len = write_buffer[2] + 2;

  //NX: Message
  if (write_buffer[0] < 1) {

    IoIf.print(F("NX:"));
    for (byte i = 1; i < arr_len; i++) {
      IoIf.print(decToHex(write_buffer[i]));
    }
    IoIf.println(decToHex(write_buffer[write_buffer[2] + 2]));
    last_tx = millis();

    cut_buffer();
    return;

  }


  // calc checksum
  write_buffer[write_buffer[2] + 2] = 0x00;
  for (byte i = 1; i < arr_len; i++) {
    write_buffer[write_buffer[2] + 2] ^= write_buffer[i];
  }

  bool write_success = true;


  // Is IBUS free to send ?
  if (!ibus_clear_to_send()) {
    return;
  }

  // Send Message to IBUS
  for (byte i = 1; i <= arr_len; i++) {
    IBus.write(write_buffer[i]);

    // compare written bytes
    last_tx = millis();

    // Wait for Echo
    while (!IBus.available()) {
      if (millis() - last_tx > 30) { //timeout
        write_success = false;
        goto finish;
      }
    }

    // Compare written Byte
    if (write_buffer[i] != byte(IBus.read())) { // WRITE ERROR
      write_success = false;
      break;
    }

  }


  finish:
  last_rx = millis();

  if (write_success) {

    IoIf.print(F("TX:"));
    for (byte i = 1; i <= arr_len; i++) {
      IoIf.print(decToHex(write_buffer[i]));
    }
    IoIf.println();

    cut_buffer();

  } else {
    write_buffer[0] -= 1;
    last_tx = millis() + 50UL; //take a break
  }
}


bool ibus_clear_to_send() {
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


void cut_buffer() {
  int len_to_cut = write_buffer[2] + 3;
  write_buffer_index -= len_to_cut;

  for (int i = 0; i < WRITE_BUFFER_SIZE - len_to_cut; i++) {
    write_buffer[i] = write_buffer[i + len_to_cut];
  }

}
