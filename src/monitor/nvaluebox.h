/* 
 * Dialog boxes for setting values.
 * Copyright (C) 2007,2010 Petr Kubanek <petr@kubanek.net>
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

#ifndef __RTS2_NVALUEBOX__
#define __RTS2_NVALUEBOX__

#include "../utils/rts2value.h"
#include "../utils/rts2valuerectangle.h"

#include "nwindow.h"
#include "nwindowedit.h"
#include "daemonwindow.h"

namespace rts2ncurses
{

/**
 * Abstract class represening edit box for a value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBox
{
	public:
		ValueBox (NWindow * top, Rts2Value * _val);
		virtual ~ValueBox (void);
		virtual keyRet injectKey (int key) = 0;
		virtual void draw () = 0;
		virtual void sendValue (Rts2Conn * connection) = 0;
		virtual bool setCursor () = 0;
	protected:
		Rts2Value * getValue () { return val; }
	private:
		NWindow * topWindow;
		Rts2Value * val;
};

/**
 * Edit box for boolean value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxBool:public ValueBox, NSelWindow
{
	public:
		ValueBoxBool (NWindow * top, Rts2ValueBool * _val, int _x, int _y);
		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
};

/**
 * Edit box for string value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxString:public ValueBox, NWindowEdit
{
	public:
		ValueBoxString (NWindow * top, Rts2Value * _val, int _x, int _y);
		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
};

/**
 * Edit box for integer value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxInteger:public ValueBox, NWindowEditIntegers
{
	public:
		ValueBoxInteger (NWindow * top, Rts2ValueInteger * _val, int _x, int _y);
		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
};

/**
 * Edit box for long integer value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxLongInteger:public ValueBox, NWindowEditIntegers
{
	public:
		ValueBoxLongInteger (NWindow * top, Rts2ValueLong * _val, int _x, int _y);
		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
};

/**
 * Edit box for float value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxFloat:public ValueBox, NWindowEditDigits
{
	public:
		ValueBoxFloat (NWindow * top, Rts2ValueFloat * _val, int _x, int _y);

		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
};

/**
 * Edit box for double value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxDouble:public ValueBox, NWindowEditDigits
{
	public:
		ValueBoxDouble (NWindow * top, Rts2ValueDouble * _val, int _x, int _y);

		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
};

/**
 * Common ancestor for selection boxes.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class AbstractBoxSelection:public ValueBox, public NSelWindow
{
	public:
		AbstractBoxSelection (NWindow * top, Rts2Value * _val, int _x, int _y);
		virtual keyRet injectKey (int key);
		virtual bool setCursor ();
	protected:
		void drawRow (const char *_text);
};

/**
 * Edit box for selection value.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxSelection:public AbstractBoxSelection
{
	public:
		ValueBoxSelection (NWindow * top, Rts2ValueSelection * _val, int _x, int _y);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
};

/**
 * Selection box for time increase.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxTimeDiff:public AbstractBoxSelection
{
	public:
		ValueBoxTimeDiff (NWindow * top, Rts2ValueTime *_val, int _x, int _y);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
};

/**
 * Edit box for editting rectangle (4 numbers)
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxRectangle:public ValueBox, NWindowEdit
{
	public:
		ValueBoxRectangle (NWindow * top, Rts2ValueRectangle * _val, int _x, int _y);
		virtual ~ValueBoxRectangle ();
		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
	private:
		NWindowEditIntegers * edt[4];
		int edtSelected;
};

/**
 * Edit box for editting  RA Dec.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class ValueBoxRaDec:public ValueBox, NWindowEdit
{
	public:
		ValueBoxRaDec (NWindow * top, Rts2ValueRaDec * _val, int _x, int _y);
		virtual ~ValueBoxRaDec ();
		virtual keyRet injectKey (int key);
		virtual void draw ();
		virtual void sendValue (Rts2Conn * connection);
		virtual bool setCursor ();
	private:
		NWindowEditDigits * edt[2];
		int edtSelected;
};

}

#endif // !__RTS2_NVALUEBOX__ 
