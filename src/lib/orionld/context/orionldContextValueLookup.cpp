/*
*
* Copyright 2018 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* iot_support at tid dot es
*
* Author: Ken Zangelin
*/
#include "logMsg/logMsg.h"                                     // LM_*
#include "logMsg/traceLevels.h"                                // Lmt*

extern "C"
{
#include "kjson/KjNode.h"                                      // KjNode
}

#include "orionld/context/OrionldContext.h"                    // OrionldContext
#include "orionld/context/orionldContextList.h"                // orionldContextHead
#include "orionld/context/orionldContextLookup.h"              // orionldContextLookup
#include "orionld/context/orionldContextValueLookup.h"         // Own interface



// -----------------------------------------------------------------------------
//
// orionldContextValueLookup -
//
KjNode* orionldContextValueLookup(OrionldContext* contextP, const char* value)
{
  if (contextP == NULL)
    return NULL;

  if (contextP->tree->type == KjString)
  {
    LM_T(LmtContextValueLookup, ("The context is of type String - must lookup a new context"));

    // Lookup the context
    OrionldContext* dereferencedContextP = orionldContextLookup(contextP->tree->value.s);
    if (dereferencedContextP == NULL)
    {
      LM_E(("Can't find context '%s'", contextP->tree->value.s));
      return NULL;  // Or: download it?
    }

    // Lookup the value
    LM_T(LmtContextValueLookup, ("Now we can search for '%s' (in context '%s'", value, dereferencedContextP->url));
    return orionldContextValueLookup(dereferencedContextP, value);
  }
  else if (contextP->tree->type == KjArray)
  {
    LM_T(LmtContextValueLookup, ("The context is of type Array"));

    for (KjNode* contextNodeP = contextP->tree->children; contextNodeP != NULL; contextNodeP = contextNodeP->next)
    {
      if (contextNodeP->type != KjString)
      {
        LM_E(("The Context Array must only contain JSON Strings"));
        return NULL;
      }

      OrionldContext* dereferencedContextP = orionldContextLookup(contextNodeP->value.s);

      if (dereferencedContextP == NULL)
      {
        LM_W(("Can't find context '%s'", contextNodeP->value.s));
        continue;   // Or: download it?
      }

      // Lookup the value
      LM_T(LmtContextValueLookup, ("Now we can search for '%s' in context '%s'", value, dereferencedContextP->url));
      KjNode* kNodeP = orionldContextValueLookup(dereferencedContextP, value);

      if (kNodeP != NULL)
      {
        LM_T(LmtContextValueLookup, ("Found '%s' in context '%s'", value, dereferencedContextP->url));
        return kNodeP;
      }
    }

    return NULL;
  }
  else if (contextP->tree->type == KjObject)
  {
    //
    // JSON Object - only check here. Do the lookup later.
    // this way, the default context gets the lookup as well
    //
    // Now, either we have an inline context, with many key-values, or
    // the object has a single member "@context".
    //
    LM_T(LmtContextValueLookup, ("Looking up '%s' in the %s context '%s'", value, kjValueType(contextP->tree->type), contextP->url));

    if (contextP->tree->children == NULL)
    {
      LM_E(("ERROR: Context tree is a JSON object, but, it has no children!"));
      return NULL;
    }

    if (contextP->tree->children->next != NULL)
    {
      LM_E(("ERROR: Context tree is a JSON object, but, it has more than one child! (%s)", contextP->url));
      return NULL;
    }

    if (strcmp(contextP->tree->children->name, "@context") != 0)
    {
      LM_E(("Context tree is a JSON object, and it has exactly one child, but its name must be '@context', not '%s'", contextP->tree->children->name));
      return NULL;
    }
  }
  else
  {
    LM_E(("The '@context' is a %s - must be either Object, String or Array", kjValueType(contextP->tree->children->type)));
    return NULL;
  }

  //
  // If we reach this point, the context is a JSON Object
  //
  KjNode*  atContextP    = contextP->tree->children;

  LM_T(LmtContextValueLookup, ("@atcontext is of type '%s'", kjValueType(atContextP->type)));

  if (atContextP->type == KjObject)
  {
    for (KjNode* contextItemP = atContextP->children; contextItemP != NULL; contextItemP = contextItemP->next)
    {
      if ((contextItemP->type != KjString) && (contextItemP->type !=  KjObject))
      {
        LM_E(("Invalid @context - items of contexts must be JSON Strings or jSOn objects - not %s", kjValueType(contextItemP->type)));
        return NULL;
      }

      //
      // Skip members whose value is a string that starts with "@" - they are information, not translations
      //
      if ((contextItemP->type == KjString) && (contextItemP->value.s[0] == '@'))
      {
        LM_T(LmtContextValueLookup, ("Skipping '%s' with value '%s'", contextItemP->name, contextItemP->value.s));
        continue;
      }

      LM_T(LmtContextValueLookup, ("looking for '%s', comparing with '%s'", value, contextItemP->name));
      LM_TMP(("contextItemP at %p", contextItemP));
      LM_TMP(("contextItemP->value.s: '%s'", contextItemP->value.s));
      LM_TMP(("value: '%s'", value));

      if (contextItemP->type == KjObject)
      {
        // Lookup item "@id" inside contextItemP that is a JSON object
      }
      else if (contextItemP->type == KjString)
      {
        if (strcmp(contextItemP->value.s, value) == 0)
        {
          LM_T(LmtContextValueLookup, ("found it!"));
          return contextItemP;
        }
      }
      else
      {
        // error
      }
    }
  }
  else if (atContextP->type == KjArray)
  {
    for (KjNode* contextP = atContextP->children; contextP != NULL; contextP = contextP->next)
    {
      if (contextP->type != KjString)
      {
        LM_E(("Invalid @context - items of contexts must be JSON Strings"));
        return NULL;
      }

      KjNode* nodeP = orionldContextValueLookup(contextP->value.s, value);

      if (nodeP != NULL)
        return nodeP;
    }
  }
  else if (atContextP->type == KjString)
  {
    KjNode* nodeP = orionldContextValueLookup(atContextP->value.s, value);
    if (nodeP != NULL)
      return nodeP;
  }

  LM_T(LmtContextValueLookup, ("found no expansion: returning NULL :("));
  return NULL;
}



// -----------------------------------------------------------------------------
//
// orionldContextValueLookup -
//
KjNode* orionldContextValueLookup(char* contextUrl, const char* value)
{
  OrionldContext* contextP = orionldContextLookup(contextUrl);

  if (contextP == NULL)
    return NULL;

  return orionldContextValueLookup(contextP, value);
}



// -----------------------------------------------------------------------------
//
// orionldContextValueLookup -
//
KjNode* orionldContextValueLookup(KjNode* contextVector, const char* value)
{
  if (contextVector->type != KjArray)
  {
    LM_E(("Not an Array"));
    return NULL;
  }

  KjNode* contextNodeP = contextVector->children;
  KjNode* itemP;

  while (contextNodeP != NULL)
  {
    OrionldContext* contextP = orionldContextLookup(contextNodeP->value.s);

    if (contextP == NULL)
      return NULL;

    itemP = orionldContextValueLookup(contextP, value);
    if (itemP != NULL)
      return itemP;

    contextNodeP = contextNodeP->next;
  }

  return NULL;
}