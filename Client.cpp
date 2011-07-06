/*
Client.cpp - network client class 

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

#include "gs.h"
//#include "socket.h"

extern "C" {
  #include "string.h"
}

#include "WProgram.h"

#include "Wirefree.h"
#include "Client.h"
#include "Server.h"

uint16_t Client::_srcport = 1024;

Client::Client(uint8_t sock) : _sock(sock) {
}

Client::Client(uint8_t *ip, uint16_t port) : _ip(ip), _port(port), _sock(MAX_SOCK_NUM) {
}

uint8_t Client::connect() {
  return 1;
}

void Client::write(uint8_t b) {
}

void Client::write(const char *str) {
}

void Client::write(const uint8_t *buf, size_t size) {
}

int Client::available() {
  return 0;
}

int Client::read() {
	return 0;
}

int Client::peek() {
  return 0;
}

void Client::flush() {
}

void Client::stop() {
}

uint8_t Client::connected() {
	return 0;
}

uint8_t Client::status() {
	if (_sock == MAX_SOCK_NUM) return 0;
	return GS.socket_status(_sock);
}

// the next three functions are a hack so we can compare the client returned
// by Server::available() to null, or use it as the condition in an
// if-statement.  this lets us stay compatible with the Processing network
// library.

uint8_t Client::operator==(int p) {
  return _sock == MAX_SOCK_NUM;
}

uint8_t Client::operator!=(int p) {
  return _sock != MAX_SOCK_NUM;
}

Client::operator bool() {
  return _sock != MAX_SOCK_NUM;
}

