/* net - calculation of short-circuit currents 
 *  
 *   This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Igor 'sp_r00t' Tyukalov <tyukalov@bk.ru> */

#include "../../include/net.h"
#include "../../include/el.h"
#include "../../include/argparser.h"
#include <stdio.h>
#include <locale.h>

#define NUMOPT 7
#define PROGNAME "net"
#define VERSION "0.01"

extern netNetworkPtr netInit(char*);
extern netCircuitPtr netFindCircuit(netNetworkPtr, const char*);
extern double getImpedance(netCircuitPtr, netMode);

static void help(char*);
static void version();
static double calc(double, double, netMode);

int
main (int argc, char **argv)
{
  #ifdef USE_GETTEXT
	INIT_LOCALE;
  #endif
  setlocale(LC_NUMERIC, "C");
  OPTIONS opt[] =
    {
      {"m", AP_OPTIONAL},
      {"e", AP_OPTIONAL}, 	/* TODO Если нужно будет - реализовать. Если нет - убрать */
      {"v", AP_NONE},
      {"u", AP_OPTIONAL},
      {"-version", AP_NONE},
      {"h", AP_NONE},
      {"-help", AP_NONE}
    };  
  ARGPARSE_ARG arg =
    {
      '-',
      '=',
      argc,
      argv,
      NUMOPT,
      opt
    };
  char *filename, *point, *value;
  OPTARG args;
  netMode mode;
  double dvoltage, impedance, result;
  float voltage;
  PARGUMENTS nextarg;
  netNetworkPtr network;
  netCircuitPtr circuit;
  if(argc == 1)
  {
    help(argv[0]);
    return -1;
  };
  args = argparse(arg);
  if(args.result) {
    fprintf(stderr, _("Invalid argument!\n"));
    return -1;
  };
  if(argc == 2 && (findopt(args, "h", &value) != -1 || findopt(args, "-help", &value) != -1)) {
    help(argv[0]);
    return 0;
  }
  else if(argc == 2 && (findopt(args, "v", &value) != -1 || findopt(args, "-version", &value) != -1)) {
    version();
    return 0;
  }
  else if (argc == 2) {
    fputs(_("Too few arguments\n"), stderr);
    help(argv[0]);
    return -1;
  };
  if (findopt(args, "m", &value) == -1)
    mode = NET_THREE_PHASE;
  else {
    if (STREQ(value, "1"))
      mode = NET_ONE_PHASE;
    else if (STREQ(value, "2"))
      mode = NET_TWO_PHASE;
    else if (STREQ(value, "3"))
      mode = NET_THREE_PHASE;
    else {
      fprintf(stderr, _("Unknown mode - '%s'\n"), value);
      return -1;
    }
  }
  if (findopt(args, "u", &value) == -1)
    dvoltage = 400;
  else {
    if (!sscanf(value, "%f", &voltage)) {
      fprintf(stderr, _("Incorrect voltage value - '%s'\n"), value);
      return -1;
    }
    dvoltage = (double)voltage;
  }
  if (args.arg && args.arg->next) {
    filename = args.arg->argument;
    nextarg = args.arg->next;
    if (!(network = netInit(filename))) {
      fputs(_("Error reading the file\n"), stderr);
      return -1;
    }
    while (nextarg)
      {
	point = nextarg->argument;
	if (circuit = netFindCircuit(network, point)) {
	  if ((impedance = getImpedance(circuit, mode)) == (double)0) {
	    fputs(_("Error! Zero impedance\n"), stderr);
	    return -1;
	  } else {
	    fprintf(stderr, "%f\n", calc(dvoltage, impedance, mode));
	  }
	} else {
	  fprintf(stderr, _("Point '%s' not found\n"), point);
	}
	nextarg = nextarg -> next;
      }
  } else {
    fputs(_("Too few arguments\n"), stderr);
    return -1;
  };
  return 0;
}

void
help(char* progname)
{
  printf(_("Usage: %s [options] filename points...\n\
Where:\n"), progname);
  fputs(_("m - mode. Takes on values:\n\
    1 - single-phase mode;\n\
    2 - two-phase mode;\n\
    3 - three-phase mode.\n\
u - voltage, V;\n\
filename - network file;\n\
points - points of calculation of short-circuit currents.\n"),\
	stdout);
}

void
version()
{
  fprintf(stdout, "%s ( %s ) %s\n", PROGNAME, EL_PACKAGE, VERSION);
  fputs(_("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
Written by Igor 'sp_r00t' Tyukalov.\n"),\
	stdout);
}

double
calc(double voltage, double impedance, netMode mode)
{
  double K;
  switch (mode) {
  case NET_ONE_PHASE: {
    K = TRADICAL;
    break;
  }
  case NET_TWO_PHASE: {
    K = 0.5;
    break;
  }
  case NET_THREE_PHASE: {
    K = 1.0 / TRADICAL;
  }
  };
  return K * voltage / impedance;
}
