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
#include <string.h>                                              // strdup

#include <string>                                                // std::string, due to cSubP->expression.stringFilter.parse()

extern "C"
{
#include "kjson/KjNode.h"                                        // kjBufferCreate
#include "kjson/kjLookup.h"                                      // kjLookup
#include "kjson/kjRender.h"                                      // kjFastRender
}

#include "logMsg/logMsg.h"                                       // LM_*

#include "cache/subCache.h"                                      // CachedSubscription, subCacheItemInsert
#include "common/RenderFormat.h"                                 // stringToRenderFormat

#include "orionld/q/QNode.h"                                     // QNode
#include "orionld/context/OrionldContext.h"                      // OrionldContext
#include "orionld/common/mimeTypeFromString.h"                   // mimeTypeFromString
#include "orionld/common/urlParse.h"                             // urlParse
#include "orionld/common/orionldState.h"                         // orionldState
#include "orionld/common/subCacheApiSubscriptionInsert.h"        // Own interface



// -----------------------------------------------------------------------------
//
// subCacheApiSubscriptionInsert -
// subCacheDbSubscriptionInsert - later ...
//
CachedSubscription* subCacheApiSubscriptionInsert(KjNode* apiSubscriptionP, QNode* qTree, OrionldContext* contextP)
{
  CachedSubscription* cSubP = new CachedSubscription();

  cSubP->tenant              = (orionldState.in.tenant == NULL)? NULL : strdup(orionldState.in.tenant);
  cSubP->servicePath         = strdup("/#");
  cSubP->qP                  = qTree;
  cSubP->contextP            = contextP;
  cSubP->ldContext           = contextP->url;

  KjNode* subscriptionIdP    = kjLookup(apiSubscriptionP, "_id");  // "id" was changed to "_id" by orionldPostSubscriptions to accomodate the DB insertion
  KjNode* subscriptionNameP  = kjLookup(apiSubscriptionP, "subscriptionName");
  KjNode* descriptionP       = kjLookup(apiSubscriptionP, "description");
  KjNode* entitiesP          = kjLookup(apiSubscriptionP, "entities");
  KjNode* watchedAttributesP = kjLookup(apiSubscriptionP, "watchedAttributes");
  KjNode* qP                 = kjLookup(apiSubscriptionP, "q");
  KjNode* mqP                = kjLookup(apiSubscriptionP, "mq");
  KjNode* ldqP               = kjLookup(apiSubscriptionP, "ldQ");
  KjNode* geoqP              = kjLookup(apiSubscriptionP, "geoQ");
  KjNode* isActiveP          = kjLookup(apiSubscriptionP, "isActive");
  KjNode* notificationP      = kjLookup(apiSubscriptionP, "notification");
  KjNode* expiresAtP         = kjLookup(apiSubscriptionP, "expiresAt");
  KjNode* throttlingP        = kjLookup(apiSubscriptionP, "throttling");
  KjNode* langP              = kjLookup(apiSubscriptionP, "lang");

  if (subscriptionIdP != NULL)
    cSubP->subscriptionId = strdup(subscriptionIdP->value.s);

  if (subscriptionNameP != NULL)
    cSubP->name = subscriptionNameP->value.s;

  if (descriptionP != NULL)
    cSubP->description = strdup(descriptionP->value.s);

  if (qP != NULL)
    cSubP->expression.q = qP->value.s;
  if (mqP != NULL)
    cSubP->expression.mq = qP->value.s;
  if (ldqP != NULL)
    cSubP->qText = strdup(qP->value.s);


  if (isActiveP != NULL)
  {
    cSubP->isActive = isActiveP->value.b;
    if (isActiveP->value.b == false)
      cSubP->status = "inactive";
    else
      cSubP->status = "active";
  }
  else
  {
    cSubP->status   = "active";
    cSubP->isActive = true;
  }


  if (expiresAtP != NULL)
  {
    // "expiresAt" has already been translated to double?
    double ts;
    if (expiresAtP->type == KjString)
      ts = parse8601Time(expiresAtP->value.s);
    else
      ts = expiresAtP->value.f;

    cSubP->expirationTime = ts;

    if ((cSubP->expirationTime > 0) && (cSubP->expirationTime < orionldState.requestTime))
    {
      cSubP->status   = "expired";
      cSubP->isActive = false;
    }
  }
  else
    cSubP->expirationTime = -1;

  if (throttlingP != NULL)
    cSubP->throttling = (throttlingP->type == KjFloat)? throttlingP->value.f : throttlingP->value.i;

  if (langP != NULL)
    cSubP->lang = langP->value.s;

  if (geoqP != NULL)
  {
    KjNode* geometryP    = kjLookup(geoqP, "geometry");
    KjNode* coordinatesP = kjLookup(geoqP, "coordinates");
    KjNode* georelP      = kjLookup(geoqP, "georel");
    KjNode* geopropertyP = kjLookup(geoqP, "geoproperty");

    if (geometryP != NULL)
      cSubP->expression.geometry = geometryP->value.s;

    if (georelP != NULL)
      cSubP->expression.georel = georelP->value.s;

    if (geopropertyP != NULL)
      cSubP->expression.geoproperty = geopropertyP->value.s;

    if (coordinatesP != NULL)
    {
      if (coordinatesP->type == KjArray)
      {
        char coords[1024];
        kjFastRender(coordinatesP, coords);
        cSubP->expression.coords = coords;  // Not sure this is 100% correct format, but is it used? DB is used for Geo ...
      }
    }
  }

  if (watchedAttributesP != NULL)
  {
    for (KjNode* watchedAttributeP = watchedAttributesP->value.firstChildP; watchedAttributeP != NULL; watchedAttributeP = watchedAttributeP->next)
    {
      cSubP->notifyConditionV.push_back(watchedAttributeP->value.s);
    }
  }

  if (entitiesP != NULL)
  {
    for (KjNode* eSelectorP = entitiesP->value.firstChildP; eSelectorP != NULL; eSelectorP = eSelectorP->next)
    {
      KjNode*       idP         = kjLookup(eSelectorP, "id");
      KjNode*       idPatternP  = kjLookup(eSelectorP, "idPattern");
      KjNode*       typeP       = kjLookup(eSelectorP, "type");
      char*         id          = (idP        != NULL)? idP->value.s : (char*) "";
      const char*   idPattern   = (idPatternP != NULL)? idPatternP->value.s : "";
      const char*   type        = (typeP      != NULL)? typeP->value.s : "";
      const char*   isPattern   = "false";

      if (idP == NULL)
      {
        id        = (char*) ((idPatternP != NULL)? idPattern : ".*");
        isPattern = (char*) "true";
      }

      EntityInfo* eP = new EntityInfo(id, type, isPattern, false);
      cSubP->entityIdInfos.push_back(eP);
    }
  }

  if (notificationP != NULL)
  {
    KjNode* attributesP = kjLookup(notificationP, "attributes");
    KjNode* formatP     = kjLookup(notificationP, "format");
    KjNode* endpointP   = kjLookup(notificationP, "endpoint");

    if (attributesP != NULL)
    {
      for (KjNode* attributeP = attributesP->value.firstChildP; attributeP != NULL; attributeP = attributeP->next)
      {
        cSubP->attributes.push_back(attributeP->value.s);
      }
    }

    if (formatP != NULL)
    {
      cSubP->renderFormat = stringToRenderFormat(formatP->value.s, true);
      if (cSubP->renderFormat == RF_NONE)
        cSubP->renderFormat = RF_NORMALIZED;
    }
    else
      cSubP->renderFormat = RF_NORMALIZED;

    if (endpointP != NULL)  // pCheckSubscription already ensures "endpoint" is present !!!
    {
      KjNode* uriP          = kjLookup(endpointP, "uri");
      KjNode* acceptP       = kjLookup(endpointP, "accept");
      KjNode* receiverInfoP = kjLookup(endpointP, "receiverInfo");
      KjNode* notifierInfoP = kjLookup(endpointP, "notifierInfo");

      if (uriP)  // pCheckSubscription already ensures "uri" is present !!!
      {
        cSubP->httpInfo.url = uriP->value.s;
        cSubP->url = strdup(uriP->value.s);
        urlParse(cSubP->url, &cSubP->protocol, &cSubP->ip, &cSubP->port, &cSubP->rest);
      }

      if (acceptP != NULL)
      {
        uint32_t acceptMask;
        cSubP->httpInfo.mimeType = mimeTypeFromString(acceptP->value.s, NULL, true, false, &acceptMask);
      }
      else
        cSubP->httpInfo.mimeType = JSON;

      if (receiverInfoP != NULL)
      {
        for (KjNode* riP = receiverInfoP->value.firstChildP; riP != NULL; riP = riP->next)
        {
          KjNode* keyP   = kjLookup(riP, "key");
          KjNode* valueP = kjLookup(riP, "value");

          if ((keyP != NULL) && (valueP != NULL))
            keyValueAdd(&cSubP->httpInfo.receiverInfo, keyP->value.s, valueP->value.s);
        }
      }

      if (notifierInfoP != NULL)
      {
        for (KjNode* niP = notifierInfoP->value.firstChildP; niP != NULL; niP = niP->next)
        {
          KjNode* keyP   = kjLookup(niP, "key");
          KjNode* valueP = kjLookup(niP, "value");

          if ((keyP != NULL) && (valueP != NULL))
            keyValueAdd(&cSubP->httpInfo.notifierInfo, keyP->value.s, valueP->value.s);
        }
      }
    }
  }

  //
  // For legacy operations, we also need two Scopes filled:
  // 'q'
  // 'mq'
  //
  if (qP != NULL)
  {
    std::string errorString;
    if (cSubP->expression.stringFilter.parse(qP->value.s, &errorString) == false)
    {
      LM_E(("Subscription '%s': invalid 'q': %s", qP->value.s, cSubP->subscriptionId));
    }
  }

  if (mqP != NULL)
  {
    std::string errorString;
    if (cSubP->expression.mdStringFilter.parse(mqP->value.s, &errorString) == false)
    {
      LM_E(("Subscription '%s': invalid 'mq': %s", mqP->value.s, cSubP->subscriptionId));
    }
  }

  subCacheItemInsert(cSubP);

  return cSubP;
}