/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "entitywidget.h"

// qt/kde includes
#include <qpainter.h>
#include <kdebug.h>

// app includes
#include "entity.h"
#include "entityattribute.h"
#include "uniqueconstraint.h"
#include "foreignkeyconstraint.h"
#include "classifier.h"
#include "umlclassifierlistitemlist.h"
#include "classifierlistitem.h"
#include "umlview.h"
#include "umldoc.h"
#include "uml.h"
#include "listpopupmenu.h"
#include "object_factory.h"


EntityWidget::EntityWidget(UMLScene* scene, UMLObject* o): UMLWidget(scene, o) {
    init();
}

void EntityWidget::init() {
    UMLWidget::setBaseType(Uml::wt_Entity);
    setSize(100, 30);

    //set defaults from m_pView
    if (m_pScene) {
        //check to see if correct
        //const Settings::OptionState& ops = m_pView->getOptionState();
    }
    if (! UMLApp::app()->getDocument()->loading())
        updateComponentSize();
}

EntityWidget::~EntityWidget() {}

void EntityWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *)
{
	QPainter &p = *painter;
	qreal offsetX = 0, offsetY = 0;

    setPenFromSettings(p);
    if(UMLWidget::getUseFillColour())
        p.setBrush(UMLWidget::getFillColour());
    else {
        // [PORT] StyleOption
        // p.setBrush( m_pView->viewport()->palette().color(QPalette::Background) );
    }

    const qreal w = getWidth();
    const qreal h = getHeight();

    const QFontMetrics &fm = getFontMetrics(FT_NORMAL);
    qreal fontHeight  = fm.lineSpacing();
    const QString name = this->getName();

    p.drawRect(offsetX, offsetY, w, h);
    p.setPen(QPen(Qt::black));

    QFont font = UMLWidget::getFont();
    font.setBold(true);
    p.setFont(font);
    qreal y = 0;
    if ( !m_pObject->getStereotype().isEmpty() ) {
        p.drawText(offsetX + ENTITY_MARGIN, offsetY,
                   w - ENTITY_MARGIN * 2,fontHeight,
                   Qt::AlignCenter, m_pObject->getStereotype(true));
        font.setItalic( m_pObject->getAbstract() );
        p.setFont(font);
        p.drawText(offsetX + ENTITY_MARGIN, offsetY + fontHeight,
                   w - ENTITY_MARGIN * 2, fontHeight, Qt::AlignCenter, name);
        font.setBold(false);
        font.setItalic(false);
        p.setFont(font);
        y = fontHeight * 2;
    } else {
        font.setItalic( m_pObject->getAbstract() );
        p.setFont(font);
        p.drawText(offsetX + ENTITY_MARGIN, offsetY,
                   w - ENTITY_MARGIN * 2, fontHeight, Qt::AlignCenter, name);
        font.setBold(false);
        font.setItalic(false);
        p.setFont(font);

        y = fontHeight;
    }

    setPenFromSettings(p);

    p.drawLine(offsetX, offsetY + y, offsetX + w - 1, offsetY + y);

    QFontMetrics fontMetrics(font);
    UMLClassifier *classifier = (UMLClassifier*)m_pObject;
    UMLClassifierListItem* entityattribute = 0;
    UMLClassifierListItemList list = classifier->getFilteredList(Uml::ot_EntityAttribute);
    foreach (entityattribute , list ) {
        QString text = entityattribute->getName();
        p.setPen( QPen(Qt::black) );
        UMLEntityAttribute* casted = dynamic_cast<UMLEntityAttribute*>( entityattribute );
        if( casted && casted->getIndexType() == Uml::Primary )
        {
            font.setUnderline( true );
            p.setFont( font );
            font.setUnderline( false );
        }
        p.drawText(offsetX + ENTITY_MARGIN, offsetY + y,
                   fontMetrics.width(text), fontHeight, Qt::AlignVCenter, text);
        p.setFont( font );
        y+=fontHeight;
    }

    if (isSelected()) {
        drawSelected(&p, offsetX, offsetY);
    }
}

