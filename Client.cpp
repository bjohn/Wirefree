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
#include "socket.h"

//extern "C" {
//  #include "string.h"
//}

#include "WProgram.h"

#include "Wirefree.h"
#include "Client.h"
#include "Server.h"

uint16_t Client::_srcport = 1024;

Client::Client(uint8_t sock) : _sock(sock) {
}

uint8_t Client::status() {
  if (_sock == MAX_SOCK_NUM) return SOCK_STATUS::CLOSED;
  return GS.readSocketStatus(_sock);
}

void Client::write(uint8_t b) {
}

void Client::write(const char *str) {
}

void Client::write(const uint8_t *buf, size_t size) {
}

int Client::available() {
	if (_sock != MAX_SOCK_NUM) {
		if (GS.isDataOnSock(_sock)) {
			return 1;
		} else {
			GS.process();
			return 0;
		}
	}

	return 0;
}

int Client::read() {
  uint8_t b = 0;
  if (!available() || (recv(_sock, &b, 1) != 1))
    return -1;

  return b;
}

int Client::peek() {
  return 0;
}

void Client::flush() {
}

Client::operator bool() {
  return _sock != MAX_SOCK_NUM;
}

uint8_t Client::connected() {
  if (_sock == MAX_SOCK_NUM) return 0;

  //uint8_t s = status();
  //return !(s == SOCK_STATUS::LISTEN || s == SOCK_STATUS::CLOSED ||
  //  (s == SOCK_STATUS::CLOSE_WAIT && !available()));

  return (status() == SOCK_STATUS::ESTABLISHED);
}

void Client::stop() {
  if (_sock == MAX_SOCK_NUM)
    return;

  disconnect(_sock);

  //EthernetClass::_server_port[_sock] = 0;
  _sock = MAX_SOCK_NUM;
}

#if 0
Client::Client(uint8_t *ip, uint16_t port) : _ip(ip), _port(port), _sock(MAX_SOCK_NUM) {
}

uint8_t Client::connect() {
  if (_sock != MAX_SOCK_NUM)
    return 0;

  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    uint8_t s = W5100.readSnSR(i);
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT) {
      _sock = i;
      break;
    }
  }

  if (_sock == MAX_SOCK_NUM)
    return 0;

  _srcport++;
  if (_srcport == 0) _srcport = 1024;
  socket(_sock, SnMR::TCP, _srcport, 0);

  if (!::connect(_sock, _ip, _port)) {
    _sock = MAX_SOCK_NUM;
    return 0;
  }

  while (status() != SnSR::ESTABLISHED) {
    delay(1);
    if (status() == SnSR::CLOSED) {
      _sock = MAX_SOCK_NUM;
      return 0;
    }
  }

  return 1;
}

void Client::write(uint8_t b) {
  if (_sock != MAX_SOCK_NUM)
    send(_sock, &b, 1);
}

void Client::write(const char *str) {
  if (_sock != MAX_SOCK_NUM)
    send(_sock, (const uint8_t *)str, strlen(str));
}

void Client::write(const uint8_t *buf, size_t size) {
  if (_sock != MAX_SOCK_NUM)
    send(_sock, buf, size);
}

int Client::available() {
  if (_sock != MAX_SOCK_NUM)
    return W5100.getRXReceivedSize(_sock);
  return 0;
}

int Client::read() {
  uint8_t b;
  if (!available())
    return -1;
  recv(_sock, &b, 1);
  return b;
}

int Client::peek() {
  uint8_t b;
  if (!available())
    return -1;
  ::peek(_sock, &b);
  return b;
}

void Client::flush() {
  while (available())
    read();
}

void Client::stop() {
  if (_sock == MAX_SOCK_NUM)
    return;

  // attempt to close the connection gracefully (send a FIN to other side)
  disconnect(_sock);
  unsigned long start = millis();

  // wait a second for the connection to close
  while (status() != SnSR::CLOSED && millis() - start < 1000)
    delay(1);

  // if it hasn't closed, close it forcefully
  if (status() != SnSR::CLOSED)
    close(_sock);

  EthernetClass::_server_port[_sock] = 0;
  _sock = MAX_SOCK_NUM;
}

uint8_t Client::connected() {
  if (_sock == MAX_SOCK_NUM) return 0;
  
  uint8_t s = status();
  return !(s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::FIN_WAIT ||
    (s == SnSR::CLOSE_WAIT && !available()));
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

#endif

