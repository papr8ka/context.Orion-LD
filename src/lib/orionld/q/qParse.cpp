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
#include "kbase/kMacros.h"                                     // K_FT
#include "kalloc/kaStrdup.h"                                   // kaStrdup
}

#include "logMsg/logMsg.h"                                     // LM_*

#include "orionld/context/orionldAttributeExpand.h"            // orionldAttributeExpand
#include "orionld/context/orionldSubAttributeExpand.h"         // orionldSubAttributeExpand
#include "orionld/common/orionldState.h"                       // orionldState
#include "orionld/q/QNode.h"                                   // QNode
#include "orionld/q/qPresent.h"                                // qListPresent, qTreePresent
#include "orionld/q/qRelease.h"                                // qRelease
#include "orionld/q/qParse.h"                                  // Own interface



// ----------------------------------------------------------------------------
//
// varFix -
//
// - If simple attribute name - all OK
// - If attr.b.c, then 'attr' must be extracted, expanded and then '.md.b.c' appended
// - If attr[b.c], then 'attr' must be extracted, expanded and then '.b.c' appended
//
// After implementing expansion in metadata names, for attr.b.c, 'b' needs expansion also
//
static char* varFix(char* varPath, bool forDb, char** detailsP)
{
  char* cP            = varPath;
  char* attrNameP     = varPath;
  char* firstDotP     = NULL;
  char* secondDotP    = NULL;
  char* startBracketP = NULL;
  char* endBracketP   = NULL;
  char* mdNameP       = NULL;
  char* rest          = NULL;
  char  fullPath[1100];

  LM_TMP(("Q: In varFix for varPath '%s'", varPath));
  //
  // Cases (for forDb==true):
  //
  // 1. A                 => single attribute     => attrs.A.value
  // 2. A[B]    (qPath)   => B is inside A.value  => attrs.A.value.B
  // 3. A.B     (mqPath)  => B is a metadata      => attrs.A.md.B.value   OR  "attrs.A.md.B" if B is TIMESTAMP and forDb==false
  // 4. A.B.C   (mqPath)  => B is a metadata      => attrs.A.md.B.value.C
  //
  // - There can be only one '[' in the path
  // - If '[' is found, then there must be a matching ']'
  //
  // So, we need to know:
  // - attrName
  // - mdName (if NO '[' in path)
  // - rest
  //   - For "A.B.C",  attrName == "A",                rest == "B.C"
  //   - For "A[B.C]", attrName == "A", mdName == "B", rest == "C"
  //   => rest == After first '.'
  //
  while (*cP != 0)
  {
    if (*cP == '.')
    {
      if (firstDotP == NULL)
        firstDotP = cP;
      else if (secondDotP == NULL)
        secondDotP = cP;
    }
    else if (*cP == '[')
    {
      if (startBracketP != NULL)
      {
        *detailsP = (char*) "More than one start brackets found";
        return NULL;
      }
      startBracketP  = cP;
    }
    else if (*cP == ']')
    {
      if (endBracketP != NULL)
      {
        *detailsP = (char*) "More than one end brackets found";
        return NULL;
      }
      endBracketP = cP;
    }

    ++cP;
  }

  //
  // Error handling
  //
  if ((startBracketP != NULL) && (endBracketP == NULL))
  {
    *detailsP = (char*) "missing end bracket";
    LM_W(("Bad Input (%s)", *detailsP));
    return NULL;
  }
  else if ((startBracketP == NULL) && (endBracketP != NULL))
  {
    *detailsP = (char*) "end bracket but no start bracket";
    LM_W(("Bad Input (%s)", *detailsP));
    return NULL;
  }
  else if ((firstDotP != NULL) && (startBracketP != NULL) && (firstDotP < startBracketP))
  {
    *detailsP = (char*) "found a dot before a start bracket";
    LM_W(("Bad Input (%s)", *detailsP));
    return NULL;
  }

  //
  // Now we need to NULL out certain characters:
  //
  // Again, four cases:
  // 1. A
  // 2. A[B]
  // 3. A.B   (special case if B is timestamp and if forDb==false)
  // 4. A.B.C
  //
  // - If A:  ((startBracketP == NULL) && (firstDotP == NULL))
  //   - attribute: A
  //   => nothing to NULL our
  //   - fullPath:  A-EXPANDED.value
  //
  // - if A[B]: (startBracketP != NULL)
  //   - attribute: A
  //   - rest:      B
  //   => Must NULL out '[' and ']'
  //   - fullPath:  A-EXPANDED.value.B
  //
  // - if A.B ((startBracketP == NULL) && (firstDotP != NULL) && (secondDotP == NULL))
  //   - attribute:  A
  //   - metadata:   B
  //   => Must NULL out the first dot
  //   - fullPath:  A-EXPANDED.md.B.value
  //
  // - if A.B.C ((startBracketP == NULL) && (firstDotP != NULL) && (secondDotP != NULL))
  //   - attribute:  A
  //   - metadata:   B
  //   - rest:       C
  //   => Must NULL out the first two dots
  //   - fullPath:  A-EXPANDED.md.B.value.C
  //
  int caseNo = 0;

  if ((startBracketP == NULL) && (firstDotP == NULL))
    caseNo = 1;
  else if (startBracketP != NULL)
  {
    *startBracketP = 0;
    *endBracketP   = 0;
    rest           = &startBracketP[1];
    caseNo         = 2;
  }
  else if (firstDotP != NULL)
  {
    if (secondDotP == NULL)
    {
      *firstDotP = 0;
      mdNameP    = &firstDotP[1];
      caseNo     = 3;
    }
    else
    {
      *firstDotP  = 0;
      mdNameP     = &firstDotP[1];
      *secondDotP = 0;
      rest        = &secondDotP[1];
      caseNo = 4;
    }
  }

  if (caseNo == 0)
  {
    *detailsP = (char*) "invalid RHS in Q-filter";
    return NULL;
  }

  //
  // All OK - let's compose ...
  //
  char* longNameP = orionldAttributeExpand(orionldState.contextP, attrNameP, true, NULL);

  //
  // Now 'longNameP' needs to be adjusted forthe DB model, that changes '.' for '=' in the database.
  // If we use 'longNameP', that points to the context-cache, we will destroy the cache. We have to work on a copy
  //
  char longName[512];    // 512 seems like an OK limit for max length of an expanded attribute name
  char mdLongName[512];  // 512 seems like an OK limit for max length of an expanded metadata name

  strncpy(longName, longNameP, sizeof(longName) - 1);

  // Turn '.' into '=' for longName
  char* sP = longName;
  while (*sP != 0)
  {
    if (*sP == '.')
      *sP = '=';
    ++sP;
  }

  //
  // Expand mdName if present
  //
  if (mdNameP != NULL)
  {
    if (strcmp(mdNameP, "observedAt") != 0)  // Don't expand "observedAt", nor ...
    {
      char* mdLongNameP  = orionldSubAttributeExpand(orionldState.contextP, mdNameP, true, NULL);

      strncpy(mdLongName, mdLongNameP, sizeof(mdLongName) - 1);

      // Turn '.' into '=' for md-longname
      char* sP = mdLongName;
      while (*sP != 0)
      {
        if (*sP == '.')
          *sP = '=';
        ++sP;
      }

      mdNameP = mdLongName;
    }
  }

  if (forDb)
  {
    if (caseNo == 1)
      snprintf(fullPath, sizeof(fullPath) - 1, "attrs.%s.value", longName);
    else if (caseNo == 2)
      snprintf(fullPath, sizeof(fullPath) - 1, "attrs.%s.value.%s", longName, rest);
    else if (caseNo == 3)
      snprintf(fullPath, sizeof(fullPath) - 1, "attrs.%s.md.%s.value", longName, mdNameP);
    else
      snprintf(fullPath, sizeof(fullPath) - 1, "attrs.%s.md.%s.value.%s", longName, mdNameP, rest);
  }
  else
  {
    // If observedAt, createdAt, modifiedAt, unitCode, ...   No ".value" must be appended
    if (caseNo == 3)
    {
      LM_TMP(("Q: case 3 for attr '%s', sub-attr: '%s'", longName, mdNameP));
      if ((strcmp(mdNameP, "observedAt") == 0) || (strcmp(mdNameP, "modifiededAt") == 0) || (strcmp(mdNameP, "createdAt") == 0))
      {
        LM_TMP(("Q: case 5 !"));
        caseNo = 5;
      }
    }

    if (caseNo == 1)
      snprintf(fullPath, sizeof(fullPath) - 1, "%s.value", longName);
    else if (caseNo == 2)
      snprintf(fullPath, sizeof(fullPath) - 1, "%s.value.%s", longName, rest);
    else if (caseNo == 3)
      snprintf(fullPath, sizeof(fullPath) - 1, "%s.%s.value", longName, mdNameP);
    else if (caseNo == 4)
      snprintf(fullPath, sizeof(fullPath) - 1, "%s.%s.value.%s", longName, mdNameP, rest);
    else if (caseNo == 5)
      snprintf(fullPath, sizeof(fullPath) - 1, "%s.%s", longName, mdNameP);
  }

  if (orionldState.useMalloc == true)
  {
    free(varPath);
    char* fp = strndup(fullPath, sizeof(fullPath) - 1);
    LM_TMP(("Q: From varFix returning '%s'", fp));
    return fp;
  }

  LM_TMP(("Q: From varFix returning '%s'", fullPath));
  return kaStrdup(&orionldState.kalloc, fullPath);
}



