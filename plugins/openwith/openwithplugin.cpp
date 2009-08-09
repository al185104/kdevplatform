/*
 * This file is part of KDevelop
 * Copyright 2009  Andreas Pakulat <apaku@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "openwithplugin.h"
#include <QVariantList>

#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kaction.h>
#include <interfaces/contextmenuextension.h>
#include <interfaces/context.h>
#include <project/projectmodel.h>
#include <kmimetype.h>
#include <kservice.h>
#include <kservicetypetrader.h>
#include <kmimetypetrader.h>
#include <QSignalMapper>
#include <kmenu.h>

using namespace KDevelop;

K_PLUGIN_FACTORY(KDevOpenWithFactory, registerPlugin<OpenWithPlugin>(); )
K_EXPORT_PLUGIN(KDevOpenWithFactory(KAboutData("kdevopenwith","kdevopenwith", ki18n("Open With"), "0.1", ki18n("Open files with external applications."), KAboutData::License_GPL)))


OpenWithPlugin::OpenWithPlugin ( QObject* parent, const QVariantList& ) 
    : IPlugin ( KDevOpenWithFactory::componentData(), parent ),
    actionMap( 0 )
{
    setXMLFile( "kdevopenwithui.rc" );
}

OpenWithPlugin::~OpenWithPlugin()
{
}

KDevelop::ContextMenuExtension OpenWithPlugin::contextMenuExtension ( KDevelop::Context* context )
{
    if( actionMap )
    {
        delete actionMap;
    }
    KUrl url;
    FileContext* filectx = dynamic_cast<FileContext*>( context );
    ProjectItemContext* projctx = dynamic_cast<ProjectItemContext*>( context );
    if( filectx && filectx->urls().count() == 1 )
    {
        url = filectx->urls().first();
    } else if ( projctx && projctx->items().count() == 1 )
    {
        ProjectBaseItem* item = projctx->items().first();
        if( item->file() )
        {
            url = item->file()->url();
        }
    }
    if( !url.isEmpty() && url.isValid() )
    {
        actionMap = new QSignalMapper( this );
        connect( actionMap, SIGNAL(mapped(const QString&)), SLOT(open(const QString&)) );
        
        // Ok, lets fetch the mimetype for the url and the relevant services
        KMimeType::Ptr mimetype = KMimeType::findByUrl( url );
        KService::List apps = KMimeTypeTrader::self()->query( mimetype->name() );
        KService::Ptr preferredapp = KMimeTypeTrader::self()->preferredService( mimetype->name() );
        KService::List parts = KMimeTypeTrader::self()->query( mimetype->name(), "KParts/ReadOnlyPart" );
        KService::Ptr preferredpart = KMimeTypeTrader::self()->preferredService( mimetype->name(), "KParts/ReadOnlyPart" );
        
        // Now setup a menu with actions for each part and app
        KMenu* menu = new KMenu( i18n("Open With" ) );
        
        menu->addActions( actionsForServices( parts, preferredpart, "|__PART__" ) );
        menu->addActions( actionsForServices( apps, preferredapp, "|__APP__" ) );
        
        KDevelop::ContextMenuExtension ext;
        ext.addAction( KDevelop::ContextMenuExtension::FileGroup, menu->menuAction() );
        return ext;
    }
    return KDevelop::IPlugin::contextMenuExtension ( context );
}


QList< QAction* > OpenWithPlugin::actionsForServices ( const KService::List& list, KService::Ptr pref, const QString& prefix )
{
    QList<QAction*> openactions;
    foreach( KService::Ptr svc, list )
    {
        KAction* act = new KAction( svc->name(), this );
        act->setIcon( KIcon( svc->icon() ) );
        connect(act, SIGNAL(triggered()), actionMap, SLOT(map()));
        actionMap->setMapping( act, svc->storageId()+prefix );
        if( svc->storageId() == pref->storageId() )
        {
            openactions.prepend( act );
        } else
        {
            openactions.append( act );
        }
    }
    return openactions;
}

void OpenWithPlugin::open ( const QString& storageid )
{
    kDebug() << "opening:" << storageid;
}