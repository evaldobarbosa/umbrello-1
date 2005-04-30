
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Mon Sep 1 2003
 */

#include "cppheadercodeoperation.h"

#include "cppcodegenerator.h"
#include "cppheadercodedocument.h"
#include "cppcodedocumentation.h"

// Constructors/Destructors
//

CPPHeaderCodeOperation::CPPHeaderCodeOperation ( CPPHeaderCodeDocument * doc, UMLOperation *parent, const QString & body, const QString & comment )
    : CodeOperation ((ClassifierCodeDocument*)doc, parent, body, comment)
{
	init(doc);
}

CPPHeaderCodeOperation::~CPPHeaderCodeOperation ( ) { }

// Other methods
//

// we basically just want to know whether or not to print out
// the body of the operation.
// In C++ if the operations are inline, then we DO print out
// the body text.
void CPPHeaderCodeOperation::updateContent( )
{
        CPPCodeGenerationPolicy * policy = (CPPCodeGenerationPolicy*) getParentDocument()->getParentGenerator()->getPolicy();
        bool isInlineMethod = policy->getAccessorsAreInline( );

	if(isInlineMethod)
		setText(""); // change whatever it is to "";

}

// we basically want to update the doc and start text of this method
void CPPHeaderCodeOperation::updateMethodDeclaration()
{

	bool isInterface = ((ClassifierCodeDocument*)getParentDocument())->parentIsInterface();
	UMLOperation * o = getParentOperation();

        CPPCodeGenerationPolicy * policy = (CPPCodeGenerationPolicy*) getParentDocument()->getParentGenerator()->getPolicy();
        bool isInlineMethod = policy->getOperationsAreInline( );

	QString endLine = getNewLineEndingChars();

        // first, the comment on the operation, IF its autogenerated/empty
        QString comment = o->getDoc();
        if(comment.isEmpty() && getContentType() == CodeBlock::AutoGenerated)
        {
                UMLAttributeList* paramaters = o->getParmList();
                for(UMLAttributeListIt iterator(*paramaters); iterator.current(); ++iterator) {
                        comment += endLine + "@param " + iterator.current()->getName() + " ";
                        comment += iterator.current()->getDoc();
                }
                getComment()->setText(comment);
        }

	// no return type for constructors
	QString methodReturnType = o->isConstructorOperation() ? QString("") : (o->getTypeName() + QString(" "));
	QString methodName = o->getName();
	QString paramStr = QString("");

	// assemble parameters
        UMLAttributeList * list = getParentOperation()->getParmList();
	int nrofParam = list->count();
	int paramNum = 0;
	for(UMLAttribute* parm = list->first(); parm; parm=list->next())
	{
		QString rType = parm->getTypeName();
		QString paramName = parm->getName();
		QString initialValue = parm->getInitialValue();
		paramStr += rType + " " + paramName;
		if(!initialValue.isEmpty())
			paramStr += "=" + initialValue;

		paramNum++;

		if (paramNum != nrofParam )
			paramStr  += ", ";
	}

        // set start/end method text
        QString startText = methodReturnType+" "+methodName+" ("+paramStr+")";

	// write as an abstract operation if explicitly stated OR if child of interface
	if((isInterface || o->getAbstract()) && !isInlineMethod)
		startText = "virtual " + startText + " = 0";
	else if (o->getStatic())                // prefix with "static" if required
		startText = "static "  + startText;
 	startText += (isInlineMethod ? " {" : ";");

        QString endText = (isInlineMethod ? "}" : "");

	setStartMethodText(startText);
	setEndMethodText(endText);

}

int CPPHeaderCodeOperation::lastEditableLine() {
        ClassifierCodeDocument * doc = (ClassifierCodeDocument*)getParentDocument();
	UMLOperation * o = getParentOperation();
        if(doc->parentIsInterface() || o->getAbstract())
                return -1; // very last line is NOT editable as its a one-line declaration w/ no body in
                        // an interface.
        return 0;
}

void CPPHeaderCodeOperation::init (CPPHeaderCodeDocument * doc )
{

	// lets not go with the default comment and instead use
	// full-blown cpp documentation object instead
        setComment(new CPPCodeDocumentation(doc));

	// these things never change..
        setOverallIndentationLevel(1);

	setText("");
	setEndMethodText("");

        updateMethodDeclaration();
        updateContent();

}

#include "cppheadercodeoperation.moc"