// -----------------------------------------------------------------------------
//
// qClone - move to ... QNode.cpp or its own module
//
QNode* qClone(QNode* original)
{
  QNode* cloneP = qNode(original->type);

  if (orionldState.useMalloc == true)
  {
    if (original->type == QNodeVariable)
    {
      cloneP->value.v = strdup(original->value.v);
      LM_TMP(("strdupped VariablePath '%s' at %p for qNode at %p", cloneP->value.v, cloneP->value.v, cloneP));
    }
    else if (original->type == QNodeStringValue)
    {
      cloneP->value.s = strdup(original->value.s);
      LM_TMP(("strdupped String '%s' at %p for qNode at %p", cloneP->value.s, cloneP->value.s, cloneP));
    }
    else
      cloneP->value  = original->value;
  }
  else
    cloneP->value  = original->value;

  cloneP->next   = NULL;

  return cloneP;
}



// ----------------------------------------------------------------------------
//
// qNodeAppend - append 'childP' to 'container'
//
static QNode* qNodeAppend(QNode* container, QNode* childP, bool clone = true)
{
  QNode* lastP  = container->value.children;
  QNode* cloneP;

  if (clone)
  {
    cloneP = qClone(childP);

    if (cloneP == NULL)
      return NULL;
  }
  else
  {
    LM_TMP(("Not cloning '%s' at %p (comes from qNodeV)", qNodeType(childP->type), childP));
    cloneP = childP;
  }

  if (lastP == NULL)  // No children
    container->value.children = cloneP;
  else
  {
    while (lastP->next != NULL)
      lastP = lastP->next;

    lastP->next = cloneP;
  }

  return cloneP;
}



