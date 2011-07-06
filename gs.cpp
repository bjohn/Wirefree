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

#include <stdio.h>
#include <avr/interrupt.h>
#include <HardwareSerial.h>
#include <WProgram.h>

#include "gs.h"

GSClass GS;

uint8_t GSClass::init(void)
{
	Serial.begin(9600);
	delay(1000);

	Serial.flush();
	Serial.println();
	delay(1000);

	dev_mode = 0;

	// disable echo
	if (!send_cmd_w_resp(CMD_DISABLE_ECHO)) {
		return 0;
	}

	return 1;
}

typedef struct _cmd_tbl {
	String cmd_str;
} CMD_TBL;

CMD_TBL cmd_tbl[] = {
		{"ATE0"},
		{"AT+WWPA="},
		{"AT+WA="},
		{"AT+NDHCP=0"},
		{"AT+NDHCP=1"},
		{"AT+WD"},
		{"AT+NSTCP=80"},
};

uint8_t hex_to_int(char c)
{
    uint8_t val = 0;

    if (c >= '0' && c <= '9') {
        val = c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
        val = c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f') {
        val = c - 'a' + 10;
    }

    return val;
}

char int_to_hex(uint8_t c)
{
    char val = '0';

    if (c >= 0 && c <= 9) {
        val = c + '0';
    }
    else if (c >= 10 && c <= 15) {
        val = c + 'A' - 10;
    }

    return val;
}

uint8_t GSClass::send_cmd(uint8_t cmd)
{
	Serial.flush();

	switch(cmd) {
	case CMD_DISABLE_ECHO:
	case CMD_DISABLE_DHCP:
	case CMD_DISCONNECT:
	case CMD_ENABLE_DHCP:
	case CMD_LISTEN:
	{
		Serial.println(cmd_tbl[cmd].cmd_str);
		break;
	}
	case CMD_SET_WPA_PSK:
	{
		String cmd_buf = cmd_tbl[cmd].cmd_str + this->security_key;
		Serial.println(cmd_buf);
		break;
	}
	case CMD_SET_SSID:
	{
		String cmd_buf = cmd_tbl[cmd].cmd_str + this->ssid;
		Serial.println(cmd_buf);
		break;
	}
	default:
		break;
	}

	return 1;
}

uint8_t GSClass::parse_resp(uint8_t cmd)
{
	uint8_t resp_done = 0;
	uint8_t ret = 0;
	String buf;

	while (!resp_done) {
		buf = readline();

		switch(cmd) {
		case CMD_DISABLE_ECHO:
		case CMD_DISABLE_DHCP:
		case CMD_DISCONNECT:
		case CMD_SET_WPA_PSK:
		case CMD_SET_SSID:
		case CMD_ENABLE_DHCP:
		{
			if (buf == "OK") {
				/* got OK */
				ret = 1;
				resp_done = 1;
			} else if (buf.startsWith("ERROR")) {
				/* got ERROR */
				ret = 0;
				resp_done = 1;
			}
			break;
		}
		case CMD_LISTEN:
		{
			if (buf.startsWith("CONNECT")) {
				/* got CONNECT */
				serv_cid = hex_to_int(buf[8]);
			} else if (buf == "OK") {
				/* got OK */
				ret = 1;
				resp_done = 1;
			} else if (buf.startsWith("ERROR")) {
				/* got ERROR */
				ret = 0;
				resp_done = 1;
			}
			break;
		}
		default:
			break;
		}
	}

	return ret;
}

uint8_t GSClass::send_cmd_w_resp(uint8_t cmd)
{
	if (send_cmd(cmd)) {
		return parse_resp(cmd);
	} else {
		return 0;
	}
}

void GSClass::configure(GS_PROFILE *prof)
{
	// configure params

	// configure DHCP state
	// configure mode
	// configure security
	// configure SSID
	dhcp_state = prof->dhcp_state;
	mode = prof->mode;
	this->ssid = prof->ssid;
	this->security_key = prof->security_key;

}

uint8_t GSClass::connect()
{

	if (!send_cmd_w_resp(CMD_DISCONNECT)) {
		return 0;
	}

	if (!send_cmd_w_resp(CMD_DISABLE_DHCP)) {
		return 0;
	}

	if (!send_cmd_w_resp(CMD_SET_WPA_PSK)) {
		return 0;
	}

	if (!send_cmd_w_resp(CMD_SET_SSID)) {
		return 0;
	}

	if (!send_cmd_w_resp(CMD_ENABLE_DHCP)) {
		return 0;
	}

	return 1;
}

