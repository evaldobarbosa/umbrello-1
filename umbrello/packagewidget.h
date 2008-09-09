/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef PACKAGEWIDGET_H
#define PACKAGEWIDGET_H

#include "umlrectwidget.h"

class UMLPackage;

/**
 * Defines a graphical version of the Package.  Most of the
 * functionality will come from the @ref UMLPackage class.
 *
 * @short A graphical version of a Package.
 * @author Jonathan Riddell
 * @author Gopala Krishna
 *
 * @see UMLRectWidget
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class PackageWidget : public UMLRectWidget
{
public:
    PackageWidget(UMLPackage * o);
    virtual ~PackageWidget();

	// Uses UMLRectWidget::loadFromXMI to load from XMI
    virtual void saveToXMI(QDomDocument& qDoc, QDomElement& qElement);

	void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w);

protected:
    void updateGeometry();
	void updateTextItemGroups();
	QVariant attributeChange(WidgetAttributeChange change, const QVariant& oldValue);

private:
	enum {
		GroupIndex
	};
    enum {
        StereoTypeItemIndex = 0,
        NameItemIndex,
		TextItemCount
    };

	QRectF m_topRect;
    QRectF m_packageTextRect;
};

#endif
