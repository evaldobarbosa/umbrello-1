/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ASSOCIATIONWIDGET_H
#define ASSOCIATIONWIDGET_H

#include "umlnamespace.h"
#include "linepath.h"

// qt includes
#include <qobject.h>

class FloatingText;
class IDChangeLog;
class ListPopupMenu;
class QBitmap;
class QPixmap;
class QDataStream;
class UMLView;
class UMLWidget;
class UMLAssociation;

using namespace Uml;

/**
 * This class represents an association inside a diagram.
 * Associations exist not only between UML objects. For example, when a Note is
 * attached to a UML object, the Note itself is not a UML object.
 * This class supports both kinds of associations. An association where one or
 * both roles are not a UML object is called a "pure widget association".
 *
 * @author Gustavo Madrigal
 * @short This class represents an association inside a diagram.
 * @version $Revision$
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class AssociationWidget : public QObject {
	Q_OBJECT
public:
	/**
	*   enumeration used for stating where a line is on a widget.
	*/
	enum Region{ Error = 0, West , North, East, South, NorthWest, NorthEast, SouthEast, SouthWest, Center };

	/**
	* Constructor
	*/
	AssociationWidget(QWidget *parent);

	/**
	* Constructor
	*/
	AssociationWidget(QWidget *parent, UMLWidget* WidgetA, Association_Type Type, UMLWidget* WidgetB);

	/**
	* Deconstructor
	*/
	virtual ~AssociationWidget();

	/**
	* Overrides = operator
	*/
	AssociationWidget& operator=(AssociationWidget & Other);

	/**
	*   Overrides == operator
	*/
	bool operator==(AssociationWidget & Other);

	/**
	* Overrides != operator
	*/
	bool operator!=(AssociationWidget & Other);

	/**
	*   activates an associationwidget after a load
	*/
	bool activate();

	/**
	 * Write property of m_pWidgetA
	 */
	void setWidgetA( UMLWidget* WidgetA);

	/**
	 * Write property of m_pWidgetB
	 */
	void setWidgetB( UMLWidget* WidgetB);

	/**
	* Read property of FloatingText* m_pMultiA.
	*/
	FloatingText* getMultiAWidget();

	/**
	 * Returns the m_pMultiA's text.
	 */
	QString getMultiA() const;

	/**
	* Read property of FloatingText* m_pMultiB.
	*/
	FloatingText* getMultiBWidget();

	/**
	* Returns the m_pMultiB's text.
	*/
	QString getMultiB() const;

	/**
	 * Read property of CFloatingText* m_pRoleA.
	 */
	FloatingText* getNameWidget();

	/**
	 * Returns the m_pName's text.
	 */
	QString getName() const;

	/**
	* Read property of CFloatingText* m_pRoleA.
	*/
	FloatingText* getRoleAWidget();

	/**
	* Read property of CFloatingText* m_pRoleB.
	*/
	FloatingText* getRoleBWidget();

	/**
	 * Returns the documentation about this association.
	 */
	QString getDoc() const;

	/**
	* Returns the m_pRoleA's text.
	*/
	QString getRoleNameA() const;

	/**
	 * Returns the documentation about RoleA.
	 */
	QString getRoleADoc() const;

	/**
	* Returns the m_pRoleB's text.
	*/
	QString getRoleNameB() const;

	/**
	 * Returns the documentation about Role B.
	 */
	QString getRoleBDoc() const;

	/**
	 * Sets the text to the FloatingText representing the Name of this association
	 */
	void setName (QString strRole);

	/**
	* Sets the text to the FloatingText representing the Multiplicity at the ending side
	* of the association
	*/
	bool setMultiB(QString strMultiB);


	/**
	* Sets the text to the FloatingText representing the Multiplicity at the starting side
	* of the association
	*/
	bool setMultiA(QString strMultiA);

	/**
	 * Gets the visibility on the rolename A end of the Association
	 */
	Scope getVisibilityA () const;

	/*
	 * Gets the visibility on the rolename B end of the Association
	 */
	Scope getVisibilityB () const;

	/**
	 * Sets the visibility on the rolename 'A' end of the Association
	 */
	void setVisibilityA ( Scope visibility );

	/*
	 * Sets the visibility on the rolename 'B' end of the Association
	 */
	void setVisibilityB (Scope visibility);

	/**
	 * Gets the changeability on the rolename A end of the Association
	 */
	Changeability_Type getChangeabilityA() const;

	/**
	 * Gets the changeability on the rolename B end of the Association
	 */
	Changeability_Type getChangeabilityB() const;

	/**
	 * Sets the changeability on the rolename 'B' end of the Association
	 */
	void setChangeabilityA (Changeability_Type value);

	/**
	 * Sets the changeability on the rolename 'B' end of the Association
	 */
	void setChangeabilityB (Changeability_Type value);

	/**
	* Write property of int m_nWidgetAID.
	*/
	void setWidgetAID(int AID);

	/**
	* Read property of int m_nWidgetAID.
	*/
	int getWidgetAID() const;

	/**
	* Write property of int m_nWidgetBID.
	*/
	void setWidgetBID(int BID);

	/**
	* Read property of int m_nWidgetBID.
	*/
	int getWidgetBID() const;

	/**
	 * Read property of UMLWidget* m_pWidgetA.
	 */
	UMLWidget* getWidgetA();

	/**
	 * Set the associated widgets.
	 */
	bool setWidgets( UMLWidget* WidgetA, Association_Type AssocType, UMLWidget* WidgetB);

	/**
	 * Read property of UMLWidget* m_pWidgetB.
	 */
	UMLWidget* getWidgetB();

	/**
	 * Synchronizes the Widget's data members with its display properties, for example:
	 * the X and Y positions of the widget, etc
	 */
	void synchronizeData();

	/**
	 * Returns true if this association associates WidgetA to WidgetB, otherwise it returns
	 *false
	 */
	bool checkAssoc(UMLWidget * WidgetA, UMLWidget *WidgetB);

	/**
	 * CleansUp all the association's data in the related widgets
	 */
	void cleanup();

	/**
	 * Returns true if the Widget is either at the starting or ending side of the association
	 */
	bool contains(UMLWidget* Widget);

	/**
	 * Gets the association's type
	 */
	Association_Type getAssocType() const;

	/**
	 * Sets the association's type
	 */
	void setAssocType(Association_Type Type);

	/**
	 * Returns a QString Object representing this AssociationWidget
	 */
	QString toString();

	/**
	 * Read property of bool m_bActivated.
	 */
	const bool isActivated();

	/**
	 * Set the m_bActivated flag of a widget but does not perform the Activate method
	 */
	void setActivated(bool Active /*=true*/);

	/**
	 *	Sets the state of whether the widget is selected.
	 *
	 *	@param _select The state of whether the widget is selected.
	 */
	void setSelected(bool _select);

	/**
	 *	Returns the state of whether the widget is selected.
	 *
	 *	@return Returns the state of whether the widget is selected.
	 */
	bool getSelected() const {
		return m_bSelected;
	}

	/**
	 * Adjusts the endding point of the association that connects to Widget
	 */
	void widgetMoved(UMLWidget* Widget, int x, int y);

	/**
	 * calculates the m_unNameLineSegment value according to the new NameText topleft
	 *  corner PT.
	 * It iterates through all LinePath's segments for each one it calculates the sum of
	 * PT's distance to the start point + PT's distance to the end point. The segment with
	 * the smallest sum will be the RoleTextSegment (if this segment moves then the
	 * RoleText will move with it). It sets m_unNameLineSegment to the start point of the
	 * chosen segment
	 */
	void calculateNameTextSegment();

	/**
	 * Adds a break point (if left mouse button)
	 */
	void mouseDoubleClickEvent(QMouseEvent * me);

	/**
	 *  Sets the association to be selected
	 */
	void mousePressEvent(QMouseEvent * me);

	/**
	 *  Displays the right mouse buttom menu if right button is pressed
	 */
	void mouseReleaseEvent(QMouseEvent * me);

	/**
	 *  Moves the break point being dragged
	 */
	void mouseMoveEvent(QMouseEvent * me);


	/**
	 *   Returns true if the given point is ob the Association.
	 */
	bool onAssociation(const QPoint & point);

	/**
	* 	Moves all the mid points (all expcept start /end ) by the given amount
	*/
	void moveMidPointsBy( int x, int y );

	/**
	* 	Moves the entire association by the given offset.
	*/
	void moveEntireAssoc( int x, int y );

	/**
	 *     Sets the new font for the text widgets used.  Called by the text widgets
	 *    when the menu was selected on one of them.
	 */
	void changeFont(QFont font);

	/**
	 * returns a rectangle where all segments of the assocication are in
	 */
	QRect getAssocLineRectangle();


	/**
	 * Return the first font found being  used
	 * by any child widget (they could be different fonts,
	 * so this is slightly mis-leading method)
         */
	QFont getFont () const;

	/**
	 * set all 'owned' child widgets to this font
	 */
	void setFont (QFont font);


	/**
	 * Read property of FloatingText * m_pChangeWidgetA.
	 */
	FloatingText* getChangeWidgetA();

	/**
	 * Read property of FloatingText * m_pChangeWidgetB.
	 */
	FloatingText* getChangeWidgetB();


	/**
	 * Sets the text to the FloatingText that display the Role text of this association
	 * For this funtion to work properly, the associated widgets (m_pWidgetA and m_pWidgetB)
	 * should be already set
	 */
	bool setRoleNameA(QString strRole);
	bool setRoleNameB(QString strRole);

	/**
	 * Set the documentation on this association
	 */
	void setDoc(QString doc);

	/**
	 * Set the documentation on roles A,B.
	 */
	void setRoleADoc(QString doc);
	void setRoleBDoc(QString doc);

	/**
	 * Returns the UMLAssociation representation of this object.
	 */
	UMLAssociation * getAssociation () {
		return m_pAssociation;
	}

	/**
	 * Calls @ref setTextPosition() on all the labels
	 */
	void resetTextPositions();

	/**
	*  Sets the Association line index for widgetA
	*/
	void setIndexA(int index) {
		m_nIndexA = index;
	}

	/**
	*  Returns the Association line index for widgetA
	*/
	int getIndexA() const {
		return m_nIndexA;
	}

	/**
	*  Sets the Association line index for widgetB
	*/
	void setIndexB(int index) {
		m_nIndexB = index;
	}

	/**
	*  Returns the Association line index for widgetB
	*/
	int getIndexB() const {
		return m_nIndexB;
	}

	/**
	*  Sets the total count on the Association region for widgetA
	*/
	void setTotalCountA(int count) {
		m_nTotalCountA = count;
	}

	/**
	*  Returns the total count on the Association region for widgetA
	*/
	int getTotalCountA() const {
		return m_nTotalCountA;
	}

	/**
	*  Sets the total count on the Association region for widgetB
	*/
	void setTotalCountB(int count) {
		m_nTotalCountB = count;
	}

	/**
	*  Region the total count on the Association region for widgetB
	*/
	int getTotalCountB() const {
		return m_nTotalCountB;
	}

	bool saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

	bool loadFromXMI( QDomElement & qElement );

