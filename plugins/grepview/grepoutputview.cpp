/**************************************************************************
*   Copyright 2010 Silvère Lestang <silvere.lestang@gmail.com>            *
*   Copyright 2010 Julien Desgats <julien.desgats@gmail.com>              *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "grepoutputview.h"
#include "grepoutputmodel.h"
#include "grepoutputdelegate.h"
#include "ui_grepoutputview.h"
#include "grepviewplugin.h"

#include <QtGui/QAction>

#include <interfaces/icore.h>

using namespace KDevelop;

GrepOutputViewFactory::GrepOutputViewFactory()
{}

QWidget* GrepOutputViewFactory::create(QWidget* parent)
{
    return new GrepOutputView(parent);
}

Qt::DockWidgetArea GrepOutputViewFactory::defaultPosition()
{
    return Qt::BottomDockWidgetArea;
}

QString GrepOutputViewFactory::id() const
{
    return "org.kdevelop.GrepOutputView";
}

GrepOutputView::GrepOutputView(QWidget* parent)
  : QWidget(parent)
{
    Ui::GrepOutputView::setupUi(this);

    setWindowTitle(i18n("Replace output view"));
    setWindowIcon(SmallIcon("edit-find"));
    
    m_apply = new QAction(KIcon("dialog-ok-apply"), i18n("&Replace"), this);
    QAction *previous = new QAction(KIcon("go-previous"), i18n("&Previous"), this);
    QAction *next = new QAction(KIcon("go-next"), i18n("&Next"), this);
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    QAction *change_criteria = new QAction(KIcon("configure"), i18n("&Change criteria"), this);
    
    addAction(m_apply);
    addAction(previous);
    addAction(next);
    addAction(separator);
    addAction(change_criteria);
    
    resultsTreeView->setModel(new GrepOutputModel);
    resultsTreeView->setItemDelegate(GrepOutputDelegate::self());
    resultsTreeView->setHeaderHidden(true);
    
    connect(m_apply, SIGNAL(triggered(bool)), this, SLOT(onApply()));
    connect(previous, SIGNAL(triggered(bool)), this, SLOT(selectPreviousItem()));
    connect(next, SIGNAL(triggered(bool)), this, SLOT(selectNextItem()));
    connect(change_criteria, SIGNAL(triggered(bool)), this, SLOT(showDialog()));
}

GrepOutputView::~GrepOutputView()
{
    emit outputViewIsClosed();
}

GrepOutputModel* GrepOutputView::renewModel()
{
    GrepOutputModel* oldModel = model();
    GrepOutputModel* newModel = new GrepOutputModel(resultsTreeView);
    resultsTreeView->setModel(newModel);
    connect(resultsTreeView, SIGNAL(activated(QModelIndex)), newModel, SLOT(activate(QModelIndex)));
    connect(newModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(expandRootElement(QModelIndex)));
    connect(newModel, SIGNAL(showErrorMessage(QString,int)), this, SLOT(showErrorMessage(QString)));
    oldModel->deleteLater();
    return newModel;
}


GrepOutputModel* GrepOutputView::model()
{
    return static_cast<GrepOutputModel*>(resultsTreeView->model());
}

void GrepOutputView::setPlugin(GrepViewPlugin* plugin)
{
    m_plugin = plugin;
}

void GrepOutputView::setMessage(const QString& msg)
{
    messageLabel->setText(msg);
}

void GrepOutputView::enableReplace(bool enable)
{
    m_apply->setEnabled(enable);
}

void GrepOutputView::showErrorMessage( const QString& errorMessage )
{
    setStyleSheet("QLabel { color : red; }");
    setMessage(errorMessage);
}

void GrepOutputView::showMessage( KDevelop::IStatus* , const QString& message )
{
    setStyleSheet("");
    setMessage(message);
}

void GrepOutputView::onApply()
{
    setEnabled(false);
    model()->doReplacements();
    setEnabled(true);
}

void GrepOutputView::showDialog()
{
    m_plugin->showDialog(true);
}

void GrepOutputView::expandRootElement(const QModelIndex& parent)
{
    if(!parent.isValid())
    {
        resultsTreeView->setExpanded(model()->index(0,0), true);
    }
}

void GrepOutputView::selectPreviousItem()
{
    QModelIndex idx = resultsTreeView->currentIndex();
    if(idx.isValid())
    {
        QModelIndex prev_idx = model()->previousItemIndex(idx);
        resultsTreeView->setCurrentIndex(prev_idx);
        model()->activate(prev_idx);
    }
}

void GrepOutputView::selectNextItem()
{
    QModelIndex idx = resultsTreeView->currentIndex();
    if(idx.isValid())
    {
        QModelIndex next_idx = model()->nextItemIndex(idx);
        resultsTreeView->setCurrentIndex(next_idx);
        model()->activate(next_idx);
    }
}