String GSClass::readline(void)
{
	String strBuf;
	char inByte;

	bool endDetected = false;

	while (!endDetected)
	{
		if (Serial.available())
		{
			// valid data in HW UART buffer, so check if it's \r or \n
			// if so, throw away
			// if strBuf length greater than 0, then this is a true end of line,
			// so break out
			inByte = Serial.read();

			if ((inByte == '\r') || (inByte == '\n'))
			{
				// throw away
				if ((strBuf.length() > 0) && (inByte == '\n'))
				{
					endDetected = true;
				}
			}
			else
			{
				strBuf += inByte;
			}
		}
	}

	return strBuf;
}

uint8_t GSClass::listen(uint16_t port)
{
	// FIXME : use the input argument

	if (!send_cmd_w_resp(CMD_LISTEN)) {
		return 0;
	}
}

uint8_t GSClass::socket_status(uint8_t port)
{
	// FIXME : dummy, to be implemented

	return 0;
}

void GSClass::process()
{
	String strBuf;
	char inByte;
	uint8_t processDone = 0;

	if (!Serial.available())
		return;

	while (!processDone) {
		if (dev_mode == 0) {
			while (1) {
				if (Serial.available()) {
					inByte = Serial.read();

					if (inByte == 0x1b) {
						// escape seq

						// switch mode
						dev_mode = 1;
						break;
					} else {
						// command string
						if ((inByte == '\r') || (inByte == '\n')) {
							// throw away
							if ((strBuf.length() > 0) && (inByte == '\n'))
							{
								// parse command
								parse_cmd(strBuf);
								processDone = 1;
								break;
							}
						}
						else
						{
							strBuf += inByte;
						}
					}
				}
			}
		} else if (dev_mode == 1) {
			/* data mode */
			while(1) {
				if (Serial.available()) {
					inByte = Serial.read();

					if (inByte == 0x53) {
						/* data start, switch to data RX mode */
						dev_mode = 2;
						break;
					} else if (inByte == 0x45) {
						/* data end, switch to command mode */
						dev_mode = 0;

						/* send response if required */
						send_http_resp();

						processDone = 1;
						break;
					} else if (inByte == 0x4f) {
						/* data mode ok */
						dev_mode = 0;
						break;
					} else {
						/* unknown */
						dev_mode = 0;
						break;
					}
				}
			}
		} else if (dev_mode == 2) {
			/* data RX mode */
			http_req_type = HTTP_REQ_NONE;

			while (dev_mode == 2) {
				String dataBuf;

				while (1) {
					if (Serial.available()) {
						inByte = Serial.read();
						if (inByte == 0x1b) {
							/* switch mode */
							dev_mode = 1;
							break;
						} else {
							if ((inByte == '\r') || (inByte == '\n')) {
								// throw away
								if ((dataBuf.length() > 0) && (inByte == '\n'))
								{
									// parse data
									parse_data(dataBuf);
									break;
								}
							}
							else
							{
								dataBuf += inByte;
							}
						}
					}
				}
			}
		}
	}
}

void GSClass::parse_cmd(String buf)
{
	if (buf.startsWith("CONNECT")) {
		/* got CONNECT */

		if (serv_cid == hex_to_int(buf[8])) {
			/* client connected */
			client_cid = hex_to_int(buf[10]);
		}
	} else if (buf.startsWith("DISCONNECT")) {
		/* got disconnect */
	}
}

void GSClass::parse_data(String buf)
{
	if (buf.startsWith("GET", 1)) {
		if (buf.startsWith("/ ", 5)) {
			digitalWrite(3, LOW);
			http_req_type = HTTP_REQ_GET_ROOT;
		} else {
			http_req_type = HTTP_REQ_UNKNOWN;
		}
	}
}

char data_st[3] = { 0x1b, 0x53, 0x00 };
char data_end[2] = { 0x1b, 0x43 };
char http_resp[] =  "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
					"<html><meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\""
					"<center><h1>Hello World</h1><br /><h1>from Hydrogen</h1><br />"
					"<a href=\"1\">Page 1</a></center></html>";

char http_img_resp[] =  "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\n";

void GSClass::send_http_resp()
{
	uint8_t i;

	switch(http_req_type) {
	case HTTP_REQ_GET_ROOT:
		data_st[2] = int_to_hex(client_cid);
		for (i = 0; i < 3; i++) {
			Serial.print(data_st[i]);
		}
		Serial.println(http_resp);
		for (i = 0; i < 2; i++) {
			Serial.print(data_end[i]);
		}
		break;
	case HTTP_REQ_UNKNOWN:
		data_st[2] = int_to_hex(client_cid);
		for (i = 0; i < 3; i++) {
			Serial.print(data_st[i]);
		}
		Serial.println(http_img_resp);
		for (i = 0; i < 2; i++) {
			Serial.print(data_end[i]);
		}
		break;
	default:
		break;
	}
}