private:
	/**
	 * Merges/syncs the association widget data into UML object
	 * representation.
	 * FIXME: Get rid of this.
	 */
	void mergeAssociationDataIntoUMLRepresentation();

	/**
	 * Finds out in which region contains the Point (PosX, PosY) and returns the region
	 * number
	 */
	AssociationWidget::Region findPointRegion(QRect Rect, int PosX, int PosY);

	/**
	 *  Overrides moveEvent
	 */
	void moveEvent(QMoveEvent *me);

	/**
	 * This function calculates which role should be set for the m_pName* FloatingText
	 */
	Text_Role CalculateNameType(Text_Role defaultRoleType);

	/**
	 * Calculates and sets the first and last point in the Association's LinePath
	 * Each point is a middle point of its respecting UMLWidget's Bounding rectangle
	 * This method picks which sides to use for the association
	 */
	void calculateEndingPoints();

	/**
	 * Returns true if point (PosX, PosY) is close enough to any of association's segments
	 */
	bool isPointInsideBoundaries(int PosX, int PosY, QPoint & SPoint,
				     uint & StartSegmentIndex, uint & EndSegmentIndex);

	/**
	 * Returns the intersection point between line P1P2 and the bounding rectangle of pWidget
	 */
	QPoint findRectIntersectionPoint(UMLWidget* pWidget, QPoint P1, QPoint P2);

	/**
	 * Returns the intersection point between lines P1P2 and P3P4
	 */
	QPoint findIntersection(QPoint P1, QPoint P2, QPoint P3, QPoint P4);

	/**
	 * Returns the total lenght of the assocition's LinePath:
	 * result = segment_1_lenght + segment_2_lenght + ..... + segment_n_lenght
	 */
	float totalLength();

	/**
	 * Calculates which point of segment P1P2 has a distance equal to Distance from P1,
	 * Lets say such point is PX,  the distance from P1 to PX must be equal to Distance
	 * and if PX is not a point of the segment P1P2 then the function returns (-1,-1)
	 */
	QPoint calculatePointAtDistance(QPoint P1, QPoint P2, float Distance);

	/**
	 * Calculates which point of a perpendicular line to segment P1P2 that contains P2
	 * has a distance equal to Distance from P2,
	 * Lets say such point is PX,  the distance from P2 to PX must be equal to Distance
	 */
	QPoint calculatePointAtDistanceOnPerpendicular(QPoint P1, QPoint P2, float Distance);

	/**
	 * Calculates the intersection between line P1P2 and a perpendicular line containing
	 * P3, the result is returned in ResultingPoint. and result value represents the distance
	 * between ResultingPoint and P3; if this value is negative an error ocurred.
	 */
	float perpendicularProjection(QPoint P1, QPoint P2, QPoint P3, QPoint ResultingPoint);

	/**
	 * Calculates the position of the text widget depending on the role
	 * that widget is under-taking.  Returns the point at which to put the widget.
	 */
	QPoint calculateTextPosition(Text_Role role);

	/**
	 * Puts the text widget with the given role at the given position.
	 * This method is usually called after calling @ref calculateTextPostion
	 * to get the needed postion.  It is a seperate function incase there is a need
	 * to place the text without calculation.  i.e.  The line segment it is on has moved
	 * and it should move the same amount as the line.
	 */
	void setTextPosition(Text_Role role, QPoint pos);

	/**
	 * Moves the text widget with the given role by the difference between the two points
	 */
	void setTextPositionRelatively(Text_Role role, QPoint pos, QPoint oldPosition);

	/**
	 * Returns the Region the widget to line intersection is for the given widget
	 * in this Association.  If the given widget is not in the Association then
	 * Region::Error is returned.
	 * Used by @ref calculateEndingPoints to work these positions out for another
	 * Association, Since the number of Associations on the same region for the
	 * same widget will mean the lines will need to be spread out across the region.
	 */
	Region getWidgetRegion(AssociationWidget * widget) const;

	/** This is a pointer to the Floating Text widget which displays the name of this
	 * association.
	 */
	FloatingText* m_pName;

	/**
	 * This is a pointer to the Floating Text widget at the starting side of the association.
	 * This FloatingText displays the information regarding multiplicity
	 */
	FloatingText* m_pMultiA;

	/**
	 * This is a pointer to the Floating Text widget at the ending side of the association.
	 * This FloatingText displays the information regarding multiplicity
	 */
	FloatingText* m_pMultiB;

	/**
	 * This is a pointer to the Floating Text widget at the "A" side of the association.
	 * This FloatingText displays the information regarding changeability
	 */
	FloatingText* m_pChangeWidgetA;

	/**
	 * This is a pointer to the Floating Text widget at the "A" side of the association.
	 * This FloatingText displays the information regarding changeability
	 */
	FloatingText* m_pChangeWidgetB;

	/**
	 * This member holds a pointer to the floating text that displays role A of this
	 * association
	 */
	FloatingText* m_pRoleA;

	/**
	 * This member holds a pointer to the floating text that displays role B of this
	 * association
	 */
	FloatingText* m_pRoleB;

	/**
	 * This member holds a pointer to the UMLWidget at the starting side of the association
	 */
	UMLWidget* m_pWidgetA;

	/**
	 * This member holds a pointer to the UMLWidget at the ending side of the association
	 */
	UMLWidget* m_pWidgetB;

	/**
	 * WidgetA's Old TopLeft Corner before moving
	 */
	QPoint m_OldCornerA;

	/**
	 * WidgetB's Old TopLeft Corner before moving
	 */
	QPoint m_OldCornerB;

	/**
	 * Old WidgetA's TopLeft Region according to WidgetB's bounding rectangle
	 */
	int m_nCornerARegion;

	/**
	 * Old WidgetA's TopLeft Region according to WidgetB's bounding rectangle
	 */
	int m_nCornerBRegion;

	/**
	 * Returns the region the line is on for widgetA.
	 */
	Region getWidgetARegion() const {
		return m_WidgetARegion;
	}

	/**
	 * Returns the region the line is on for widgetB.
	 */
	Region getWidgetBRegion() const {
		return m_WidgetBRegion;
	}

	/**
	 * Called to tell the association that another association has added
	 * a line to the region of one of its' widgets. true - widgetA
	 * false - widgetB.
	 *
	 * Called by @ref updateAssociations which is called by
	 * @ref calculateEndingPoints when required.
	 */
	void updateRegionLineCount(int index, int totalCount, AssociationWidget::Region region , bool widgetA);

