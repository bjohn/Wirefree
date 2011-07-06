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

uint8_t Wirefree::_state[MAX_SOCK_NUM] = {
		0, 0, 0, 0 };
uint16_t Wirefree::_server_port[MAX_SOCK_NUM] = {
		0, 0, 0, 0 };

void Wirefree::begin(uint8_t *ip, WIFI_PROFILE* w_prof)
{
	uint8_t gateway[4];
	gateway[0] = ip[0];
	gateway[1] = ip[1];
	gateway[2] = ip[2];
	gateway[3] = 1;
	begin(ip, gateway, w_prof);
}

void Wirefree::begin(uint8_t *ip, uint8_t *gateway, WIFI_PROFILE* w_prof)
{
	uint8_t subnet[] = { 255, 255, 255, 0 };
	begin(ip, gateway, subnet, w_prof);
}

void Wirefree::begin(uint8_t *ip, uint8_t *gateway, uint8_t *subnet, WIFI_PROFILE* w_prof)
{
	//GS.setIPAddress(ip);
	//GS.setGatewayIp(gateway);
	//GS.setSubnetMask(subnet);

	// initialize device
	if (!GS.init()) {
		return;
	}

	// configure params
	GS.configure((GS_PROFILE*)w_prof);

	// initiate connection
	while (!GS.connect());

	// DEBUG
	GS.listen(80);
}

void Wirefree::process()
{
	GS.process();
}

Wirefree Wireless;

