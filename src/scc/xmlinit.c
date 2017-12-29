/* initializator for netNode.
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

#include "../../include/el.h"
#include "../../include/net.h"
#include <libxml/xmlreader.h>
#include "xmlinit.h"

#define PRINTD fputs("Debugstr\n", stdout)
#define ALLOC_NODE (netNodePtr)calloc(sizeof(struct netNode), 1)
#define SCANMAC(A, B) sscanf((A), "%f", &(B)) 
#define VALIDATOR(A,B) if (!SCANMAC((A), (B))) return EL_FALSE
#define GETPROPMAC(A,B)  xmlGetProp((A), (xmlChar*)(B))
#define DEFAULT_INIT(X,S,V,P,K,N) 		 \
  if (S = GETPROPMAC(X, K))       \
    {\
      VALIDATOR(S,V); \
      N->P = (double)V;      \
    }\
  

static int initNode(xmlNodePtr cur, netNodePtr node, netPointsPtr *point, netPointsPtr *rootpoint);
static int initElement(xmlNodePtr cur, netNodePtr node);
static int initSystem(xmlNodePtr cur, netNodePtr node);
static int initTransformer(xmlNodePtr cur, netNodePtr node);
static int initBus(xmlNodePtr cur, netNodePtr node);
static int initLine(xmlNodePtr cur, netNodePtr node);
static int initCommutator(xmlNodePtr cur, netNodePtr node);
static int initReactor(xmlNodePtr cur, netNodePtr node);
static int initResist(xmlNodePtr cur, netNodePtr node);
static void prnt(netNodePtr node);

netNetworkPtr
netInit (char *filename)
{
  netNetworkPtr result = (netNetworkPtr)calloc(sizeof(struct netNetwork), 1);
  xmlDocPtr doc = xmlParseFile(filename);
  xmlNodePtr cur;
  netPointsPtr plink = (netPointsPtr)NULL;
  result->network = ALLOC_NODE;
  /* ----------------------------------- */
  if (doc == NULL)
    return (netNetworkPtr)NULL;
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL)
    return (netNetworkPtr)NULL;
  if (initNode(cur, result->network, &plink, &(result->points))) {
    xmlFreeDoc(doc);
    return result;
  } else {
    xmlFreeDoc(doc);
    return (netNetworkPtr)NULL;
  }
}

static int
initNode(xmlNodePtr cur, netNodePtr node, netPointsPtr *points, netPointsPtr *rootpoint)
{
  xmlNodePtr ch;
  xmlAttrPtr attr;
  netNodePtr child;
  netNodePtr var;
  /* netPointsPtr newpoints; */
  /* -------------------------------- */
  if (cur == NULL)
    return EL_FALSE;
  if (STREQ(cur->name, (xmlChar*)"text"))
    return EL_TRUE;
  if (initElement(cur, node) == EL_FALSE)
    return EL_FALSE;
  if (node->point) {
    /* *points = (netPointsPtr)calloc(sizeof(struct netPoints), 1); */
    /* if (!(*rootpoints)) *rootpoints = *points; /\* Инициализация корневого списка *\/ */
    /* ======================== */
    if (!(*points))
      *rootpoint = *points = (netPointsPtr)calloc(sizeof(struct netPoints), 1);
    else
      *points = (*points)->next =  (netPointsPtr)calloc(sizeof(struct netPoints), 1);
    (*points)->node = node;
    (*points)->point = node->point;
  }
  ch = cur->xmlChildrenNode;
  while (ch)
    {
      if(STREQ(ch->name, (xmlChar*)"text")) {
	ch = ch->next;
	continue;
      }
      child = ALLOC_NODE;
      child->parent = node;
      if (initNode(ch, child, points, rootpoint) == EL_FALSE)
	return EL_FALSE;
      if (node->child == NULL) {
	node->child = child;
	var = child;
      } else {
	var->next = child;
	var = child;
      }
      ch = ch->next;
    }
  return EL_TRUE;
}

static int
initElement(xmlNodePtr cur, netNodePtr node)
{
  xmlChar *value;
  float val;
  DEFAULT_INIT(cur, value, val, resistance, "R", node);
  DEFAULT_INIT(cur, value, val, reactance, "X", node);
  DEFAULT_INIT(cur, value, val, zresistance, "R0", node);
  DEFAULT_INIT(cur, value, val, zreactance, "X0", node);
  if (value = GETPROPMAC(cur, "name")) {
    node->point = (char*)calloc(1, strlen(value)+1);
    strcpy(node->point, value);
  };
  if(STREQ(cur->name, (xmlChar*)"system"))
    return initSystem(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"transformer"))
    return initTransformer(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"bus"))
    return initBus(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"cable"))
    return initLine(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"airline"))
    return initLine(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"commutator"))
    return initResist(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"resist"))
    return initResist(cur, node);
  else if(STREQ(cur->name, (xmlChar*)"reactor"))
    return initReactor(cur, node);
  return EL_TRUE;
}

