/*
*
* Copyright 2019 FIWARE Foundation e.V.
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
#include "kjson/KjNode.h"                                       // KjNode
}

#include "logMsg/logMsg.h"                                      // LM_*

#include "common/RenderFormat.h"                                // RenderFormat, stringToRenderFormat
#include "orionld/common/orionldState.h"                        // orionldState
#include "orionld/common/orionldError.h"                        // orionldError
#include "orionld/context/orionldAttributeExpand.h"             // orionldAttributeExpand
#include "orionld/payloadCheck/PCHECK.h"                        // PCHECK_*
#include "orionld/payloadCheck/fieldPaths.h"                    // SubscriptionNotificationPath, ...
#include "orionld/payloadCheck/pcheckEndpoint.h"                // pCheckEndpoint
#include "orionld/payloadCheck/pCheckNotification.h"            // Own interface



// -----------------------------------------------------------------------------
//
// pCheckNotification -
//
bool pCheckNotification(KjNode* notificationP, bool patch, KjNode** uriPP, KjNode** notifierInfoPP)
{
  KjNode* attributesP = NULL;
  KjNode* formatP     = NULL;
  KjNode* endpointP   = NULL;

  PCHECK_OBJECT(notificationP, 0, NULL, SubscriptionNotificationPath, 400);
  PCHECK_OBJECT_EMPTY(notificationP, 0, NULL, SubscriptionNotificationPath, 400);

  for (KjNode* nItemP = notificationP->value.firstChildP; nItemP != NULL; nItemP = nItemP->next)
  {
    if (strcmp(nItemP->name, "attributes") == 0)
    {
      PCHECK_DUPLICATE(attributesP, nItemP, 0, NULL, SubscriptionNotificationAttributesPath, 400);
      PCHECK_ARRAY(nItemP, 0, NULL, SubscriptionNotificationAttributesPath, 400);
      PCHECK_ARRAY_EMPTY(nItemP, 0, NULL, SubscriptionNotificationAttributesPath, 400);

      for (KjNode* attrP = nItemP->value.firstChildP; attrP != NULL; attrP = attrP->next)
      {
        PCHECK_STRING(attrP, 0, NULL, SubscriptionNotificationAttributesItemPath, 400);
        attrP->value.s = orionldAttributeExpand(orionldState.contextP, attrP->value.s, true, NULL);
      }
    }
    else if (strcmp(nItemP->name, "format") == 0)
    {
      PCHECK_DUPLICATE(formatP, nItemP, 0, NULL, SubscriptionNotificationFormatPath, 400);
      PCHECK_STRING(formatP, 0, NULL, SubscriptionNotificationFormatPath, 400);
      PCHECK_STRING_EMPTY(formatP, 0, NULL, SubscriptionNotificationFormatPath, 400);

      RenderFormat rf = stringToRenderFormat(formatP->value.s, true);
      if ((rf == RF_NONE) || (rf == RF_LEGACY))
      {
        orionldError(OrionldBadRequestData, "Invalid value for 'Subscription::notification::format'", formatP->value.s, 400);
        return false;
      }
    }
    else if (strcmp(nItemP->name, "endpoint") == 0)
    {
      PCHECK_DUPLICATE(endpointP, nItemP, 0, NULL, SubscriptionNotificationEndpointPath, 400);
      PCHECK_OBJECT(endpointP, 0, NULL, SubscriptionNotificationEndpointPath, 400);
      PCHECK_OBJECT_EMPTY(endpointP, 0, NULL, SubscriptionNotificationEndpointPath, 400);
      if (pcheckEndpoint(endpointP, patch, uriPP, notifierInfoPP) == false)
        return false;
    }
    else if (strcmp(nItemP->name, "status") == 0)
    {
      orionldError(OrionldBadRequestData, "Invalid field for Subscription::notification", "'status' is read-only", 400);
      return false;
    }
    else
    {
      orionldError(OrionldBadRequestData, "Invalid field for Subscription::notification", nItemP->name, 400);
      return false;
    }
  }

  if (endpointP == NULL)
  {
    orionldError(OrionldBadRequestData, "Mandatory field missing", SubscriptionNotificationEndpointPath, 400);
    return false;
  }

  return true;
}