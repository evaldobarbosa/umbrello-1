/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

 /* Created By Krzywda Stanislas and Bouchikhi Mohamed-Amine ;) */

#ifndef CMD_CHANGEFILLCOLOR_H
#define CMD_CHANGEFILLCOLOR_H

#include <QtGui/QUndoCommand>

#include "umlwidget.h"

namespace Uml
{

    class CmdChangeFillColor : public QUndoCommand
    {
        UMLRectWidget *m_umlWidget;

        QColor m_oldColor;  ///< old color
        QColor m_color;     ///< new color

        public:
            CmdChangeFillColor(UMLRectWidget *w, const QColor& col);
            ~CmdChangeFillColor();
            void redo();
            void undo();
    };
}

#endif /*CMD_CHANGEFILLCOLOR_H*/
