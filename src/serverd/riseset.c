/*!
 * @file Calculate rise and set times.
 *
 * Used for calculating of next events (dusk, night, dawn, day).
 *
 * @author petr
 */

#define _GNU_SOURCE

#include <libnova.h>
#include <math.h>

#include "riseset.h"
#include "status.h"

#define DAWNDUSK_TIME		(1.0 / 24.0) * 2.0

int
next_event (time_t * start_time, int *type, time_t * ev_time)
{
  double jd_time = get_julian_from_timet (start_time);
  double md;
  struct ln_lnlat_posn obs;
  struct ln_equ_posn pos;
  struct ln_rst_time rst;

  obs.lat = 50;
  obs.lng = -14.95;

  md = round (jd_time);

  get_equ_solar_coords (md, &pos);

  if (get_object_next_rst (jd_time, &obs, &pos, &rst))
    // don't care about abs(lng)>60
    return -1;

  if (rst.rise >= rst.set)
    {
      // daytime, so decide between DAWN, DAY, DUSK 
      if (jd_time > (rst.set - DAWNDUSK_TIME))
	{
	  *type = SERVERD_NIGHT;
	  get_timet_from_julian (rst.set, ev_time);
	}
      else if (jd_time < (rst.rise + DAWNDUSK_TIME - 1))
	{
	  *type = SERVERD_DAY;
	  get_timet_from_julian (rst.rise + DAWNDUSK_TIME - 1, ev_time);
	}
      else
	{
	  *type = SERVERD_DUSK;
	  get_timet_from_julian (rst.set - DAWNDUSK_TIME, ev_time);
	}
    }
  else
    {
      *type = SERVERD_DAWN;

      get_timet_from_julian (rst.rise, ev_time);
    }
  return 0;
}
