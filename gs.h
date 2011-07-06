/*
gs.cpp - HAL driver to talk with Gainspan GS1011 WiFi module

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

#ifndef	_gs_h_
#define	_gs_h_

#include <avr/pgmspace.h>
#include <WString.h>

#define MAX_SOCK_NUM 16

typedef enum
{
  WF_OK = 0,
  WF_CONNECT_ERROR = 1,
  WF_PASSPHRASE_ERROR = 2,
  WF_UNKNOWN = 255
} tWFStatus;

#define CMD_DISABLE_ECHO 0
#define CMD_SET_WPA_PSK  1
#define CMD_SET_SSID     2
#define CMD_DISABLE_DHCP 3
#define CMD_ENABLE_DHCP  4
#define CMD_DISCONNECT   5
#define CMD_LISTEN       6

#define HTTP_REQ_NONE     0
#define HTTP_REQ_GET_ROOT 1
#define HTTP_REQ_UNKNOWN  2

typedef struct _GS_PROFILE {
	int dhcp_state;
	int mode;
	String security_key;
	String ssid;
} GS_PROFILE;

class GSClass {
public:
	uint8_t init();
	void configure(GS_PROFILE* prof);
	uint8_t connect();
	uint8_t listen(uint16_t port);
	uint8_t socket_status(uint8_t);
	void process();

private:
	uint8_t dhcp_state;
	uint8_t mode;
	String security_key;
	String ssid;

	uint8_t serv_cid;
	uint8_t client_cid;

	uint8_t dev_mode;

	uint8_t http_req_type;

	String readline(void);
	uint8_t send_cmd(uint8_t cmd);
	uint8_t parse_resp(uint8_t cmd);
	uint8_t send_cmd_w_resp(uint8_t cmd);
	void parse_cmd(String buf);
	void parse_data(String buf);
	void send_http_resp();
};

extern GSClass GS;

#endif // _gs_h_

