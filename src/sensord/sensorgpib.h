/* 
 * Class for GPIB sensors.
 * Copyright (C) 2007-2008 Petr Kubanek <petr@kubanek.net>
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

#ifndef __RTS2_SENSOR_GPIB__
#define __RTS2_SENSOR_GPIB__

#include "sensord.h"
#include "conngpib.h"

#include <gpib/ib.h>

namespace rts2sensord
{

class Gpib:public Sensor
{
	private:
		ConnGpib *connGpib;
	protected:
		int gpibWrite (const char *buf) { return connGpib->gpibWrite (buf); }
		int gpibRead (void *buf, int blen) { return connGpib->gpibRead (buf, blen); }
		int gpibWriteRead (const char *buf, char *val, int blen = 50) { return connGpib->gpibWriteRead (buf, val, blen); }

		int gpibWaitSRQ () { return connGpib->gpibWaitSRQ (); }

		virtual int processOption (int in_opt);
		virtual int init ();

		void setPad (int _pad) { return connGpib->setPad (_pad); }
	public:
		Gpib (int argc, char **argv);
		virtual ~ Gpib (void);
};

};
#endif		 /* !__RTS2_SENSOR_GPIB__ */