protected:

	/**
	 * Tells all the other view associations the new count for the
	 * given widget on a certain region. And also what index they should be.
	 */
	void updateAssociations(int totalCount, Region region, bool widgetA);

	/**
	 * Returns the number of lines there are on the given region for
	 * either widget a or b of the association.  Setting the boolean value
	 * to true will give a count for widgetA and false for widgetB.
	 */
	int getRegionCount(Region region, bool widgetA);

private:

	/**
	 * initialize attributes of this class at construction time
	 */
	void init (QWidget *parent);

	// yes, this is correct, we dont want other classes/users seeing this
	// they should use setChangeability[AB] instead
	bool setChangeWidgetA (QString strChangeWidgetA);

	// yes, this is correct, we dont want other classes/users seeing this
	bool setChangeWidgetB (QString strChangeWidgetB);

	/**
	 *  Checks to see if the given point is one of the points of the line.
	 *  If so will try and get the view to flag the point for moving.
	 *  This is only valid if no other point id being moved and only
	 *  while the left mouse button is down.
	 */
	void checkPoints(QPoint p);

	/*QPixmap 	*m_pPix;
	QBitmap 	*m_pMask;*/

	UMLView 	* m_pView;
	Region 		m_WidgetARegion,
			m_WidgetBRegion;

	/**
	 * It is true if the Activate Function has been called for this
	 * class instance
	 */
	bool m_bActivated;

	/**
	 * When the association has a Role Floating Text this text should move when the
	 * LinePath moves but only if the closest segment to the role text moves.
	 * this segment is m_LinePath[m_unNameLineSegment] -- m_LinePath[m_unNameLineSegment + 1]
	 */
	uint 		m_unNameLineSegment;
	bool 		m_bFocus;
	ListPopupMenu 	*m_pMenu;
	UMLAssociation  *m_pAssociation;
	bool 		m_bSelected;
	int 		m_nMovingPoint;

	// Data loaded/saved:

	/**
	 * The definition points for the association line
	 */
	LinePath m_LinePath;

	/**
	 * The index of where the line is on the region for widget a
	 */
	int m_nIndexA;

	/**
	 * The index of where the line is on the region for widget b
	 */
	int m_nIndexB;

	/**
	 * The total amount of associations on the region widget a line is on.
	 */
	int m_nTotalCountA;

	/**
	 * The total amount of associations on the region widget b line is on.
	 */
	int m_nTotalCountB;

	/**
	 * The following items are only used if m_pAssociation is not set.
	 */
	Uml::Association_Type m_AssocType;
	int m_nWidgetAID;
	int m_nWidgetBID;
	Scope m_VisibilityA, m_VisibilityB;
	Changeability_Type m_ChangeabilityA, m_ChangeabilityB;
	QString m_RoleADoc, m_RoleBDoc;
	QString m_RoleNameA, m_RoleNameB;
	QString m_MultiA, m_MultiB;
	QString m_Doc;

public slots:
	/**
	 * Handles the selection from the popup menu.
	 */
	void slotMenuSelection(int sel);

	/**
	 * This slot is entered when an event has occurred on the views display,
	 * most likely a mouse event.  Before it sends out that mouse event all
	 * children should make sure that they don't have a menu active or there
	 * could be more than one popup menu displayed.
	 */
	void slotRemovePopupMenu();

	/**
	 * Handles any signals that tells everyone not to be selected.
	 */
	void slotClearAllSelected();

	/**
	 * Merge together the UMLAssociation representation and this widget
	 */
	void mergeUMLRepresentationIntoAssociationData();
};
#endif
