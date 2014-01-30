/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Open Source Modelica Consortium (OSMC),
 * c/o Linköpings universitet, Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR 
 * THIS OSMC PUBLIC LICENSE (OSMC-PL) VERSION 1.2. 
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S ACCEPTANCE
 * OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3, ACCORDING TO RECIPIENTS CHOICE. 
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from OSMC, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or  
 * http://www.openmodelica.org, and in the OpenModelica distribution. 
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */
/*
 *
 * @author Martin Sjölund <martin.sjolund@liu.se>
 *
 * RCS: $Id$
 *
 */

#ifndef OMDUMPXML_H
#define OMDUMPXML_H

#include <QFile>
#include <QXmlDefaultHandler>
#include <QHash>

enum OMEquationType {
  start = 0,
  parameter,
  initial,
  regular,
  equationTypeSize
};

const char* OMEquationTypeToString(int);

class OMOperation {
public:
  virtual QString toString();
};

class OMOperationSimplify : public OMOperation
{
public:
  QString before,after;
  OMOperationSimplify(QStringList ops);
  QString toString();
};

class OMOperationScalarize : public OMOperation
{
public:
  int index;
  QString before,after;
  OMOperationScalarize(int _index,QStringList ops);
  QString toString();
};

class OMOperationInline : public OMOperation
{
public:
  QString before,after;
  OMOperationInline(QStringList ops);
  QString toString();
};

class OMOperationSubstitution : public OMOperation
{
public:
  QString before;
  QStringList substitutions;
  OMOperationSubstitution(QStringList ops);
  QString toString();
};

class OMOperationSolved : public OMOperation
{
public:
  QString lhs,rhs;
  OMOperationSolved(QStringList ops);
  QString toString();
};

class OMOperationLinearSolved : public OMOperation
{
public:
  QString text;
  OMOperationLinearSolved(QStringList ops);
  QString toString();
};

class OMOperationSolve : public OMOperation
{
public:
  QString lhs_old,rhs_old;
  QString lhs_new,rhs_new;
  OMOperationSolve(QStringList ops);
  QString toString();
};

class OMOperationDifferentiate : public OMOperation
{
public:
  QString exp,wrt,result;
  OMOperationDifferentiate(QStringList ops);
  QString toString();
};

class OMOperationResidual : public OMOperation
{
public:
  QString lhs,rhs,result;
  OMOperationResidual(QStringList ops);
  QString toString();
};

class OMOperationDummyDerivative : public OMOperation
{
public:
  QString chosen;
  QStringList candidates;
  OMOperationDummyDerivative(QStringList ops);
  QString toString();
};

class OMOperationFlattening : public OMOperation
{
public:
  QString original;
  QString flattened;
  OMOperationFlattening(QStringList ops);
  QString toString();
};

struct OMInfo {
  QString file;
  int lineStart,lineEnd,colStart,colEnd;
  QString toString();
};

struct OMVariable {
  QString name;
  QString comment;
  OMInfo info;
  QStringList types;
  int definedIn[equationTypeSize];
  QList<int> usedIn[equationTypeSize];
  QList<OMOperation*> ops;
  OMVariable();
  OMVariable(const OMVariable& var);
  ~OMVariable();
};

struct OMEquation {
  OMEquationType kind;
  int index;
  QStringList text;
  OMInfo info;
  QStringList defines;
  QStringList depends;
  QList<OMOperation*> ops;
  QList<int> eqs;
  OMEquation();
  OMEquation(const OMEquation& eq);
  ~OMEquation();
  QString toString();
};

class MyHandler : private QXmlDefaultHandler {
public:
  QHash<QString,OMVariable> variables;
  QList<OMEquation> equations;
  bool hasOperationsEnabled;
  MyHandler(QFile &file);
  OMEquation getOMEquation(int index);
private:
  OMVariable currentVariable;
  OMEquation currentEquation;
  QList<int> nestedEquations;
  OMInfo currentInfo;
  OMEquationType currentKind;
  QString currentText;
  QStringList texts;
  QList<OMOperation*> operations;
  int currentIndex;
  static const QSet<QString> equationTags;
  static const QSet<QString> equationPartTags;
  static const QSet<QString> operationTags;
  static const QSet<QString> operationExpTags;
  bool startDocument();
  bool endDocument();
  bool characters( const QString & ch );
  bool startElement( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
  bool endElement( const QString & namespaceURI, const QString & localName, const QString & qName);
  bool fatalError(const QXmlParseException & exception);
};

#endif