/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "pinwidget.h"

// app includes
#include "floatingtextwidget.h"
#include "listpopupmenu.h"
#include "uml.h"
#include "umldoc.h"
#include "umlscene.h"
#include "umlview.h"
#include "uniqueid.h"

// qt includes
#include <QtCore/QTimer>
#include <QtGui/QGraphicsSceneMouseEvent>

// kde includes
#include <klocale.h>
#include <kinputdialog.h>

const qreal PinWidget::Size = 10;

/**
 * Creates a Pin widget.
 *
 * @param  owner  The widget to which this pin is attached.
 * @param     id  The ID to assign (-1 will prompt a new ID.)
 */
PinWidget::PinWidget(UMLRectWidget* owner, Uml::IDType id ):
    UMLRectWidget(0, id)
{
    m_baseType = Uml::wt_Pin;

    setIgnoreSnapToGrid(true);
    setIgnoreSnapComponentSizeToGrid(true);
    setResizable(false);

    // Intialize the members now
    m_ownerWidget = owner;
    // Create floating text widget on demand.
    m_nameFloatingTextWiget = 0;

    // No need for margin
    setMargin(0);

    // Make the owner the parent of this widget.
    setParentItem(owner);

    // Ensure the pin widget is above the owner
    qreal z = owner ? owner->zValue() + 5 : 20;
    setZValue(z);

    // HACK: @see setInitialPosition
    QTimer::singleShot(2, this, SLOT(setInitialPosition()));
}

/// Destructor
PinWidget::~PinWidget()
{
}

/**
 * Reimplemented from UMLRectWidget::paint to draw the pin widget
 * box.
 */
void PinWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(lineColor(), lineWidth()));
    painter->setBrush(brush());

    painter->drawRect(rect());
}

/**
 * Reimplemented from UMLRectWidget::loadFromXMI to load PinWidget
 * from XMI element.
 */
bool PinWidget::loadFromXMI( QDomElement & qElement )
{
    if (!UMLRectWidget::loadFromXMI(qElement)) {
        return false;
    }
    QString widgetaid = qElement.attribute( "widgetaid", "-1" );

    Uml::IDType aId = STR2ID(widgetaid);

    UMLRectWidget *pWA = umlScene()->findWidget( aId );
    if (pWA == 0) {
        uDebug() << "role A object " << ID2STR(aId) << " not found";
        return false;
    }

    m_ownerWidget = pWA;
    setParentItem(m_ownerWidget);

    QString textid = qElement.attribute( "textid", "-1" );
    Uml::IDType textId = STR2ID(textid);
    if (textId != Uml::id_None) {
        UMLRectWidget *flotext = umlScene()->findWidget( textId );
        if (flotext != 0) {
            // This only happens when loading files produced by
            // umbrello-1.3-beta2.
            m_nameFloatingTextWiget = static_cast<FloatingTextWidget*>(flotext);
            return true;
        }
    }
    else {
        // no textid stored->get unique new one
        textId = UniqueID::gen();
    }

    //now load child elements
    QDomNode node = qElement.firstChild();
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
        QString tag = element.tagName();
        if (tag == "floatingtext") {
            m_nameFloatingTextWiget = new FloatingTextWidget( Uml::tr_Floating, textId );
            m_nameFloatingTextWiget->setText(m_Text);
            // Set it as parent so that it moves with moving of PinWidget.
            m_nameFloatingTextWiget->setParentItem(this);

            if( ! m_nameFloatingTextWiget->loadFromXMI(element) ) {
                // Most likely cause: The FloatingTextWidget is empty.
                delete m_nameFloatingTextWiget;
                m_nameFloatingTextWiget = 0;
            }
        } else {
            uError() << "unknown tag " << tag << endl;
        }
    }

    return true;
}

/**
 * Reimplemented from UMLRectWidget::saveToXMI to save PinWidget
 * data into 'pinwidget' XMI element.
 */
