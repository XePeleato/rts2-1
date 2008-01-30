/* 
 * Device with database connection.
 * Copyright (C) 2004-2008 Petr Kubanek <petr@kubanek.net>
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

#include "rts2devicedb.h"

#define OPT_DEBUGDB    OPT_LOCAL + 201

EXEC SQL include sqlca;

int
Rts2DeviceDb::willConnect (Rts2Address * in_addr)
{
	if (in_addr->getType () < getDeviceType ()
		|| (in_addr->getType () == getDeviceType ()
		&& strcmp (in_addr->getName (), getDeviceName ()) < 0))
		return 1;
	return 0;
}


Rts2DeviceDb::Rts2DeviceDb (int in_argc, char **in_argv, int in_device_type,
char *default_name):Rts2Device (in_argc, in_argv, in_device_type, default_name)
{
	connectString = NULL;		 // defualt DB
	configFile = NULL;

	addOption (OPT_DATABASE, "database", 1, "connect string to PSQL database (default to stars)");
	addOption (OPT_CONFIG, "config", 1, "configuration file");
	addOption (OPT_DEBUGDB, "debugdb", 0, "print database debugging messages");
}


Rts2DeviceDb::~Rts2DeviceDb (void)
{
	EXEC SQL DISCONNECT;
	if (connectString)
		delete connectString;
}


int
Rts2DeviceDb::processOption (int in_opt)
{
	switch (in_opt)
	{
		case OPT_DATABASE:
			connectString = new char[strlen (optarg) + 1];
			strcpy (connectString, optarg);
			break;
		case OPT_CONFIG:
			configFile = optarg;
			break;
		case OPT_DEBUGDB:
			ECPGdebug (1, stderr);
			break;
		default:
			return Rts2Device::processOption (in_opt);
	}
	return 0;
}


int
Rts2DeviceDb::reloadConfig ()
{
	Rts2Config *config;

	// load config..

	config = Rts2Config::instance ();
	return config->loadFile (configFile);
}


int
Rts2DeviceDb::initDB ()
{
	int ret;
	std::string cs;
	EXEC SQL BEGIN DECLARE SECTION;
		const char *conn_str;
	EXEC SQL END DECLARE SECTION;
	// try to connect to DB

	Rts2Config *config;

	ret = reloadConfig();

	config = Rts2Config::instance ();

	if (ret)
		return ret;

	if (connectString)
	{
		conn_str = connectString;
	}
	else
	{
		config->getString ("database", "name", cs);
		conn_str = cs.c_str ();
	}

	EXEC SQL CONNECT TO :conn_str;
	if (sqlca.sqlcode != 0)
	{
		logStream (MESSAGE_ERROR) << "Rts2DeviceDb::init Cannot connect to DB '" << conn_str << "' : " << sqlca.sqlerrm.sqlerrmc << sendLog;
		return -1;
	}

	return 0;
}


int
Rts2DeviceDb::init ()
{
	int ret;

	ret = Rts2Device::init ();
	if (ret)
		return ret;

	// load config.
	return initDB ();
}


void
Rts2DeviceDb::forkedInstance ()
{
	// dosn't work??
	//  EXEC SQL DISCONNECT;
	Rts2Device::forkedInstance ();
}


void
Rts2DeviceDb::signaledHUP ()
{
	reloadConfig();
	Rts2DeviceDb::reloadConfig ();
}
