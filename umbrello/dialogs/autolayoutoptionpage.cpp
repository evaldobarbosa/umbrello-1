/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2012                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "autolayoutoptionpage.h"

// local includes
#include "layoutgenerator.h"

// kde includes
#include <KFileDialog>
#include <KLineEdit>
#include <KLocale>

// qt includes
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

/**
 * Constructor.general
 * @param parent   the parent (wizard) of this wizard page
 */
AutoLayoutOptionPage::AutoLayoutOptionPage(QWidget* parent)
  : QWidget(parent)
{
    setupUi(this);
    m_autoDotPath->setChecked( Settings::optionState().autoLayoutState.autoDotPath );
    m_dotPath->setText( Settings::optionState().autoLayoutState.dotPath );
    connect( m_autoDotPath, SIGNAL(toggled(bool)), this, SLOT(slotAutoDotPathCBClicked(bool)) );

    if (Settings::optionState().autoLayoutState.autoDotPath) {
        m_dotPath->setEnabled (false );
        m_dotPath->setText( LayoutGenerator::currentDotPath() );
    }
}

/**
 * destructor
 */
AutoLayoutOptionPage::~AutoLayoutOptionPage()
{
}

/**
 * sets default values
 */
void AutoLayoutOptionPage::setDefaults()
{
    m_autoDotPath->setChecked(true);
}

/**
 * Reads the set values from their corresponding widgets, writes them back to
 * the data structure, and notifies clients.
 */
void AutoLayoutOptionPage::apply()
{
    // FIXME use autoLayoutState
    Settings::optionState().autoLayoutState.autoDotPath = m_autoDotPath->isChecked();
    Settings::optionState().autoLayoutState.dotPath = m_autoDotPath->isChecked() ? "" : m_dotPath->text();
    emit applyClicked();
}

void AutoLayoutOptionPage::slotAutoDotPathCBClicked(bool value)
{
    if (value)
        m_dotPath->setText(LayoutGenerator::currentDotPath());
}