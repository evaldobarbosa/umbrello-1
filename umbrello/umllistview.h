/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2013                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef UMLLISTVIEW_H
#define UMLLISTVIEW_H

#include "basictypes.h"
#include "umllistviewitem.h"
#include "umllistviewitemlist.h"
#include "icon_utils.h"
#include "umlobject.h"

#include <QDomDocument>
#include <QDomElement>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class QEvent;
class QMouseEvent;
class QFocusEvent;
class QKeyEvent;
class QDropEvent;
class IDChangeLog;
class ListPopupMenu;
class UMLClassifier;
class UMLDoc;
class UMLView;
class UMLClassifierListItem;
class UMLDragData;

/**
 * This is one of the main classes used in this program.
 * Information is displayed here in a tree view. No objects are created
 * here. A call to @ref UMLDoc make any additions/deletion or updates to
 * objects. This class will then wait for a signal before updating the tree view.
 *
 * @short  Displays the list view for the program.
 * @author Paul Hensgen    <phensgen@techie.com>
 * Bugs and comments to umbrello-devel@kde.org or http://bugs.kde.org
 */
class UMLListView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit UMLListView(QWidget *parent = 0);
    ~UMLListView();

    void setDocument(UMLDoc * doc);
    UMLDoc * document() const;

    void init();
    void clean();

    void setView(UMLView* view);

    void setTitle(int column, const QString &text);

    UMLListViewItemList selectedItems();
    UMLListViewItemList selectedItemsRoot();
    int selectedItemsCount();

    UMLListViewItem* createDiagramItem(UMLView *view);

    bool createItem(UMLListViewItem *item);

    UMLListViewItem* findFolderForDiagram(Uml::DiagramType::Enum dt);

    UMLListViewItem* determineParentItem(UMLObject* object) const;
    UMLListViewItem* determineParentItem(UMLListViewItem::ListViewType lvt) const;

    static bool mayHaveChildItems(UMLObject::ObjectType type);

    void addNewItem(UMLListViewItem * parent, UMLListViewItem::ListViewType type);

    UMLListViewItem * findUMLObject(const UMLObject *p) const;
    UMLListViewItem * findView(UMLView *v);
    UMLListViewItem * findItem(Uml::ID::Type id);

    UMLListViewItem *rootView(UMLListViewItem::ListViewType type);

    void changeIconOf(UMLObject *o, Icon_Utils::IconType to);

    bool isUnique(UMLListViewItem * item, const QString &name);

    void setSelected(UMLListViewItem * item, bool state) { setItemSelected((QTreeWidgetItem*)item, state);  }
    void setStartedCut(bool startedCut);
    void setStartedCopy(bool startedCopy);
    bool startedCopy() const;

    UMLListViewItem * moveObject(Uml::ID::Type srcId, UMLListViewItem::ListViewType srcType,
                                 UMLListViewItem *newParent);

    void closeDatatypesFolder();

    UMLListViewItem *theRootView() { return m_rv; }
    UMLListViewItem *theLogicalView() { return m_lv[Uml::ModelType::Logical]; }
    UMLListViewItem *theUseCaseView() { return m_lv[Uml::ModelType::UseCase]; }
    UMLListViewItem *theComponentView() { return m_lv[Uml::ModelType::Component]; }
    UMLListViewItem *theDeploymentView() { return m_lv[Uml::ModelType::Deployment]; }
    UMLListViewItem *theDatatypeFolder() { return m_datatypeFolder; }

    UMLListViewItem::ListViewType rootViewType(UMLListViewItem *item);

    void saveToXMI(QDomDocument & qDoc, QDomElement & qElement);
    bool loadFromXMI(QDomElement & element);
    bool loadChildrenFromXMI(UMLListViewItem * parent, QDomElement & element);

    friend QDebug operator<<(QDebug out, const UMLListView& view);

protected:
    bool event(QEvent *e);
    void mouseReleaseEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseDoubleClickEvent(QMouseEvent *me);
    void focusOutEvent (QFocusEvent *fe);
    void contextMenuEvent(QContextMenuEvent *event);
    UMLDragData* getDragData();

    bool acceptDrag(QDropEvent* event) const;
    void keyPressEvent(QKeyEvent* ke);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
    void commitData(QWidget *editor);

    UMLListViewItem * findUMLObjectInFolder(UMLListViewItem *folder, UMLObject *obj);

    static bool isExpandable(UMLListViewItem::ListViewType lvt);

    void deleteChildrenOf(UMLListViewItem *parent);

    bool deleteItem(UMLListViewItem *temp);

    void childObjectAdded(UMLClassifierListItem* child, UMLClassifier* parent);

    void addAtContainer(UMLListViewItem *item, UMLListViewItem *parent);

public slots:
    void slotDiagramCreated(Uml::ID::Type id);
    void slotDiagramRenamed(Uml::ID::Type id);
    void slotDiagramRemoved(Uml::ID::Type id);
    void slotObjectCreated(UMLObject* object);
    void slotObjectRemoved(UMLObject* object);

    void connectNewObjectsSlots(UMLObject* object);

    void childObjectAdded(UMLClassifierListItem* obj);
    void childObjectRemoved(UMLClassifierListItem* obj);

    void slotObjectChanged();

    void slotMenuSelection(QAction* action);

    void slotDropped(QDropEvent* de, UMLListViewItem* target);

    void expandAll(UMLListViewItem *item);
    void collapseAll(UMLListViewItem *item);

    void slotCutSuccessful();
    void slotDeleteSelectedItems();

protected slots:
    void slotItemSelectionChanged();
    void slotExpanded(QTreeWidgetItem* item);
    void slotCollapsed(QTreeWidgetItem* item);

private:
    UMLListViewItem* recursiveSearchForView(UMLListViewItem* folder,
                                            UMLListViewItem::ListViewType type, Uml::ID::Type id);

    void setBackgroundColor(const QColor & color);

    UMLListViewItem* m_rv;         ///< root view (home)
    UMLListViewItem* m_lv[Uml::ModelType::N_MODELTYPES];  ///< predefined list view roots
    UMLListViewItem* m_datatypeFolder;
    UMLDoc*          m_doc;
    bool             m_bStartedCut;
    bool             m_bStartedCopy;
    bool m_bCreatingChildObject;  ///< when creating an attribute or an operation to stop it adding a second listViewItem
    QPoint m_dragStartPosition;
    QMimeData* m_dragCopyData;
};

#endif
