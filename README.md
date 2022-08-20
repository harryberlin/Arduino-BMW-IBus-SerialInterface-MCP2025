## Arduino-BMW-IBus-SerialInterface-MCP2025

Sketch for IBus Communication with MCP2025 through Serialport (Baud 38400)
AltSoftserial is edited for Parity Even
Shut down the MCP2025 when no activity at IBus.

You get these Messages from Arduino:
- RX:IBUSMESSAGE <-- Recieve IBus Message  (example: RX:6805183800004D | CD StateRequest)
- NX:IBUSMESSAGE <-- Not Sent IBus Message  (example: NX:680518380000CK | CD StateRequest)
- PONG           <-- Answer for PING
- UKN:message    <-- returns the unknown message

You can send some few commands to the Arduino:
- TX:IBUSMESSAGE <-- IBus Message without Length or Checksum (example: TX:68LL18380000CK | CD StateRequest)
- PING           <-- The Arduino will answer with PONG

A simple IBus message handle/parse example is included.


## Donation
If you like my project and want to support me, feel free for a donation.<br>
[<img src="https://www.paypalobjects.com/en_US/DK/i/btn/btn_donateCC_LG.gif">](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=KYAHYEJRUK4PN)
