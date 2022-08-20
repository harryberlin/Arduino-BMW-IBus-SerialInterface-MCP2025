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


void ibus_read() {
  if (IBus.available()) {
    read_buffer_index += 1;
    read_buffer[read_buffer_index] = IBus.read();

    last_rx = millis();
    ibus_msg_get();

  }

}


void ibus_msg_get() {

  if (read_buffer_index < 4) {
    return; // index to small
  }

  byte Len = read_buffer[1];

  if (Len < 3 || Len > 37) {
    cut_read_buffer(1); // Length is out of range
    return;
  }

  if (read_buffer_index < Len + 1) {
    return; // Buffer to small
  }

  if (ibus_msg_checksum(Len + 2) == 0x00) {
    //GOOD PACKET

    // Data
    byte data[40];
    for (int i = 0; i < Len - 2; i++) {
      data[i] = read_buffer[i + 3];
    }

    ibus_msg_handle(read_buffer[0], Len, read_buffer[2], data, read_buffer[Len + 1]);
    cut_read_buffer(Len + 2);

  } else {
    // BAD PACKET
    cut_read_buffer(1);
  }

}


byte ibus_msg_checksum(int msg_len) {
  byte cksum = read_buffer[0];
  for (int i = 1; i <= msg_len - 1; i++) {
    cksum ^= read_buffer[i];
  }
  return cksum;
}


void cut_read_buffer(int len_to_cut) {
  for (int i = 0; i < read_buffer_index + 1 - len_to_cut; i++) {
    read_buffer[i] = read_buffer[i + len_to_cut];
  }

  // maybe we can skip this??
  for (int i = read_buffer_index - len_to_cut + 1; i < 64; i++) {
    read_buffer[i] = 0x00;
  }

  read_buffer_index = read_buffer_index - len_to_cut;

}
