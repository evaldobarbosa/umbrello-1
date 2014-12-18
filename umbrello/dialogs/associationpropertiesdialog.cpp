/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "associationpropertiesdialog.h"

// local includes
#include "associationwidget.h"
#include "associationgeneralpage.h"
#include "associationrolepage.h"
#include "classgeneralpage.h"
#include "classpropertiesdialog.h"
#include "debug_utils.h"
#include "icon_utils.h"
#include "objectwidget.h"
#include "uml.h"
#include "umldoc.h"
#include "umlobject.h"
#include "umlview.h"
#include "umlwidgetstylepage.h"

// kde includes
#include <kfontdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kvbox.h>

// qt includes
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QHBoxLayout>

/**
 *  Sets up an Association Properties Dialog.
 *  @param  parent  The parent of the AssociationPropertiesDialog
 *  @param  assocWidget  The Association Widget to display properties of.
 *  @param  pageNum The page to show first.
 */
AssociationPropertiesDialog::AssociationPropertiesDialog (QWidget *parent, AssociationWidget * assocWidget, int pageNum)
  : DialogBase(parent),
    m_pGenPage(0),
    m_pAssoc(assocWidget)
{
    Q_UNUSED(pageNum)
    setCaption(i18n("Association Properties"));
    setupPages();

    connect(this, SIGNAL(okClicked()), this, SLOT(slotOk()));
    connect(this, SIGNAL(applyClicked()), this, SLOT(slotApply()));
}

/**
 *  Standard destructor.
 */
AssociationPropertiesDialog::~AssociationPropertiesDialog()
{
}

void AssociationPropertiesDialog::slotOk()
{
    slotApply();
    accept();
}

void AssociationPropertiesDialog::slotApply()
{
    DialogBase::apply();

    if (m_pGenPage) {
        m_pGenPage->apply();
    }

    if (m_pAssoc) {
        applyFontPage(m_pAssoc);
    }
}

void AssociationPropertiesDialog::setupPages()
{
    // general page
    QFrame *page = createPage(i18nc("general settings", "General"), i18n("General Settings"), Icon_Utils::it_Properties_General);
    QHBoxLayout *layout = new QHBoxLayout(page);
    m_pGenPage = new AssociationGeneralPage (page, m_pAssoc);
    layout->addWidget(m_pGenPage);

    setupAssociationRolePage(m_pAssoc);
    setupStylePage(m_pAssoc);
    setupFontPage(m_pAssoc);
}

#include "associationpropertiesdialog.moc"
