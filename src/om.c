/* om - program for calculating by Ohm's law
   
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

#define NUMOPT	sizeof(opt)/sizeof(OPTIONS)
#define FZERO	0.0
#define VERSION "0.01"
#define PROGNAME "om"


static void help(char*);
static void version();
static int FindOption(OPTARG, const char*,  char**);

int 
main (int argc, char **argv)
{
  #ifdef USE_GETTEXT
	INIT_LOCALE;
	setlocale(LC_NUMERIC, "C");
  #endif
  OPTIONS		opt[]	=
    {
      {"p", AP_NONE},
      {"d", AP_NONE},
      {"h", AP_NONE},
      {"-help", AP_NONE},
      {"v", AP_NONE},
      {"-version", AP_NONE},
      {"i", AP_REQUIRED},
      {"u", AP_REQUIRED},
      {"r", AP_REQUIRED},
      {"x", AP_REQUIRED},
    };
  ARGPARSE_ARG 	arg =
    {
      '-',
      EL_SPACE,
      argc,
      argv,
      NUMOPT,
      opt,
    };
  OM_ARGS omargs;
  OPTARG args;
  int count, num, iflag=0, uflag=0;
  float var;
  double result;
  char *value;
  omargs.mode = 0x0;
if((argc == 1) || (argc > (NUMOPT+4))) {
  fputs(_("Wrong number of arguments\n"), stderr);
  return -1;
 };
 args = argparse(arg);
 if(args.result) {
   fputs(_("Invalid argument!\n"), stderr);
   return -1;
 }
 if(argc == 2 && (FindOption(args, "h", &value) != -1 || FindOption(args, "-help", &value) != -1)) {
   help(argv[0]);
   return 0;
 }
 if(argc == 2 && (FindOption(args, "v", &value) != -1 || FindOption(args, "-version", &value) != -1)) {
   version();
   return 0;
 }
 if(FindOption(args, "p", &value) != -1)
   omargs.mode = omargs.mode | OM_THREE_PHASE;
 else
   omargs.mode = omargs.mode | OM_ONE_PHASE;
 if(FindOption(args, "d", &value) == -1) {
   omargs.mode = omargs.mode | OM_AC_MODE;
   omargs.resistance = omargs.reactance = 0; /* Значения по умолчанию, во избежание коллизий */
 } else
   omargs.mode = omargs.mode | OM_DC_MODE;
 if (FindOption(args, "i", &value) != -1) {
   if (sscanf(value, "%f", &var)){
     iflag = 1;
     omargs.amperage = (double)var;
   } else {
     fputs(_("Invalid value for -i\n"), stderr);
     return -1;
   }
 } else
   omargs.mode = omargs.mode | OM_I_RESULT;
 if (FindOption(args, "u", &value) != -1) {
   if (sscanf(value, "%f", &var)){
     uflag = 1;
     omargs.voltage = (double)var;
   } else {
     fputs(_("Invalid value for -u\n"), stderr);
     return -1;
   }
 } else
   omargs.mode = omargs.mode | OM_U_RESULT;
 if (iflag*uflag)
   omargs.mode = omargs.mode | OM_R_RESULT;
 if (FindOption(args, "r", &value) != -1) {
   if (sscanf(value, "%f", &var)){
     omargs.resistance = (double)var;
   } else {
     fputs(_("Invalid value for -r\n"), stderr);
     return -1;
   }
 };
 if (FindOption(args, "x", &value) != -1) {
   if (sscanf(value, "%f", &var)){
     omargs.reactance = (double)var;
   } else {
     fputs(_("Invalid value for -x\n"), stderr);
     return -1;
   }
 };
 switch(om(&omargs, &result)) {
 case OM_NORMAL: {
   printf("%f\n", result);
   break;
 };
 case OM_INVALID_COMBINATION_MODES: {
   fputs(_("Incorrect combinations of modes\n"), stderr);
   return -1;
 };
 case OM_INVALID_REQUEST: {
   fputs(_("Invalid request\n"), stderr);
   return -1;
 };
 default: {
   fputs(_("Unknown error\n"), stderr);
   return -1;
 }
 }
 return 0;
}

static int
FindOption(OPTARG a, const char *o,  char **value)
{
  POPTION opts = a.opt;
  while(opts) {
    if(!strcmp(opts->option, o)) {
      *value = opts->value;
      return 0;
    }
    opts = opts->next;
  }
  return -1;
}

static void
help(char *str)
{
  printf(_("Usage %s %s\n"), str, "[-pdhviurx] [--help] [--version]");
  fputs(_("\
where:\n\
 -h or --help - print help\n\
 -v or --version - print version\n\
 -p - mode. With the option - single-phase, in the absence of - three-phase\n\
 -d - type of current. With the option - DC, in the absence of - AC\n\
 -i - amperage,\n\
 -u - voltage,\n\
 -r - resistance,\n\
 -x - reactance,\n "), stdout);
}

static void
version()
{
  fprintf(stdout, "%s ( %s ) %s\n", PROGNAME, EL_PACKAGE, VERSION);
  fputs(_("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
Written by Igor 'sp_r00t' Tyukalov.\n"),\
	stdout);
}
