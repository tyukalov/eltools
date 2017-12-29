/* definition for libom
   
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

#define MODE_MASK(A) ((A) & (OM_DC_MODE | OM_AC_MODE))
#define RESULT_MASK(A) ((A) & (OM_I_RESULT | OM_U_RESULT | OM_R_RESULT))
#define PHASE_MASK(A) ((A) & (OM_ONE_PHASE | OM_THREE_PHASE))
#define TRADICAL 1.7320508

enum emode {
  OM_AC_MODE     = 0x1,        \
  OM_DC_MODE     = 0x2,        \
  OM_ONE_PHASE   = 0x4,        \
  OM_THREE_PHASE = 0x8,        \
  OM_I_RESULT    = 0x10,       \
  OM_U_RESULT    = 0x20,       \
  OM_R_RESULT    = 0x40        \
};

enum err {
  OM_NORMAL,
  OM_INVALID_COMBINATION_MODES,
  OM_INVALID_REQUEST,
};

typedef struct {
  int		        mode;
  double		voltage;
  double		amperage;
  double		resistance;
  double		reactance;
} OM_ARGS, *POM_ARGS;

int om(POM_ARGS args, double* result);
