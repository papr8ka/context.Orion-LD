/*
*
* Copyright 2022 FIWARE Foundation e.V.
*
* This file is part of Orion-LD Context Broker.
*
* Orion-LD Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion-LD Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion-LD Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* orionld at fiware dot org
*
* Author: Ken Zangelin
*/
extern "C"
{
#include "kjson/KjNode.h"                                         // KjNode
#include "kjson/kjLookup.h"                                       // kjLookup
#include "kjson/kjBuilder.h"                                      // kjString, kjObject, kjChildAdd, ...
}

#include "logMsg/logMsg.h"                                        // LM_*

#include "orionld/common/orionldState.h"                          // orionldState
#include "orionld/common/orionldError.h"                          // orionldError
#include "orionld/kjTree/kjTreeLog.h"                             // kjTreeLog
#include "orionld/dbModel/dbModelFromApiAttribute.h"              // dbModelFromApiAttribute
#include "orionld/dbModel/dbModelFromApiAttributeDatasetArray.h"  // Own interface



// -----------------------------------------------------------------------------
//
// dbModelFromApiAttributeDatasetArray -
//
bool dbModelFromApiAttributeDatasetArray(KjNode* attrArrayP, KjNode* dbAttrsP, KjNode* attrAddedV, KjNode* attrRemovedV, bool* ignoreP)
{
  bool     defaultFound  = false;
  KjNode*  datasetArrayP = NULL;
  char*    attrNameEq    = attrArrayP->name;

  //
  // Allocate (if needed) the datasets object "orionldState.datasets"
  //
  if (orionldState.datasets == NULL)
  {
    orionldState.datasets = kjObject(orionldState.kjsonP, "@datasets");
    if (orionldState.datasets == NULL)
    {
      orionldError(OrionldInternalError, "Out of memory", "allocating dataset object for an entity", 500);
      return false;
    }
  }
  else
    datasetArrayP = kjLookup(orionldState.datasets, attrNameEq);

  if (datasetArrayP == NULL)
  {
    datasetArrayP = kjArray(orionldState.kjsonP, attrNameEq);

    if (datasetArrayP == NULL)
    {
      orionldError(OrionldInternalError, "Out of memory", "allocating dataset array for an attribute", 500);
      return false;
    }

    kjChildAdd(orionldState.datasets, datasetArrayP);
  }


  KjNode*  attrP = attrArrayP->value.firstChildP;
  KjNode*  next;
  while (attrP != NULL)
  {
    next = attrP->next;

    kjTreeLog(attrP, "attr instance");
    if (next != NULL)
      kjTreeLog(next, "next attr instance");

    KjNode* datasetIdNodeP = kjLookup(attrP, "datasetId");

    if (datasetIdNodeP == NULL)  // Default instance
    {
      if (defaultFound == true)
      {
        orionldError(OrionldBadRequestData, "More than one attribute instances without datasetId", attrP->name, 400);
        return false;
      }

      if (dbModelFromApiAttribute(attrP, dbAttrsP, attrAddedV, attrRemovedV, ignoreP, false) == false)
        return false;

      defaultFound = true;
    }
    else
    {
      kjChildRemove(attrArrayP, attrP);  // Remove the attribute instance from the attribute ...
      kjChildAdd(datasetArrayP, attrP);  // ... And insert it under @datasets::attrName

      //
      // The DB Model for datasetId instances is just as the API - only need to add the timestamps
      // If any of the timestamps are already present in the incoing payload ...
      // Should they be removed here or are they already removed by pCheckAttribute?
      // For now, I remove them here - then we'll see ...
      //
      KjNode* createdAtP  = kjLookup(attrP, "createdAt");
      KjNode* modifiedAtP = kjLookup(attrP, "modifiedAt");

      if (createdAtP  != NULL) kjChildRemove(attrP, createdAtP);
      if (modifiedAtP != NULL) kjChildRemove(attrP, modifiedAtP);

      createdAtP  = kjFloat(orionldState.kjsonP, "createdAt",  orionldState.requestTime);
      modifiedAtP = kjFloat(orionldState.kjsonP, "modifiedAt", orionldState.requestTime);

      kjChildAdd(attrP, createdAtP);
      kjChildAdd(attrP, modifiedAtP);
    }

    attrP = next;
  }

  // No datasets?
  if (datasetArrayP->value.firstChildP == NULL)
    kjChildRemove(orionldState.datasets, datasetArrayP);

  //
  // The array of the attribute now needs to be an object - just the default attribute is left in there -
  // all other instances (with datasetId) have been moved to datasetArrayP (that's inside orionldState.datasets).
  // Unless it is empty of course ...
  //
  if (attrArrayP->value.firstChildP != NULL)
  {
    attrArrayP->type  = attrArrayP->value.firstChildP->type;
    attrArrayP->value = attrArrayP->value.firstChildP->value;
  }

  return true;
}

