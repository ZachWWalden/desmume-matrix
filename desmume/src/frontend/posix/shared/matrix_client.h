/*==================================================================================
 *Class - matrix_client
 *Author - Zach Walden
 *Created - May 2024
 *Last Changed - 6/7/2025
 *Description -
====================================================================================*/

/*
 * This program source code file is part of desmume-matrix
 *
 * Copyright (C) 2022 Zachary Walden zachary.walden@eagles.oc.edu
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/lgpl-3.0.en.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
#ifndef MATRIX_CLIENT_H
#define MATRIX_CLIENT_H
#define HAVE_MATRIX

//#include <pthread.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>

#include "../../../types.h"

#define PROTOCOL_VERSION 0x00

#pragma pack(push,1)
struct SinkPacketHeader
{
	//Protocol Version
	u8 protocol_vers;
	//Priority
	u8 priority;
	//resolution
	u8 h_res;
	u8 v_res;
	//Bytes for pixels
	u8 bytes_per_pixel;
	//color mode
	u8 color_mode;
	//Location
	u8 h_loc;
	u8 v_loc;
	//Display intensity
	float intensity;
};

struct HandshakeHeader
{
	uint8_t req_protocol_vers;
	uint8_t success;
};
#pragma pack(pop)

class matrix_client
{
	//Attributes
public:

private:
	int client_fd;
	bool conn_valid = false;
	//Methods
public:
	matrix_client();
	matrix_client(std::string addr);
	~matrix_client();

	bool send_frame(u16 *buffer,int height, int width);

private:
};
#endif
