/*
WebClient.pde - Web client Arduino processing sketch

Copyright (C) 2011 DIYSandbox LLC

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <Wirefree.h>

WIFI_PROFILE w_prof = { "diysandbox",  /* SSID */
                        "12345678"    /* WPA/WPA2 passphrase */
                      };

void parseRxData(String data)
{
  if (data.startsWith("?TIME:", 1)) {
    // send response
    Wireless.sendResponse(data.substring(7));
  }
}

void setup()
{
  // setup LEDs
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  // connect to AP
  Wireless.begin(&w_prof, &parseRxData);

  // open socket connection
  Wireless.socketOpen("test-server.com", "12345");

  // turn on green LED
  digitalWrite(3, HIGH);
  
  Wireless.sendDeviceID();
}

void loop()
{
  if (Wireless.connected()) {
    Wireless.process();

    delay(1000);

    Wireless.sendDeviceID();
  } else {
    // turn off green LED
    digitalWrite(3, LOW);
    
    // connect to AP
    Wireless.begin(&w_prof, &parseRxData);

    // open socket connection
    Wireless.socketOpen("test-server.com", "12345");

    // turn on green LED
    digitalWrite(3, HIGH);
    
    Wireless.sendDeviceID();
  }
}