// ----------------------------------------------------------------------------
//
// qParse -
//
// Example input strings (s):
// - A1>23
// - P1<1|P2>7
// - (P1<1|P2>7);(A1>23|Q3<0);(R2<=14;F1==7)
// - ((P1<1|P2>7);(A1>23|Q3<0))|(R2<=14;F1==7)
//
// * ';' means '&'
// * On the same parenthesis level, the same op must be used (op: AND|OR)
// *
//
QNode* qParse(QNode* qLexList, QNode* endNodeP, bool forDb, char** titleP, char** detailsP)
{
  QNode*     qNodeV[10]      = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  int        qNodeIx         = 0;
  QNode*     qLexP           = qLexList;
  QNode*     opNodeP         = NULL;
  QNode*     compOpP         = NULL;
  QNode*     leftP           = NULL;
  QNode*     openP           = NULL;
  QNode*     closeP          = NULL;

  while (qLexP != endNodeP)
  {
    if ((compOpP != NULL) && ((qLexP->type == QNodeTrueValue) || (qLexP->type == QNodeFalseValue)))
    {
      if ((compOpP->type != QNodeEQ) && (compOpP->type != QNodeNE))
      {
        *titleP   = (char*) "Invalid Q-Filter";
        *detailsP = (char*) "invalid operator for boolean value";
        qRelease(compOpP);
        qListRelease(qLexList);
        // Free nodes in qNodeV ?
        if (opNodeP != NULL)
          qRelease(opNodeP);

        return NULL;
      }
    }

    switch (qLexP->type)
    {
    case QNodeOpen:
      // Extract all tokens inside the ( )  (both parenthesis on THE SAME LEVEL)
      openP = qLexP;

      if (openP->next == NULL)
      {
        *titleP   = (char*) "Invalid Q-Filter";
        *detailsP = (char*) "empty parenthesis";
        return NULL;
      }

      // Lookup the corresponding ')' - or error if not present
      closeP = openP->next;
      while (true)
      {
        if ((closeP->type == QNodeClose) && (closeP->value.level == openP->value.level))
          break;

        closeP = closeP->next;
        if (closeP == NULL)
        {
          *titleP   = (char*) "Invalid Q-Filter";
          *detailsP = (char*) "mismatching parenthesis - no matching ')'";
          return NULL;
        }
      }
      LM_TMP(("Calling qParse from '%s' to '%s'", qNodeType(openP->next->type), qNodeType(closeP->type)));
      qNodeV[qNodeIx++] = qParse(openP->next, closeP, forDb, titleP, detailsP);

      // Make qLexP point to ')' (will get ->next at the end of the loop)
      qLexP = closeP;
      break;

    case QNodeVariable:
      qLexP->value.v = varFix(qLexP->value.v, forDb, detailsP);
      if ((qLexP->next == NULL) || (qLexP->next->type == QNodeOr) || (qLexP->next->type == QNodeAnd) || (qLexP->next->type == QNodeClose))
      {
        if (compOpP == NULL)
          compOpP = qNode(QNodeExists);

        qNodeAppend(compOpP, qLexP);
        qNodeV[qNodeIx++] = compOpP;
        compOpP = NULL;  // Current copmpOpP is taken - need a new one for the next time
        break;  // As there is no "RHS" - EXIST is a unary operator
      }
      // NO BREAK !!!
    case QNodeStringValue:
    case QNodeIntegerValue:
    case QNodeFloatValue:
    case QNodeTrueValue:
    case QNodeFalseValue:
    case QNodeRegexpValue:
      if (compOpP == NULL)  // Still no operatore - this is on the Left-Hand side - saved for later
      {
        leftP = qLexP;
        LM_TMP(("Saving LHS (of type '%s')", qNodeType(leftP->type)));
      }
      else  // Right hand side
      {
        QNode* rangeP = NULL;
        QNode* commaP = NULL;

        if ((qLexP->next != NULL) && (qLexP->next->type == QNodeRange))
        {
          QNode*    lowerLimit;
          QNode*    upperLimit;

          lowerLimit = qLexP;        // referencing the lower limit
          qLexP      = qLexP->next;  // step over the lower limit
          rangeP     = qLexP;        // referencing the range
          qLexP      = qLexP->next;  // step over the RANGE operator
          upperLimit = qLexP;        // referencing the upper limit

          if (lowerLimit->type != upperLimit->type)
          {
            *titleP   = (char*) "ngsi-ld query language: mixed types in range";
            *detailsP = (char*) qNodeType(upperLimit->type);
            return NULL;
          }

          qNodeAppend(rangeP, lowerLimit);
          qNodeAppend(rangeP, upperLimit);
        }
        else if ((qLexP->next != NULL) && (qLexP->next->type == QNodeComma))
        {
          QNodeType commaValueType  = qLexP->type;

          commaP = qLexP->next;

          while ((qLexP != NULL) && (qLexP->next != NULL) && (qLexP->next->type == QNodeComma))
          {
            QNode* valueP = qLexP;

            qLexP = qLexP->next;  // point to the comma
            qLexP = qLexP->next;  // point to the next value

            if (qLexP->type != commaValueType)
            {
              *titleP   = (char*) "ngsi-ld query language: mixed types in comma-list";
              *detailsP = (char*) qNodeType(qLexP->type);
              return NULL;
            }

            qNodeAppend(commaP, valueP);  // OK to enlist commaP and valueP as qLexP point to after valueP
          }

          //
          // Appending last list item
          //
          qNodeAppend(commaP, qLexP);
        }

        //
        // If operator is '!', there is no LEFT side of the expression
        //
        if (compOpP->type != QNodeNotExists)
        {
          LM_TMP(("Appending saved LHS"));
          qNodeAppend(compOpP, leftP);
        }

        if (rangeP != NULL)
          qNodeAppend(compOpP, rangeP);
        else if (commaP != NULL)
          qNodeAppend(compOpP, commaP);
        else
          qNodeAppend(compOpP, qLexP);

        qNodeV[qNodeIx++] = compOpP;
        compOpP    = NULL;
        leftP      = NULL;
      }
      break;

    case QNodeOr:
    case QNodeAnd:
      if (opNodeP != NULL)
      {
        if (qLexP->type != opNodeP->type)
        {
          *titleP   = (char*) "ngsi-ld query language: mixed AND/OR operators";
          *detailsP = (char*) "please use parenthesis to avoid confusion";
          return NULL;
        }
      }
      else
        opNodeP = qClone(qLexP);
      break;

    case QNodeEQ:
    case QNodeNE:
    case QNodeGE:
    case QNodeGT:
    case QNodeLE:
    case QNodeLT:
    case QNodeMatch:
    case QNodeNoMatch:
      compOpP = qClone(qLexP);
      break;

    case QNodeNotExists:
      compOpP = qClone(qLexP);
      break;

    default:
      *titleP   = (char*) "parse error: unsupported node type";
      *detailsP = (char*) qNodeType(qLexP->type);
      return NULL;
      break;
    }
    LM_TMP(("Current token: %s", qNodeType(qLexP->type)));
    qLexP = qLexP->next;
    if (qLexP != NULL)
      LM_TMP(("Next token: %s", qNodeType(qLexP->type)));
    else
      LM_TMP(("Next token: NULL"));
  }

  if (opNodeP != NULL)
  {
    for (int ix = 0; ix < qNodeIx; ix++)
      qNodeAppend(opNodeP, qNodeV[ix], false);  // 3rd param FALSE: qNodeV contains compOp's - all already cloned
    return opNodeP;
  }

  if (qNodeV[0] == NULL)
  {
    *titleP   = (char*) "Parse Error in Q-Expression";
    *detailsP = (char*) "Unfinished statement?";
    return NULL;
  }

  return qNodeV[0];
}



