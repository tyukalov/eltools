/* config for sccinit
   
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


/* Drfinition string attributes */

/* Common */
#define HIGH_VOLTAGE    "uh"
#define LOW_VOLTAGE     "ul"

/* For initSystem */
#define SC_CURRENT          "iscc"
#define SC_POWER               "sscc"
#define SC_BREAKER          "ir"

/* For initTransformer */
#define SCHEMA                    "schema" /* Values - "dy" and "" */
#define SC_VOLTAGE          "uk"	   /* % */
#define NOM_POWER          "sn"		   /* kVA */
#define SC_POWERLOSS     "pk"		   /* kW */
#define DEFAULT_UK           5.5
#define DEFAULT_PK            10.0

/* For initBus */
#define BUS_RES                     "r" /* mOm */
#define BUS_REACT               "x" /* mOm */

/* For initReactor */
#define R_POWERLOSS         "dp"     /* W */
#define R_CURRENT               "in" /* A */
#define R_INDUCTANCE        "L"	/* Gn */
#define R_MINDUCTANCE    "M"	/* Gn */
#define W                                     2 * 3.14 * 50 /* 2*pi*f */
