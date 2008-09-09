/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-200(                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "cmd_create_widget.h"

#include "uml.h"
#include "umldoc.h"

#include <klocale.h>

namespace Uml
{

    CmdCreateWidget::CmdCreateWidget(UMLView* view, UMLRectWidget* w):m_view(view), m_widget(w)
    {
        setText(i18n("Create widget :") + w->name());
    }

    CmdCreateWidget::~CmdCreateWidget()
    {
        //m_view->removeWidget(m_widget);
    }

    // Create the UMLObject
    void CmdCreateWidget::redo()
    {
        m_widget->setVisible(true);
    }

    // Suppress the UMLObject
    void CmdCreateWidget::undo()
    {
        m_widget->setVisible(false);
    }

}