#if 0
//
// qTreePresent - might be useful for debugging in the future
// qLexRender   - might be useful for debugging in the future
//
static const char* indentV[] = {
  "",
  "  ",
  "    ",
  "      ",
  "        ",
  "          ",
  "            ",
  "              ",
  "                "
};



// ----------------------------------------------------------------------------
//
// qTreePresent - DEBUG function to see an entire QNode Tree in the log file
//
static void qTreePresent(QNode* qNodeP, int level)
{
  const char* indent = indentV[level];

  if      (qNodeP->type == QNodeVariable)       LM_K(("Q: %sVAR: '%s'",   indent, qNodeP->value.v));
  else if (qNodeP->type == QNodeIntegerValue)   LM_K(("Q: %sINT: %lld",   indent, qNodeP->value.i));
  else if (qNodeP->type == QNodeFloatValue)     LM_K(("Q: %sFLT: %f",     indent, qNodeP->value.f));
  else if (qNodeP->type == QNodeStringValue)    LM_K(("Q: %sSTR: '%s'",   indent, qNodeP->value.s));
  else if (qNodeP->type == QNodeTrueValue)      LM_K(("Q: %sTRUE",        indent));
  else if (qNodeP->type == QNodeFalseValue)     LM_K(("Q: %sFALSE",       indent));
  else if (qNodeP->type == QNodeRegexpValue)    LM_K(("Q: %sRE: '%s'",    indent, qNodeP->value.re));
  else
  {
    ++level;
    LM_K(("Q: %s%s", indent, qNodeType(qNodeP->type)));
    for (QNode* childP = qNodeP->value.children; childP != NULL; childP = childP->next)
      qTreePresent(childP, level);
  }
}



