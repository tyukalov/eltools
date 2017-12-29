/* library for net
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
#include <stdio.h>


static netNodePtr getNodePoint(netPointsPtr, const char*);


netCircuitPtr
netFindCircuit (netNetworkPtr net, const char* pnt)
{
  netNodePtr nodepoint;
  netCircuitPtr result, var;
  if (nodepoint = getNodePoint(net->points, pnt)) {
    result = (netCircuitPtr)calloc(sizeof(struct netCircuit), 1);
    result->node = nodepoint;
    while(nodepoint->parent)
      {
	var = result;
	result = (netCircuitPtr)calloc(sizeof(struct netCircuit), 1);
	result->node = nodepoint->parent;
	result->next = var;
	nodepoint = nodepoint->parent;
      }
    return result;
  } else
    return (netCircuitPtr)NULL;
}

netNodePtr
getNodePoint (netPointsPtr pnts, const char *pnt)
{
  if (STREQ(pnt, pnts->point)) {
    return pnts->node;
  }
  else if (pnts->next)
    return getNodePoint(pnts->next, pnt);
  else
    return (netNodePtr)NULL;
}

double
getImpedance (netCircuitPtr circuit, netMode mode)
{
  double R = 0,X = 0;
  while(circuit)
    {
      R = R + circuit->node->resistance;
      X = X + circuit->node->reactance;
      if (mode == NET_ONE_PHASE) {
	R = 2 * R + circuit->node->zresistance;
	X = 2 * X + circuit->node->zreactance;
      }
      circuit = circuit->next;
    }
  return sqrt(R * R + X * X);
}
