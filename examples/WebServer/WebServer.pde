/*
WebServer.pde - Web server Arduino processing sketch

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

byte ip[] = { 192,168,0,100 };

WIFI_PROFILE w_prof = { 1, 0, "12345678", "diysandbox" };

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
Server server(80);

void setup()
{
  // setup LEDs
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);

  // start the connection and the server:
  Wireless.begin(ip, &w_prof);

  //delay(5000);
  
  //server.begin();

  digitalWrite(5, LOW);
}

void loop()
{
  Wireless.process();

  //delay(1000);
}

