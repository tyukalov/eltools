/* motor - calculation of motor current
   
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

#include "../include/el.h"
#include "../include/argparser.h"

#define PROGNAME "motor"
#define VERSION "0.01"
#define INITMOTOR(A, M, V, O) {						\
  if(findopt((O), (A) , &(V))==-1) {MISSINGOPT((A));} \
  else                                                                                                        \
    {                                                                                                           \
        if(((M) = strtod((V),NULL))==-1) FERROR((V));			       	\
    }                                                                                                          \
  }


typedef enum{
  AC,
  AC3,
  DC,
} modes;

typedef struct
{
  modes mode;
  double voltage;
  double power;
  double cos;
  double kpd;
} MOTOR;

static void help(char*);
static void version();
static double calculation(MOTOR);

int
main (int argc, char **argv)
{
  #ifdef USE_GETTEXT
	INIT_LOCALE;
	setlocale(LC_NUMERIC, "C");
  #endif
  MOTOR motor;
  char *value;
  switch (argc)
    {
    case 1:
      {
	help(argv[0]);
	return -1;
      }
    case 2:
      {
	if (STREQ("--help", argv[1]) || STREQ("-h", argv[1])){
	  help(argv[0]);
	  return 0;
	}
	else if (STREQ("--version", argv[1]) || STREQ("-v", argv[1])){
	  version();
	  return 0;
	}
	else {
	  if ((motor.power=strtod(argv[1], NULL)) != 0.0) {
		motor.mode = AC3;
		motor.voltage = 0.38;
		motor.kpd = 0.95;
		motor.cos = 0.9;
		printf("%f\n", calculation(motor));
		return 0;
	      } else FERROR(argv[1]);
	  }
      }
    default:
      {
	OPTIONS arr[] = {
	  {"u", AP_REQUIRED},
	  {"p", AP_REQUIRED},
	  {"n", AP_REQUIRED},
	  {"c", AP_REQUIRED},
	  {"m", AP_REQUIRED}
	};
	ARGPARSE_ARG arg = {
	  '-',
	  '=',
	  argc,
	  argv,
	  5,
	  arr
	};
	OPTARG opt = argparse(arg);
	if(opt.result) {
	  if(opt.result == AP_ILLEGAL_USE_OPTIONS)
	    printf(_("Error! Illegal use of options\n"));
	  else if(opt.result == AP_MISSING_VALUE) 
	    printf(_("Error! Missing value from option!\n"));
	  else if (opt.result == AP_UNKNOWN_OPTION)
	    printf(_("Unknown option '%s'\n"), argv[opt.erropt]);
	  argclean(opt);
	  return -1;
	}
	if(findopt(opt, "m", &value)==-1)
	  motor.mode=AC3;
	else {
	  if(STREQ(value, "ac"))
	    motor.mode=AC;
	  else if (STREQ(value, "dc"))
	    motor.mode=DC;
	  else if (STREQ(value, "ac3"))
	    motor.mode=DC;
	  else
	    FERROR(value);
	}
	INITMOTOR("u", motor.voltage, value, opt);
	INITMOTOR("p", motor.power, value, opt);
	INITMOTOR("n", motor.kpd, value, opt);
	INITMOTOR("c", motor.cos, value, opt);
	if ((motor.voltage == 0) || (motor.kpd == 0) || (motor.cos == 0)){
	  fputs(_("Incorrect data!\n"), stderr);
	  return -1;
	}
	printf("%f\n", calculation(motor));
	return 0;
      }
    }
}

static double
calculation(MOTOR motor)
{
  double k = (motor.mode==AC3)?(1 / sqrt(3.0)):1.0;
  return k * motor.power / (motor.voltage * motor.cos * motor.kpd);
}

static void
help(char *str)
{
  printf(_("Usage %s %s\n"), str, "-m=mode -u=voltage -p=power -c=cos -n=ece");
  fputs(_("\
where:\n\
 mode - mode. 'dc' for D.C.,  'ac' for single-phase alternating current, 'ac3' for three-phase alternating current;\n\
 voltage - motor voltage;\n\
 power - engine power;\n\
 cos - power factor;\n\
 ece - energy conversion efficiency,\n\
or:\n\
"), stdout);
  printf(_("%s power\n"), str);
  fputs(_("\
 where power - engine power (default voltage=0.38, cos=0.9 ece=0.95, mode=ac3)\n\
"), stdout);
}

static void
version()
{
  fprintf(stdout, "%s ( %s ) %s\n", PROGNAME, EL_PACKAGE, VERSION);
  fputs(_("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
Written by Igor 'sp_r00t' Tyukalov.\n"),\
	stdout);
}
