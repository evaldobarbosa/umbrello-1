/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef ACTORWIDGET_H
#define ACTORWIDGET_H

#include "umlrectwidget.h"

class UMLActor;

/**
 * This class is the graphical version of a UML Actor and can be
 * placed in UseCase diagram.
 *
 * The ActorWidget class inherits from the @ref UMLRectWidget class
 * which adds most of the functionality to this class.
 *
 * @short A graphical version of a UML Actor.
 * @author Paul Hensgen <phensgen@techie.com>
 * @author Gopala Krishna
 * @see UMLRectWidget
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class ActorWidget : public UMLRectWidget
{
public:

    ActorWidget(UMLActor *o);
	virtual ~ActorWidget();

	virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *w);

	//Note: For loading from XMI, the inherited parent method is used.
	virtual void saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

protected:
    virtual void updateGeometry();
	virtual QVariant attributeChange(WidgetAttributeChange change, const QVariant& oldValue);
	virtual void updateTextItemGroups();

private:
	static const QSizeF MinimumSize;
	/// A path representing the actor drawing.
	QPainterPath m_actorPath;
};

#endif