QSizeF EntityWidget::calculateSize() {
    if (!m_pObject) {
        return UMLWidget::calculateSize();
    }

    qreal width, height;
    QFont font = UMLWidget::getFont();
    font.setItalic(false);
    font.setUnderline(false);
    font.setBold(false);
    const QFontMetrics fm(font);

    const qreal fontHeight = fm.lineSpacing();

    int lines = 1;//always have one line - for name
    if ( !m_pObject->getStereotype().isEmpty() ) {
        lines++;
    }

    const int numberOfEntityAttributes = ((UMLEntity*)m_pObject)->entityAttributes();

    height = width = 0;
    //set the height of the entity

    lines += numberOfEntityAttributes;
    if (numberOfEntityAttributes == 0) {
        height += fontHeight / 2; //no entity literals, so just add a bit of space
    }

    height += lines * fontHeight;

    //now set the width of the concept
    //set width to name to start with
    // FIXME spaces to get round beastie with font width,
    // investigate UMLWidget::getFontMetrics()
    width = getFontMetrics(FT_BOLD_ITALIC).boundingRect(' ' + getName() + ' ').width();

    const qreal w = getFontMetrics(FT_BOLD).boundingRect(m_pObject->getStereotype(true)).width();

    width = w > width?w:width;

    UMLClassifier* classifier = (UMLClassifier*)m_pObject;
    UMLClassifierListItemList list = classifier->getFilteredList(Uml::ot_EntityAttribute);
    UMLClassifierListItem* listItem = 0;
    foreach (listItem , list ) {
        qreal w = fm.width( listItem->getName() );
        width = w > width?w:width;
    }

    //allow for width margin
    width += ENTITY_MARGIN * 2;

    return QSizeF(width, height);
}

void EntityWidget::slotMenuSelection(QAction* action) {
    ListPopupMenu::Menu_Type sel = m_pMenu->getMenuType(action);
    switch(sel) {
    case ListPopupMenu::mt_EntityAttribute:
        if (Object_Factory::createChildObject(static_cast<UMLClassifier*>(m_pObject),
                                              Uml::ot_EntityAttribute) )  {
            UMLApp::app()->getDocument()->setModified();
        }
        break;

    case ListPopupMenu::mt_PrimaryKeyConstraint:
    case ListPopupMenu::mt_UniqueConstraint:
        if ( UMLObject* obj = Object_Factory::createChildObject(static_cast<UMLEntity*>(m_pObject),
                                               Uml::ot_UniqueConstraint) ) {
            UMLApp::app()->getDocument()->setModified();

            if ( sel == ListPopupMenu::mt_PrimaryKeyConstraint ) {
                UMLUniqueConstraint* uc = static_cast<UMLUniqueConstraint*>(obj);
                static_cast<UMLEntity*>(m_pObject)->setAsPrimaryKey(uc);
            }
        }
        break;

    case ListPopupMenu::mt_ForeignKeyConstraint:
         if (Object_Factory::createChildObject(static_cast<UMLEntity*>(m_pObject),
                                               Uml::ot_ForeignKeyConstraint) ) {
             UMLApp::app()->getDocument()->setModified();

        }
        break;

    case ListPopupMenu::mt_CheckConstraint:
         if (Object_Factory::createChildObject(static_cast<UMLEntity*>(m_pObject),
                                               Uml::ot_CheckConstraint) ) {
             UMLApp::app()->getDocument()->setModified();

        }
        break;

    default:
        UMLWidget::slotMenuSelection(action);
    }

}

void EntityWidget::saveToXMI( QDomDocument& qDoc, QDomElement& qElement ) {
    QDomElement conceptElement = qDoc.createElement("entitywidget");
    UMLWidget::saveToXMI(qDoc, conceptElement);
    qElement.appendChild(conceptElement);
}

bool EntityWidget::loadFromXMI( QDomElement & qElement ) {
    if ( !UMLWidget::loadFromXMI(qElement) ) {
        return false;
    }
    return true;
}

