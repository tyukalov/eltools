/* vl380 - program for calculating voltage losses in 0.4 kV networks
   
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
#include <string.h>
#include <stdlib.h>
#include "../include/csvdblt.h"

#define NUMBER_OF_VALUES	5
#define VALUESIZE		6
#define DELIMITER		';'
#define MAX_STR_SIZE		256

typedef enum{INVALID_ARGS=0, ERROR, DB_ERROR, NF_ERROR} err;
const char *errors[] = {"Invalid argument", "Pleas, usage 'vl380 [-dh] | [material(al|cu) cross_section cos length amperage]'", "Database error", "Value not found"};

static double getvalue(char *material, char *cs, char *cos, char *len, char *amperage);
static int asfilter(void);
static int dialog(void);
/*static int print(void);*/
static int calculate(char **argv);
static int errarg(err);
static void normalize(char*);

int 
main(int argc, char **argv)
{
  switch(argc){
  case 1:
    return asfilter();
  case 2: {
    if(!strcmp(argv[1], "-d"))
      return dialog();
    if(!strcmp(argv[1], "-h"))
      return errarg(ERROR);
    return errarg(INVALID_ARGS);
  }
  case 6:
    return calculate(argv);
  default:
    return errarg(ERROR);
  }
}

static int 
errarg(err e)
{
  fprintf(stderr, "%s\n", errors[e]);
  return -1;
}

/* TODO Переписать так, чтобы пути к файлам БД извлекались из конфигурационного файла */
static double 
getvalue(char *material, char *cs, char *cos, char *length, char *amperage)
{
  CSVD csvd;
  char *db[2] = {"../databases/al.csv", "../databases/cu.csv"};
  char value[VALUESIZE];
  int index;
  double amper,len, moment, result;
  if(!strcmp(material, "al"))
    index=0;
  else if(!strcmp(material, "cu"))
    index=1;
  else
    return errarg(INVALID_ARGS);
  if(csvd = csvinit(db[index], DELIMITER)) {
    switch(csvselect(csvd, cs, cos, value)){
    case DATABASE_ERROR: {
      csvclose(csvd);
      return errarg(DB_ERROR);
    }
    case NOT_FOUND: {
      csvclose(csvd);
      return errarg(NF_ERROR);
    }
    case 0: {
      len		= atof(length);
      amper		= atof(amperage);
      moment		= atof(value);
      csvclose(csvd);
      return len * amper * moment;
    }
    default:
      break;
    }
  }
  csvclose(csvd);
  return errarg(DB_ERROR);
}

static int 
calculate(char **argv)
{
  double result = getvalue(argv[1], argv[2], argv[3], argv[4], argv[5]);
  if(result != -1.0) {
    fprintf(stdout, "%f\n", result);
    return 0;
  }
  return -1;
}

static int 
asfilter()
{
  char input[MAX_STRING_SIZE], *params[NUMBER_OF_VALUES], *inp, ch;
  int flag, count;
  flag		= TRUE;
  count		= 0;
  if(fgets(input, MAX_STR_SIZE, stdin)) {
    normalize(input);
    inp	= input;
    /* Грубая сила - удаляем начальные пробелы */
    while(*inp == ' ') inp++;
    /* Дробим строку по пробелам */
    while(ch=*inp)
      {
	if((ch != ' ') && (flag == TRUE)) {
	  params[count] = inp;
	  count++;
	  if(count == NUMBER_OF_VALUES)
	    break;
	  flag = FALSE;
	} else if(ch == ' ')
	  {
	    if(!flag) {
	      *inp = 0; /* Обрезаем строку */
	      flag = TRUE;
	    }
	  }
	inp++;
      }
    fprintf(stdout, "%f\n", getvalue(params[0], params[1], params[2], params[3], params[4]));
    return 0;
  }
  fprintf(stderr, "pl380 error!\n");
  return -1;
}

static int
dialog()
{
  char material[MAX_STR_SIZE];
  char cross_s[MAX_STR_SIZE];
  char cos[MAX_STR_SIZE];
  char len[MAX_STR_SIZE];
  char amp[MAX_STR_SIZE];
  char cont[MAX_STR_SIZE];
  do
    {
      printf("Specify material: ");
      fgets(material, MAX_STR_SIZE, stdin);
      normalize(material);
      printf("Input cross-section: ");
      fgets(cross_s, MAX_STR_SIZE, stdin);
      normalize(cross_s);
      printf("Input cos: ");
      fgets(cos, MAX_STR_SIZE, stdin);
      normalize(cos);
      printf("Input length: ");
      fgets(len, MAX_STR_SIZE, stdin);
      normalize(len);
      printf("Input amperage: ");
      fgets(amp, MAX_STR_SIZE, stdin);
      normalize(amp);
      printf("%f\%\n Continue?(y/n [y])", getvalue(material, cross_s, cos, len, amp));
      fgets(cont, MAX_STR_SIZE, stdin);
      normalize(cont);
    }while(strcmp(cont, "n"));
  return 0;
}

static void
normalize(char *s)
{
  while(*s)
    {
      if((*s == '\n') || (*s == '\r')) {
	*s = 0;
	break;
      }
      s++;
    }
}


