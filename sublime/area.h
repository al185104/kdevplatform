/***************************************************************************
 *   Copyright 2006-2007 Alexander Dymo  <adymo@kdevelop.org>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef SUBLIMEAREA_H
#define SUBLIMEAREA_H

#include <QtCore/QObject>

#include "sublimeexport.h"

#include "areaindex.h"
#include "sublimedefs.h"

#include <KDE/KConfigGroup>

namespace Sublime {

class AreaIndex;
class RootAreaIndex;
class Controller;
class View;

/**
@short Area - the universal view container

Area contains views and toolviews, knows about their positions
and provides functionality to add new (tool)views and remove existing.

Area takes care of all placement/configuration details so that
in order for @ref MainWindow to show the area it just needs to
reconstruct itself according to the area's rules.

Usual way of creating an area is:
@code
Controller *controller = new Controller();
... //document creation code here
Area *area = new Area(controller, "My Area");
area->addView(document->createView());
MainWindow *mw = new MainWindow(controller);
controller->show(area, mw);
@endcode
*/
class SUBLIME_EXPORT Area: public QObject {
    Q_OBJECT
public:
    /**Creates and area with given @p name and adds it to the @p controller.
    @param controller is the controller in which this area will be available.
    @param name should identify this area and be unique for all areas in the controller.
        @ref QObject::objectName shall be used to get this name after area creation.
    @param title is the user-visible (translatable) title for the area.
        Use @ref title and @ref setTitle to operate on the title.
        This parameter can be omitted and then name will be used as title.*/
    Area(Controller *controller, const QString &name, const QString &title = "");
    Area(const Area &area);
    ~Area();

    QString title() const;
    void setTitle(const QString &title);

    /**Adds the @p view to the list of views at the given area index.*/
    void addView(View *view, AreaIndex *index);

    /**Adds the @p view to the list of views at the current area index.*/
    void addView(View *view, View *after = 0);
    /**Adds the @p view to the area splitting the @p viewToSplit using
    given @p orientation.*/
    void addView(View *view, View *viewToSplit, Qt::Orientation orientation);
    /**Removes the @p view from the area.*/
    View* removeView(View *view);
    /**@return the list of all views in this area in no particular order. To
    process the views in ordered manner (tree walk) use @ref walkViews method.
    This method is added only for convenience.*/
    QList<View*> views();

    /**@return the index of view or 0 if it can not be found.*/
    AreaIndex *indexOf(View *view);
    /**@return the root index of the area. Root index always exists so this
    method will never return 0.*/
    RootAreaIndex *rootIndex() const;

    /**Adds the toolview to the area. Area will use its configuration and restore
    the proper position for the toolview when necessary. If it has no configuration
    for this view, it will use @p defaultPosition.*/
    void addToolView(View *toolView, Position defaultPosition);

    /**Removes the toolview from the area.*/
    View* removeToolView(View *toolView);

    /**Moves the toolview to a different position.  */
    void moveToolView(View *toolView, Position newPosition);

    /**Raise tool view.*/
    void raiseToolView(View *toolView);
    /**@return the list of toolviews in the area. No particular sort order is guaranteed.*/
    QList<View*> &toolViews() const;
    /**@return the current position of @p toolView in the area.*/
    Position toolViewPosition(View *toolView) const;

    /* Returns true if this area actively desires to show a tool view
       with id of 'id'.  The area, of course, will show any tool view
       added with 'addToolView', however, this method can be used
       to guess a set of tool views that make most sense to be added.  */
    bool wantToolView(const QString& id);
    void setShownToolView(Sublime::Position pos, const QString& id);
    QString shownToolView(Sublime::Position pos) const;
    void setDesiredToolViews(
        const QMap<QString, Sublime::Position>& desiredToolViews);

    void setThickness(Sublime::Position pos, int thickness);
    int thickness(Sublime::Position pos) const;

    void save(KConfigGroup& group) const;
    void load(const KConfigGroup& group);

    /**@return the controller for this area.*/
    Controller *controller() const;

    /**Walker mode to determine the behavior of area walkers.*/
    enum WalkerMode {
        StopWalker,       /**< Stop after processing this area index or toolview */
        ContinueWalker    /**< Continue walking */
    };

    /**Walks the tree of area indices and executes the operator. It will always walk the
    tree of views from top to bottom from left to right.

    Operator should be the class with <i>WalkerResult operator()(AreaIndex *index)</i>
    method. That method should return Area::StopWalker if the walker has to stop at current index
    or Area::ContinueWalker to continue.

    Example (operator to print the indices, assumes hypothetical operator <<()):
    @code
    struct MyOperator {
        WalkerMode operator()(AreaIndex *index) {
            std::cerr << index << std::endl;
            return Area::ContinueWalker;
        }
    };
    ...
    MyOperator op;
    walkViews(op, rootIndex())
    @endcode*/
    template <typename Operator>
    void walkViews(Operator &op, AreaIndex *index);

    /**Walks the list of toolviews. The order in which toolviews are walked is not specified.

    Operator should be the class with <i>bool operator()(View *view, Sublime::Position position)</i>
    method. That method should return Area::StopWalker if the walker has to stop at current index
    or Area::ContinueWalker to continue.

    Example (operator to print the list of views):
    @code
    struct MyOperator {
        WalkerMode operator()(View *view, Sublime::Position position) {
            std::cerr << view << " at position " << position << std::endl;
            return Area::ContinueWalker;
        }
    };
    ...
    MyOperator op;
    walkToolViews(op, Sublime::AllPositions)
    @endcode
    */
    template <typename Operator>
    void walkToolViews(Operator &op, Positions positions);

Q_SIGNALS:
    /**Emitted when a new view is added to the area.*/
    void viewAdded(Sublime::AreaIndex*, Sublime::View*);
    /**Emitted when a view is going to be removed from the area.*/
    void aboutToRemoveView(Sublime::AreaIndex*, Sublime::View*);
    /**Emitted when a new toolview is added to the area.*/
    void toolViewAdded(Sublime::View*, Sublime::Position);
    /**Emitted when a toolview is requesting to be raised.*/
    void requestToolViewRaise(Sublime::View*);
    /**Emitted when a toolview is going to be removed from the area.*/
    void aboutToRemoveToolView(Sublime::View*, Sublime::Position);
    /**Emitted when a toolview is moved to a different position.*/
    void toolViewMoved(Sublime::View*, Sublime::Position);

private:
    template <typename Operator>
    WalkerMode walkViewsInternal(Operator &op, AreaIndex *index);

    void initialize();

    struct AreaPrivate *const d;

};

}

#include "areawalkers.h"

#endif