// -----------------------------------------------------------------------------
//
// qValueGet -
//
static int qValueGet(QNode* qLexP, char* buf, int bufLen)
{
  if (qLexP->type == QNodeVariable)
    snprintf(buf, bufLen, "VAR:%s", qLexP->value.v);
  else if (qLexP->type == QNodeIntegerValue)
    snprintf(buf, bufLen, "INT:%lld", qLexP->value.i);
  else if (qLexP->type == QNodeFloatValue)
    snprintf(buf, bufLen, "FLT:%f", qLexP->value.f);
  else if (qLexP->type == QNodeTrueValue)
    strncpy(buf, "TRUE", bufLen);
  else if (qLexP->type == QNodeFalseValue)
    strncpy(buf, "FALSE", bufLen);
  else
    strncpy(buf, qNodeType(qLexP->type), bufLen);

  return strlen(buf);
}



// ----------------------------------------------------------------------------
//
// qLexRender - render the linked list into a char buffer
//
static bool qLexRender(QNode* qLexList, char* buf, int bufLen)
{
  QNode*  qLexP = qLexList;
  int     left  = bufLen;
  int     sz;

  if (qLexP == NULL)
    return false;

  sz = qValueGet(qLexP, buf, left);
  left -= sz;

  qLexP = qLexP->next;

  while (qLexP != NULL)
  {
    if (left <= 4)
      return false;

    strcpy(&buf[bufLen - left], " -> ");
    left -= 4;

    sz = qValueGet(qLexP, &buf[bufLen - left], left);
    left -= sz;
    if (left <= 0)
      return false;

    qLexP = qLexP->next;
  }

  return true;
}
#endif