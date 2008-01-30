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

#ifndef __RTS2_DEVICEDB__
#define __RTS2_DEVICEDB__

#include "../utils/rts2device.h"
#include "../utils/rts2config.h"

/**
 * This add database connectivity to device class
 */

class Rts2DeviceDb:public Rts2Device
{
	private:
		char *connectString;
		char *configFile;
	protected:
		virtual int willConnect (Rts2Address * in_addr);
		virtual int processOption (int in_opt);
		virtual int reloadConfig ();

		/**
		 * Init database connection.
		 *
		 * @return -1 on error, 0 on sucess.
		 */
		int initDB ();
		virtual int init ();
		virtual void forkedInstance ();
	public:
		Rts2DeviceDb (int in_argc, char **in_argv, int in_device_type, char *default_name);
		virtual ~ Rts2DeviceDb (void);

		virtual void signaledHUP ();
};
#endif							 /* !__RTS2_DEVICEDB__ */
