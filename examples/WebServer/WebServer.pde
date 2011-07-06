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

WIFI_PROFILE w_prof = { "diysandbox",  /* SSID */
                        "12345678"    /* WPA/WPA2 passphrase */
                      };

// port 80 is default for HTTP
Server server(80);

void parseRxData(String data)
{
  //Client client = server.available();
  //if (client) {
  //}
}

void setup()
{
  // setup LEDs
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);

  // connect to AP
  Wireless.begin(&w_prof, &parseRxData);
  
  server.begin();
  
  // turn on green LED
  digitalWrite(5, LOW);
  
  delay(1000);
}

void loop()
{
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);

  // listen for incoming clients
  Client client = server.available();
  if (client) {
    String httpReq;
    char httpReqType = 0;
    
    digitalWrite(5, LOW);
    
    while (client.connected()) {
      if (client.available()) {
        int c;
        char b;
      
        digitalWrite(6, LOW);
        
        if ((c = client.read()) != -1) {
          b = c;
          httpReq += b;
        } else {
          break;
        }
        
        if (httpReq == "GET") {
          digitalWrite(3, LOW);
            
          httpReqType = 1;
          
          while ( (c = client.read()) != -1);
          
          break;
        }
      }
    }
    
    digitalWrite(6, HIGH);
    
    // close the connection
    client.stop();
  }
}

#if 0
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          digitalWrite(5, HIGH);
          digitalWrite(3, LOW);

          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(analogRead(analogChannel));
            client.println("<br />");
          }

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }

      }
    }
    // give the web browser time to receive the data
    delay(1000);
    
    // close the connection:
    client.stop();
  }

}

#endif