void PinWidget::saveToXMI( QDomDocument & qDoc, QDomElement & qElement )
{
    QDomElement PinElement = qDoc.createElement( "pinwidget" );
    PinElement.setAttribute( "widgetaid", ID2STR(m_ownerWidget->id()));
    UMLRectWidget::saveToXMI( qDoc, PinElement );
    if (m_nameFloatingTextWiget && !m_nameFloatingTextWiget->text().isEmpty()) {
        PinElement.setAttribute( "textid", ID2STR(m_nameFloatingTextWiget->id()) );
        m_nameFloatingTextWiget->saveToXMI( qDoc, PinElement );
    }
    qElement.appendChild( PinElement );
}

/**
 * This method updates the position of this widget with repect to the
 * reference point \a reference.
 */
void PinWidget::updatePosition(const QPointF& reference)
{
    if (!m_ownerWidget) {
        uError() << "Owner is null!";
        return;
    }

    QList<QPointF> points;
    QRectF r = m_ownerWidget->rect();
    const QPointF c = r.center();

    points << QPointF(r.left(), c.y()) << QPointF(c.x(), r.top())
           << QPointF(r.right(), c.y()) << QPointF(c.x(), r.bottom());

    // Iterate and find the nearest edge to where this widget should lie.
    int minIndex = -1;
    qreal distance = 99999;// Start with some maximum
    for(int i = 0; i < points.size(); ++i) {
        const QPointF &p = points.at(i);
        qreal pointDistance = QLineF(p, reference).length();
        if (pointDistance < distance) {
            distance = pointDistance;
            minIndex = i;
        }
    }

    const qreal w = size().width();
    const qreal h = size().height();
    setPos(points[minIndex] - QPointF(.5 * w, .5 * h));
}

/**
 * Reimplemented from UMLRectWidget::updateGeometry to set minimum
 * and maximum size for this widget.
 */
void PinWidget::updateGeometry()
{
    const QSizeF sz(PinWidget::Size, PinWidget::Size);
    setMinimumSize(sz);
    setMaximumSize(sz);

    UMLRectWidget::updateGeometry();
}

/**
 * Reimplemented from UMLRectWidget::attributeChange to handle @ref
 * NameHasChanged to create/delete and update FloatingTextWidget's
 * text.
 */
QVariant PinWidget::attributeChange(WidgetAttributeChange change, const QVariant& oldValue)
{
    if (change == NameHasChanged) {
        if (name().isEmpty()) {
            delete m_nameFloatingTextWiget;
            m_nameFloatingTextWiget = 0;
        }
        else {
            if (!m_nameFloatingTextWiget) {
                m_nameFloatingTextWiget = new FloatingTextWidget(Uml::tr_Floating);
                m_nameFloatingTextWiget->setParentItem(this);
            }
            m_nameFloatingTextWiget->setText(name());
        }
    }

    return UMLRectWidget::attributeChange(change, oldValue);
}

/**
 * Reimplemented from UMLRectWidget::mouseMoveEvent to move this
 * widget only along the edge of the ActivityWidget to which this
 * widget is pinned to.
 */
void PinWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_ownerWidget) {
        QPointF eventPos = m_ownerWidget->mapFromScene(event->scenePos());
        updatePosition(eventPos);
    }
}

/**
 * Reimplemented from UMLRectWidget::slotMenuSelection to handle
 * rename action to set the text of this widget.
 */
void PinWidget::slotMenuSelection(QAction* action)
{
    bool ok = false;
    QString text;

    // Menu is passed in as action's parent.
    ListPopupMenu *menu = qobject_cast<ListPopupMenu*>(action->parent());
    ListPopupMenu::Menu_Type sel = menu->getMenuType(action);

    switch( sel ) {
    case ListPopupMenu::mt_Rename:
        text = KInputDialog::getText( i18n("Enter Pin Name"), i18n("Enter the pin name :"), name(), &ok );
        if(ok) {
            setName(text);
        }
        break;

    default:
        UMLRectWidget::slotMenuSelection(action);
    }
}

/**
 * Hack! This is the only way i could get the pin widget attached to
 * ActivityWidget on creation
 */
void PinWidget::setInitialPosition()
{
    if (m_ownerWidget) {
        updatePosition(pos());
    }
}

#include "pinwidget.moc"
