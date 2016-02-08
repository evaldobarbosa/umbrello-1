/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2015      Tzvetelin Katchov <katchov@gmail.com>         *
 *   copyright (C) 2002-2015                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "umlenumliteraldialog.h"

// app includes
#include "enumliteral.h"
#include "classifier.h"
#include "dialog_utils.h"

// kde includes
#include <klineedit.h>
#include <KLocalizedString>
#include <KMessageBox>

// qt includes
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>

UMLEnumLiteralDialog::UMLEnumLiteralDialog(QWidget * pParent, UMLEnumLiteral * pEnumLiteral)
  : SinglePageDialogBase(pParent)
{
    setCaption(i18n("EnumLiteral Properties"));
    m_pEnumLiteral = pEnumLiteral;
    setupDialog();
}

UMLEnumLiteralDialog::~UMLEnumLiteralDialog()
{
}

/**
 *   Sets up the dialog
 */
void UMLEnumLiteralDialog::setupDialog()
{
    int margin = fontMetrics().height();

    QFrame * frame = new QFrame(this);
    setMainWidget(frame);
    QVBoxLayout * mainLayout = new QVBoxLayout(frame);

    m_pValuesGB = new QGroupBox(i18n("General Properties"), frame);
    QGridLayout * valuesLayout = new QGridLayout(m_pValuesGB);
    valuesLayout->setMargin(margin);
    valuesLayout->setSpacing(10);

    Dialog_Utils::makeLabeledEditField(valuesLayout, 0,
                                    m_pNameL, i18nc("literal name", "&Name:"),
                                    m_pNameLE, m_pEnumLiteral->name());

    Dialog_Utils::makeLabeledEditField(valuesLayout, 1,
                                    m_pValueL, i18n("&Value:"),
                                    m_pValueLE, m_pEnumLiteral->value());

    mainLayout->addWidget(m_pValuesGB);

    m_pNameLE->setFocus();
    connect(m_pNameLE, SIGNAL(textChanged(QString)), SLOT(slotNameChanged(QString)));
    slotNameChanged(m_pNameLE->text());
}

void UMLEnumLiteralDialog::slotNameChanged(const QString &_text)
{
    enableButtonOk(!_text.isEmpty());
}

/**
 * Checks if changes are valid and applies them if they are,
 * else returns false
 */
bool UMLEnumLiteralDialog::apply()
{
    QString name = m_pNameLE->text();
    if (name.isEmpty()) {
        KMessageBox::error(this, i18n("You have entered an invalid attribute name."),
                           i18n("Attribute Name Invalid"), 0);
        m_pNameLE->setText(m_pEnumLiteral->name());
        return false;
    }
    UMLClassifier * pConcept = dynamic_cast<UMLClassifier *>(m_pEnumLiteral->parent());
    UMLObject *o = pConcept->findChildObject(name);
    if (o && o != m_pEnumLiteral) {
        KMessageBox::error(this, i18n("The attribute name you have chosen is already being used in this operation."),
                           i18n("Attribute Name Not Unique"), 0);
        m_pNameLE->setText(m_pEnumLiteral->name());
        return false;
    }
    m_pEnumLiteral->setName(name);

    m_pEnumLiteral->setValue(m_pValueLE->text());

    return true;
}