/***************************************************************************
                          codegenerationwizard.cpp  -  description
                             -------------------
    begin                : Wed Jul 24 2002
    copyright            : (C) 2002 by Paul Hensgen
    email                : phensgen@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qlistview.h>
#include <qfileinfo.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "codegenerationwizard.h"
#include "codegenerationoptionspage.h"
#include "../concept.h"
#include "../codegenerator.h"
#include "../umldoc.h"

CodeGenerationWizard::CodeGenerationWizard(UMLDoc *doc,
					   QPtrList<UMLConcept> *classList,
					   SettingsDlg::CodeGenState codegenState,
					   QDict<GeneratorInfo> ldict,
					   QString activeLanguage,
					   QWidget *parent, const char *name)
		:CodeGenerationWizardBase(parent,name) {
	m_doc = doc;
	m_ldict = ldict;
	m_availableList -> setAllColumnsShowFocus(true);
	m_availableList -> setResizeMode(QListView::AllColumns);
	m_selectedList  -> setAllColumnsShowFocus(true);
	m_selectedList  -> setResizeMode(QListView::AllColumns);
	m_statusList    -> setAllColumnsShowFocus(true);
	m_statusList    -> setResizeMode(QListView::AllColumns);

	m_CodeGenerationOptionsPage = new CodeGenerationOptionsPage(codegenState, ldict,
								    activeLanguage, this);

	insertPage(m_CodeGenerationOptionsPage, i18n("Code Generation Options"), 1);

	QList<UMLConcept> cList = m_doc->getConcepts();

	if(classList) {
		for(UMLConcept *c = classList->first(); c ; c = classList->next()) {
			new QListViewItem( m_selectedList, c->getName());
		}
	} else {
		for(UMLConcept *c = cList.first(); c ; c = cList.next()) {
			new QListViewItem( m_selectedList, c->getName());
		}
	}

	setNextEnabled(page(0),m_selectedList->childCount() > 0);

	setFinishEnabled(page(2),true);
	finishButton()->disconnect();
	finishButton()->setText(i18n("&Generate"));
	connect(finishButton(),SIGNAL(clicked()),this,SLOT(generateCode()));
}

CodeGenerationWizard::~CodeGenerationWizard() {}


void CodeGenerationWizard::selectClass() {
	if( !m_availableList->selectedItem() ) {
		return;
	}
	QString name = m_availableList->selectedItem()->text(0);
	if( !m_selectedList->findItem( name,0 ) ) {
		new QListViewItem(m_selectedList, name);
	}
	m_availableList->removeItem( m_availableList->selectedItem() );
	setNextEnabled(currentPage(),true);
}

void CodeGenerationWizard::deselectClass() {
	if( !m_selectedList->selectedItem() ) {
		return;
	}
	QString name = m_selectedList->selectedItem()->text(0);
	if( !m_availableList->findItem(name, 0) ) {
		new QListViewItem(m_availableList, name);
	}
	if(m_selectedList->childCount() == 0) {
		setNextEnabled(currentPage(),false);
	}
	m_selectedList->removeItem( m_selectedList->selectedItem() );
}

void CodeGenerationWizard::generateCode() {
	backButton()->setEnabled(false);

	CodeGenerator* codeGenerator = generator();

	if (codeGenerator) {
		cancelButton()->setEnabled(false);
		//get current Code Generation Options...
		SettingsDlg::CodeGenState codegenState;
		((CodeGenerationOptionsPage*)page(1))->state(codegenState);

		codeGenerator->setForceDoc(codegenState.forceDoc);
		codeGenerator->setForceSections(codegenState.forceSections);
		codeGenerator->setIncludeHeadings(codegenState.includeHeadings);
		codeGenerator->setHeadingFileDir(codegenState.headingsDir);
		//modname isn't (yet) user configurable so we just use this value
		codeGenerator->setModifyNamePolicy(CodeGenerator::Capitalise);
		codeGenerator->setOutputDirectory(codegenState.outputDir);
		codeGenerator->setOverwritePolicy(codegenState.overwritePolicy);

		connect( codeGenerator, SIGNAL(codeGenerated(UMLConcept*, bool)),
			 this, SLOT(classGenerated(UMLConcept*, bool)) );

		QList<UMLConcept> cList;
		cList.setAutoDelete(false);

		for(QListViewItem *item = m_statusList->firstChild(); item;
		    item = item-> nextSibling()) {
			cList.append((UMLConcept*)m_doc->findUMLObject(Uml::ot_Concept,item->text(0)));
		}
		codeGenerator->generateCode(cList);
		finishButton()->setText(i18n("Finish"));
		finishButton()->disconnect();
		connect(finishButton(),SIGNAL(clicked()),this,SLOT(accept()));

		delete codeGenerator; codeGenerator = NULL;
	}
}

void CodeGenerationWizard::classGenerated(UMLConcept* concept, bool generated) {
	QListViewItem* item = m_statusList->findItem( concept->getName(), 0 );
	if( !item ) {
		kdDebug()<<"GenerationStatusPage::Error finding class in list view"<<endl;
	} else if (generated) {
		item->setText( 1, i18n("Code Generated") );
	} else {
		item->setText( 1, i18n("Not Generated") );
	}
}

void CodeGenerationWizard::populateStatusList() {
	m_statusList->clear();
	for(QListViewItem* item = m_selectedList->firstChild(); item; item = item->nextSibling()) {
		new QListViewItem(m_statusList,item->text(0),i18n("Not Yet Generated"));
	}
}

void CodeGenerationWizard::showPage(QWidget *page) {
	if (indexOf(page) == 2)
	{//before advancint to the final page, check that the output directory exists and is
	 //writtable
		SettingsDlg::CodeGenState codegenState;
		((CodeGenerationOptionsPage*)QWizard::page(1))->state(codegenState);
		QFileInfo info(codegenState.outputDir);
		if(!info.exists())
		{
			if(KMessageBox::questionYesNo(this,
			i18n("The directory %1 does not exist. Do you want to create it now?").arg(info.filePath()),
			i18n("Output Directoy Does Not Exist")) == KMessageBox::Yes)
			{
				QDir dir;
				if(!dir.mkdir(info.filePath()))
				{
					KMessageBox::sorry(this,i18n("The directory could not be created.\nPlease make sure you have write access to its parent directory or select valid directory."),
					i18n("Error Creating Directory"));
					return;
				}
				//else, directory created
			}
			else // do not create output directory
			{
				KMessageBox::information(this,i18n("Please select a valid directory."),
				i18n("Output Directory Does Not Exist"));
				return;
			}
		}
		{//directory exists.. make sure we can write to it
			if(!info.isWritable())
			{
      	KMessageBox::sorry(this,i18n("The output directory exists, but it is not writable.\nPlease set\
				the appropiate permissions or choose another directory."),
				i18n("Error Writing to Output Directory"));
				return;
			}
     // it exits and we can write... make sure it is a directory
			if(!info.isDir())
			{
				KMessageBox::sorry(this,i18n("%1 does not seem to be a directory. Please choose a valid directory.").arg(info.filePath()),
				i18n("Please Choose Valid Directory"));
				return;
			}
		}
	}
		populateStatusList();
	QWizard::showPage(page);
}

CodeGenerator* CodeGenerationWizard::generator() {
	GeneratorInfo* info;
	if( m_CodeGenerationOptionsPage->getCodeGenerationLanguage().isEmpty() ) {
		KMessageBox::sorry(this,i18n("There is no Active Language defined.\nPlease select\
		                             one of the installed languages to generate the code in."),
				   i18n("No Language Selected"));
		return 0;
	}
	info = m_ldict.find( m_CodeGenerationOptionsPage->getCodeGenerationLanguage() );
	if(!info) {
		kdDebug()<<"error looking up library information (dictionary)"<<endl;
		return 0;
	}

	KLibLoader* loader = KLibLoader::self();
	if(!loader) {
		kdDebug()<<"error getting KLibLoader!"<<endl;
		return 0;
	}

	KLibFactory* fact = loader->factory(info->library.latin1());
	if(!fact) {
		kdDebug()<<"error getting the Factory"<<endl;
		return 0;
	}

	QObject* o=fact->create(0,0,info->object.latin1());
	if(!o) {
		kdDebug()<<"could not create object"<<endl;
		return 0;
	}

	CodeGenerator* g = (CodeGenerator*)o;
	g->setDocument(m_doc);
	return g;
}

#include "codegenerationwizard.moc"
