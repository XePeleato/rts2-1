/*
 * Sitech telescope protocol handling.
 * Copyright (C) 2014 Petr Kubanek <petr@kubanek.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "connection/sitech.h"

using namespace rts2teld;

ConnSitech::ConnSitech (const char *devName, rts2core::Block *_master):rts2core::ConnSerial (devName, _master, rts2core::BS19200, rts2core::C8, rts2core::NONE, 5,5)
{
}

int ConnSitech::init ()
{
	int ret = rts2core::ConnSerial::init ();
	if (ret)
		return ret;

	// switch to checksum mode
	ret = writePort ("YXY1\r", 5);
	if (ret < 0)
		return -1;
	return 0;
}

void ConnSitech::siTechCommand (const char axis, const char *cmd)
{
	size_t len = strlen (cmd);
	char ccmd[strlen(cmd) + 3];

	ccmd[0] = axis;
	memcpy (ccmd + 1, cmd, len);
	ccmd[len + 2] = '\r';
	ccmd[len + 3] = '\0';

	writePortChecksumed (ccmd, len + 1);
}

int32_t ConnSitech::getSiTechValue (const char axis, const char *val)
{
	siTechCommand (axis, val);

	char ret[100];

	size_t len = readPort (ret, 100, "\n");
	if (len < 0)
		throw rts2core::Error (std::string ("cannot read response get value command ") + val);

	return atol (ret + 1);
}

void ConnSitech::getAxisStatus (char axis, SitechAxisStatus &ax_status)
{
	siTechCommand (axis, "XS");

	char ret[42];

	size_t len = readPort (ret, 41);
	if (len != 41)
	{
		flushPortIO ();
		throw rts2core::Error ("cannot read all 41 bits from status response!");
	}

	// checksum checks
	uint16_t checksum = 0;
	for (int i = 0; i < 39; i++)
		checksum += ret[i];
	
	if (ret[39] != (checksum & 0x00FF) || ret[40] != (~(checksum >> 16) & 0xFF))
		throw rts2core::Error ("invalid checksum!");

	// fill in proper return values..
	ax_status.address = ret[0];
	ax_status.x_pos = ntohl (*(ret + 1));
	ax_status.y_pos = ntohl (*(ret + 5));
	ax_status.x_enc = ntohl (*(ret + 9));
	ax_status.y_enc = ntohl (*(ret + 13));

	ax_status.keypad = ret[17];
	ax_status.x_bit = ret[18];
	ax_status.y_bit = ret[19];
	ax_status.extra_bit = ret[20];
	ax_status.ain_1 = ntohs (*(ret + 21));
	ax_status.ain_2 = ntohs (*(ret + 23));
	ax_status.mclock = ntohl (*(ret + 25));
	ax_status.temperature = ret[29];
	ax_status.y_worm_phase = ret[30];
	ax_status.x_last = ntohl (*(ret + 31));
	ax_status.y_last = ntohl (*(ret + 35));
}

void ConnSitech::setSiTechValue (const char axis, const char *val, int value)
{
	char *ccmd = NULL;
	size_t len = asprintf (&ccmd, "%c%s%d\r", axis, val, value);

	try
	{
		writePortChecksumed (ccmd, len);
	}
	catch (rts2core::Error &er)
	{
		free (ccmd);
		throw er;
	}

	free (ccmd);
}

void ConnSitech::writePortChecksumed (const char *cmd, size_t len)
{
	size_t ret = writePort (cmd, len);
	if (ret < 0)
		throw rts2core::Error (std::string ("cannot write ") + cmd + " :" + strerror (errno));

	ret = writePort ((char) calculateChecksum (cmd, len));
	if (ret < 0)
		throw rts2core::Error (std::string ("cannot write checksum of ") + cmd + " :" + strerror (errno));
}

uint8_t ConnSitech::calculateChecksum (const char *cbuf, size_t len)
{
	uint8_t ret = 0;
	for (size_t i = 0; i < len; i++)
		ret += cbuf[i];
	
	return ~ret;
}
