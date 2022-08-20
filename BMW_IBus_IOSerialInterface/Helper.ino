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


int hexToDec(String hexString) {

  int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}


String decToHex(const byte decValue) {
  
  String hexString = String(decValue, HEX);
  if (hexString.length() < 2) hexString = "0" + hexString;
  hexString.toUpperCase();
  return hexString;
}


void getValues(String input, char separator, String output[])
{
    int index = 0;

    if (input.indexOf(separator) < 1) {
        output[0] = input;
        return;
    }

    for (int i = 0; i < input.length() - 1; i++)
    {
        int sep_pos = input.indexOf(separator,i+1);

        if (index > 0) i+=1;

        output[index] = input.substring(i,sep_pos);
        index+=1;
        i = sep_pos-1;
    }
    return;
}


void shut_down() 
{
  if (millis() - last_rx > (SHUT_DOWN_MINUTES*60000)) {   
    
    wake_set(LOW);
    
    delay(200);
    
    digitalWrite(TX_PIN, LOW);
    
  }
}
