/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H

#include <QString>

// forward declarations
class UMLObject;
class UMLScene;
class UMLWidget;

/**
 * Widget factory methods.
 * Bugs and comments to umbrello-devel@kde.org or http://bugs.kde.org
 */
namespace Widget_Factory {

    UMLWidget *createWidget(UMLScene *scene, UMLObject *docObj);

    UMLWidget* makeWidgetFromXMI(const QString& tag,
                                 const QString& idStr, UMLScene *scene);

}   // end namespace Widget_Factory

#endif
