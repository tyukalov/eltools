/* libom - library for calculating by Ohm's law
   
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Igor 'sp_r00t' Tyukalov <tyukalov@bk.ru> */

#include "../include/om.h"
#include <math.h>

#define OM_BREAK(A) \
  {                                        \
    (A);                                 \
    break;                            \
  }

int 
om(POM_ARGS args, double *result) 
{
  double full;
  switch (MODE_MASK(args->mode)){
  case OM_DC_MODE: {
    switch (RESULT_MASK(args->mode)){
    case OM_I_RESULT: {
      if (args->resistance == 0.0)
	return OM_INVALID_REQUEST;
      OM_BREAK(*result = (args->voltage / args->resistance));
    }
    case OM_U_RESULT:  OM_BREAK(*result = (args->amperage * args->resistance));
    case OM_R_RESULT: {
      if (args->amperage == 0.0)
	return OM_INVALID_REQUEST;
      OM_BREAK(*result = (args->voltage / args->amperage));
    }
    default: return OM_INVALID_REQUEST;
    };
    break;
  }
  case OM_AC_MODE:
    {
      int rmask = RESULT_MASK(args->mode);
      if (rmask & OM_R_RESULT) {
	if (args->amperage == 0.0)
	  return OM_INVALID_REQUEST;
	double res = args->voltage / args->amperage;
	switch (PHASE_MASK(args->mode)){
	case OM_ONE_PHASE: OM_BREAK(*result = res);
	case OM_THREE_PHASE: OM_BREAK(*result = res / TRADICAL);
	default: return OM_INVALID_REQUEST;
	};
	break;
      };
      full = sqrt((args->resistance)*(args->resistance) + (args->reactance)*(args->reactance));
      if(PHASE_MASK(args->mode)) full *= TRADICAL;
      switch (RESULT_MASK(args->mode)) {
      case OM_I_RESULT: {
	if (full == 0.0)
	  return OM_INVALID_REQUEST;
	OM_BREAK(*result = (args->voltage / full));
      }
      case OM_U_RESULT: OM_BREAK(*result = (args->amperage * full));
      default: return OM_INVALID_REQUEST;
      };
      break;
    }
  default: return OM_INVALID_COMBINATION_MODES;
  }
  return OM_NORMAL;
}
