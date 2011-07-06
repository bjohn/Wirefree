/*
Wirefree.h - interface class to talk with DIYSandbox Arduino devices 

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

#ifndef _wirefree_h_
#define _wirefree_h_

#include "Client.h"
#include "Server.h"

#define MAX_SOCK_NUM 16

typedef struct _WIFI_PROFILE {
	int dhcp_state;
	int mode;
	String security_key;
	String ssid;
} WIFI_PROFILE;

class Wirefree {
private:
public:
  static uint8_t _state[MAX_SOCK_NUM];
  static uint16_t _server_port[MAX_SOCK_NUM];
  void begin(uint8_t *, WIFI_PROFILE*);
  void begin(uint8_t *, uint8_t *, WIFI_PROFILE*);
  void begin(uint8_t *, uint8_t *, uint8_t *, WIFI_PROFILE*);

  void process();

  friend class Client;
  friend class Server;
};

extern Wirefree Wireless;

#endif // _wirefree_h_