static int
initSystem(xmlNodePtr cur, netNodePtr node)
{
  /* Reactance - in mOm*/
  char* uh;                          /* System high voltage, V */
  char* ul;                           /* System low voltage, V*/
  char* inet;                        /* System short-circuit current, kA */
  char* snet;                       /* System short-circuit power, kVA */
  char* ir;                           /* Breaker current, kA */
  float fuh, ful, finet, fsnet;
  if ((uh = GETPROPMAC(cur, HIGH_VOLTAGE)) &&
      (ul = GETPROPMAC(cur, LOW_VOLTAGE))) {
      if (SCANMAC(uh,fuh) &&
	  SCANMAC(ul,ful)) {
	  if ((inet = GETPROPMAC(cur, SC_CURRENT)) ||
	      (ir = GETPROPMAC(cur, SC_BREAKER))) {
	      if (ir) {
		VALIDATOR(ir,finet);
	      }
	      if (inet) {
		VALIDATOR(inet,finet);
	      }
	      node->reactance = (double)((ful * ful) / (TRADICAL * finet * fuh)); /* TODO Zero-division*/
	    }
	}
    }
  if ((snet = GETPROPMAC(cur, SC_POWER)) &&
      (ul = GETPROPMAC(cur, LOW_VOLTAGE))) {
    VALIDATOR(snet,fsnet);
    VALIDATOR(ul, ful);
    node->reactance = (double)((ful * ful) / fsnet);
  }
  return EL_TRUE;
}
static int
initTransformer(xmlNodePtr cur, netNodePtr node)
{
  char* ul;			/* Nominal low voltage, kV */
  char* sn;			/* Nominal power, kVA */
  char* uk;			/* Short-circuit voltage, % */
  char* pk;			/* Short-circuit losses, kW */
  char* var;
  float ful, fsn, fuk, fpk, fval;
  if (uk = GETPROPMAC(cur, SC_VOLTAGE)) {
    VALIDATOR(uk, fuk); /* fuk = (float)uk */
  } else
    fuk = DEFAULT_UK;
  if (pk = GETPROPMAC(cur, SC_POWERLOSS)) {
    VALIDATOR(pk, fpk); /* fuk = (float)uk */
  } else
    fpk = DEFAULT_PK;
  if ((ul = GETPROPMAC(cur, LOW_VOLTAGE)) &&
      (sn = GETPROPMAC(cur, NOM_POWER))) {
    VALIDATOR(ul, ful);
    VALIDATOR(sn, fsn);
    node->resistance = (double)(1000000 * fpk * (ful * ful) / (fsn * fsn));
    node->reactance = (double)(10000 * (ful * ful) * sqrt(pow(fuk, 2) - pow((100 * fpk / fsn ), 2)) / fsn);
    var = GETPROPMAC(cur, SCHEMA);
    if (!var || STREQ(var, "dy")) {
      node->zresistance = node->resistance;
      node->zreactance = node->reactance;
    } else {
      DEFAULT_INIT(cur, var, fval, zresistance, "R0", node);
      DEFAULT_INIT(cur, var, fval, zreactance, "X0", node);
    }
    return EL_TRUE;
  }
  DEFAULT_INIT(cur, var, fval, resistance, "R", node);
  DEFAULT_INIT(cur, var, fval, reactance, "X", node);
  DEFAULT_INIT(cur, var, fval, zresistance, "R0", node);
  DEFAULT_INIT(cur, var, fval, zreactance, "X0", node);
  return EL_TRUE;
}
static int
initBus(xmlNodePtr cur, netNodePtr node)
{
  float fval;
  char* var;
  double len;
  if (var = GETPROPMAC(cur, "len")) {
    VALIDATOR(var, fval);
    len = (double)fval;
  }
  DEFAULT_INIT(cur, var, fval, resistance, BUS_RES, node);
  DEFAULT_INIT(cur, var, fval, reactance, BUS_REACT, node);
  DEFAULT_INIT(cur, var, fval, zresistance, "R0", node);
  DEFAULT_INIT(cur, var, fval, zreactance, "X0", node);
  node->resistance = len * node->resistance;
  node->reactance = len * node->reactance;
  node->zresistance = len * node->zresistance;
  node->zreactance = len * node->zreactance;
  return EL_TRUE;
}
static int
initLine(xmlNodePtr cur, netNodePtr node)
{
  float fval;
  double dval;
  char* var;
  DEFAULT_INIT(cur, var, fval, resistance, "R", node);
  DEFAULT_INIT(cur, var, fval, reactance, "X", node);
  DEFAULT_INIT(cur, var, fval, zresistance, "R0", node);
  DEFAULT_INIT(cur, var, fval, zreactance, "X0", node);
  if (var = GETPROPMAC(cur, "len")) {
    VALIDATOR(var, fval);
    dval = (double)fval;
    node->resistance = dval * node->resistance;
    node->reactance = dval * node->reactance;
    node->zresistance = dval * node->zresistance;
    node->zreactance = dval * node->zreactance;
  } else
    return EL_FALSE;
  return EL_TRUE;
}
static int
initResist(xmlNodePtr cur, netNodePtr node)
{
  float fval;
  char* var;
  DEFAULT_INIT(cur, var, fval, resistance, "R", node);
  DEFAULT_INIT(cur, var, fval, reactance, "X", node);
  return EL_TRUE;
}
static int
initReactor(xmlNodePtr cur, netNodePtr node)
{
  char *dp, *in, *L, *M, *var;
  float fdp, fin, fL, fM, fvar;
  if ((dp = GETPROPMAC(cur, R_POWERLOSS)) &&
      (in = GETPROPMAC(cur, R_CURRENT))) {
    VALIDATOR(dp, fdp);
    VALIDATOR(in, fin);
    node->resistance = (double)(1000 * fdp / (fin * fin));
  } else
    DEFAULT_INIT(cur, var, fvar, resistance, "R", node);
  if ((L = GETPROPMAC(cur, R_INDUCTANCE)) &&
      (M = GETPROPMAC(cur, R_MINDUCTANCE))) {
    VALIDATOR(L, fL);
    VALIDATOR(M, fM);
    node->reactance = (double)(1000 * W *(fL - fM));
  } else
    DEFAULT_INIT(cur, var, fvar, reactance, "X", node);
  node->zreactance = node->reactance;
  node->zresistance = node->resistance;
  return EL_TRUE;
}
