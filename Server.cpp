/*
Server.cpp - network server class 

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

extern "C" {
#include "string.h"
}

#include "Wirefree.h"
#include "Client.h"
#include "Server.h"
#include "socket.h"

Server::Server(uint16_t port)
{
  _port = port;
}

void Server::begin()
{
	for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
		Client client(sock);
		if (client.status() == 0) {
			Sock.listen(_port);
			Wirefree::_server_port[sock] = _port;
			break;
		}
	}
}

void Server::accept()
{
}

Client Server::available()
{
	for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
		Client client(sock);
		if (Wirefree::_server_port[sock] == _port &&
				(client.status() == 2)) {
			if (client.available()) {
				// XXX: don't always pick the lowest numbered socket.
				return client;
			}
		}
	}

	return Client(MAX_SOCK_NUM);
}

void Server::write(uint8_t b) 
{
}

void Server::write(const char *str) 
{
}

void Server::write(const uint8_t *buffer, size_t size) 
{
}

