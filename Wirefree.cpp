/*
Wirefree.cpp - interface class to talk with DIYSandbox Arduino devices 

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

#include "Wirefree.h"
#include "gs.h"

#include <WProgram.h>

uint16_t Wirefree::_server_port[MAX_SOCK_NUM] = {
  0, 0, 0, 0 };

void Wirefree::begin(WIFI_PROFILE* w_prof, void (*rxDataHndlr)(String data))
{
	// initialize device
	if (!GS.init(rxDataHndlr)) {
		return;
	}

	// configure params
	GS.configure((GS_PROFILE*)w_prof);

	// initiate wireless connection
	while (!GS.connect());
}

void Wirefree::process()
{
	GS.process();
}

uint8_t Wirefree::socketOpen(String url, String port)
{
	String ip;

	// get IP address from URL
	if ((ip = GS.dns_lookup(url)) == "0.0.0.0") {
		return 0;
	}

	// open socket connection
	if (!GS.connect_socket("192.168.0.100", "32000")) {
		return 0;
	}

	return 1;
}

uint8_t Wirefree::connected()
{
	return GS.connected();
}

void Wirefree::sendDeviceID()
{
	String dev_id_str = "ID:" + GS.get_dev_id();

	GS.send_data(dev_id_str);
}

void Wirefree::sendResponse(String data)
{
	String resp_str = "TIME:" + data + ":" + String(millis());

	GS.send_data(resp_str);
}

Wirefree Wireless;

