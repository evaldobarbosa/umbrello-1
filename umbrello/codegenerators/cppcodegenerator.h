
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
 *      Date   : Thu Jun 19 2003
 */



#ifndef CPPCODEGENERATOR_H
#define CPPCODEGENERATOR_H

#include <qstring.h>
#include "../codeviewerstate.h"
#include "../codegenerator.h"
#include "../umldoc.h"
#include "cppmakecodedocument.h"
#include "cppcodegenerationpolicy.h"

class CodeViewerDialog;
class CPPHeaderCodeDocument;
class CodeBlockWithComments;
class KConfig;

class CPPCodeGenerator : public CodeGenerator
{
	Q_OBJECT
public:

	static const bool DEFAULT_BUILD_MAKEFILE;

	// Constructors/Destructors
	//  

	/**
	 * Basic Constructor
	 */
	CPPCodeGenerator ( UMLDoc * doc , const char * name);

	/**
	 * Empty Destructor
	 */
	virtual ~CPPCodeGenerator ( );

	/**
	 * Set the value of m_createMakefile
	 * @param new_var the new value of m_createMakefile
	 */
	void setCreateProjectMakefile ( bool new_var );

	/**
	 * Get the value of m_createMakefile
	 * @return the value of m_createMakefile
	 */
	bool getCreateProjectMakefile ( );

	// Public attribute accessor methods
	//  

	/**
	 * A utility method to get the cppCodeGenerationPolicy()->getCommentStyle() value.
	 */
	CPPCodeGenerationPolicy::CPPCommentStyle getCommentStyle ( );

	/**
	 * A utility method to get the javaCodeGenerationPolicy()->getAutoGenerateConstructors() value.
	 */
	bool getAutoGenerateConstructors ( );

	/**
	 * A utility method to get the javaCodeGenerationPolicy()->getAutoGenerateAccessors() value.
	 */
	bool getAutoGenerateAccessors( );

	QString fixTypeName(QString string);

	/**
	 * Add a header CodeDocument object from m_headercodedocumentVector List
	 */
	bool addHeaderCodeDocument ( CPPHeaderCodeDocument * doc ); 

	/** Converts the passed name into an acceptable class name for CPP code.
	 */
	QString getCPPClassName (QString name); 

	/**
	 * Remove a header CodeDocument object from m_headercodedocumentVector List
 	 */
	bool removeHeaderCodeDocument ( CPPHeaderCodeDocument * remove_object );

	virtual bool isType (QString & type);

	QString getLanguage(); 

	// should be 'static' method
	QString scopeToCPPDecl(Uml::Scope scope);

	// generate 2 different types of classifier code documents.
	CodeDocument * newClassifierCodeDocument (UMLClassifier * classifier);
	CodeComment * newCodeComment ( CodeDocument * doc);
	CPPHeaderCodeDocument * newHeaderClassifierCodeDocument (UMLClassifier * classifier);

       /** Get the editing dialog for this code document
	 */
	virtual CodeViewerDialog * getCodeViewerDialog( QWidget* parent, CodeDocument * doc,
							Settings::CodeViewerState state);

       /**
	 * Write out all code documents to file as appropriate.
	 */
	virtual void writeCodeToFile ( );

	// this method is here to provide class wizard the
	// ability to write out only those classes which
	// are selected by the user.
	virtual void writeCodeToFile(UMLClassifierList &list);

	/**
	 * Add C++ primitives as datatypes
	 */
	void createDefaultDatatypes();

	/**
	 * Save the XMI representation of this object
	 */
	virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

	/**
	 * Check whether the given string is a reserved word for the
	 * language of this code generator
	 *
	 * @param rPossiblyReservedKeyword  The string to check.
	 */
	virtual bool isReservedKeyword(const QString & rPossiblyReservedWord);

protected:

	/**
	 * @return	CPPMakeCodeDocument
	 * @param	this 
	 */
	CPPMakefileCodeDocument * newMakefileCodeDocument ( );

	/**
	 * Find a cppheadercodedocument by the given classifier.
	 * @return      CPPHeaderCodeDocument
	 * @param       classifier
	 */
	CPPHeaderCodeDocument * findHeaderCodeDocumentByClassifier (UMLClassifier * classifier ); 

	void initFromParentDocument( );

private:

	bool m_createMakefile;

	// a separate list for recording the header documents
	QPtrList<CodeDocument> m_headercodedocumentVector;

	void initAttributes ( ) ;

public slots:

	/** These 2 functions check for adding or removing objects to the UMLDocument 
	 * they are need to be overridden here because unlike in the Java (or most other lang)
	 * we add 2 types of classifiercodedocument per classifier,
	 * e.g. a "source" and a "header" document.
	 */
	virtual void checkAddUMLObject (UMLObject * obj);
	virtual void checkRemoveUMLObject (UMLObject * obj);

	/**
	 * Force a synchronize of this code generator, and its present contents, to that of the parent UMLDocument.
	 * "UserGenerated" code will be preserved, but Autogenerated contents will be updated/replaced
	 * or removed as is apppropriate.
	 */
	virtual void syncCodeToDocument ( );

};

#endif // CPPCODEGENERATOR_H
