/* definition for eltools
   
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef USE_GETTEXT
  #include <libintl.h>
  #include <locale.h>
#endif

/* Common definition */
#define EL_PACKAGE      "eltools"
#define EL_SPACE		' '
#define EL_TRUE		0x1
#define EL_FALSE		0x0

#define STREQ(A,B) (strcmp((A), (B))==0)
#ifdef USE_GETTEXT
  #define _(A) gettext(A)
  #define INIT_LOCALE          {                                          \
    setlocale(LC_ALL,"");                                                       \
    bindtextdomain(EL_PACKAGE, USE_GETTEXT);    \
    textdomain(EL_PACKAGE);}
#else
  #define _(A) A
#endif

#define FERROR(A) {                                \
  fprintf(stderr, _("Invalid value %s\n"), A); \
  return -1;                                                     \
  }
#define MISSINGOPT(A) {                                 \
    fprintf(stderr, _("Missing paraneter '%s'\n"), A); \
    return -1;                                                            \
  }

/* Definition for libom */

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


/* Interfaces */

/* For libom */
int om(POM_ARGS args, double* result);
