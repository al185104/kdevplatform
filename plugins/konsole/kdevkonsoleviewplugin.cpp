/***************************************************************************
*   Copyright 2003, 2006 Adam Treat <treat@kde.org>                       *
*   Copyright 2007 Andreas Pakulat <apaku@gmx.de>                         *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "kdevkonsoleviewplugin.h"

#include <kgenericfactory.h>
#include <kaboutdata.h>

#include <interfaces/iuicontroller.h>
#include <interfaces/icore.h>
#include <kservice.h>
#include "kdevkonsoleview.h"

QObject* createKonsoleView( QWidget*, QObject* op, const QVariantList& args)
{
    KService::Ptr service = KService::serviceByDesktopName("konsolepart");
    KPluginFactory* factory = nullptr;
    if (service) {
        factory = KPluginLoader(*service.data()).factory();
    }
    if (!factory) {
        kWarning() << "Failed to load 'konsolepart' plugin";
    }
    return new KDevKonsoleViewPlugin(factory, op, args);
}

K_PLUGIN_FACTORY(KonsoleViewFactory, registerPlugin<KDevKonsoleViewPlugin>( QString(), &createKonsoleView ); )
// K_EXPORT_PLUGIN(KonsoleViewFactory(KAboutData("kdevkonsoleview","kdevkonsole", ki18n("Konsole"), "0.1", ki18n("This plugin provides KDevelop with an embedded konsole for quick and easy command line access."), KAboutData::License_GPL)))

class KDevKonsoleViewFactory: public KDevelop::IToolViewFactory{
public:
    KDevKonsoleViewFactory(KDevKonsoleViewPlugin *plugin):
        mplugin(plugin) {}
    virtual QWidget* create(QWidget *parent = 0)
    {
        return new KDevKonsoleView(mplugin, parent);
    }
    virtual Qt::DockWidgetArea defaultPosition()
    {
        return Qt::BottomDockWidgetArea;
    }
    virtual QString id() const
    {
        return "org.kdevelop.KonsoleView";
    }
private:
    KDevKonsoleViewPlugin *mplugin;
};

KDevKonsoleViewPlugin::KDevKonsoleViewPlugin( KPluginFactory* konsoleFactory, QObject *parent, const QVariantList & )
    : KDevelop::IPlugin( QStringLiteral("kdevkonsoleview"), parent )
    , m_konsoleFactory(konsoleFactory)
    , m_viewFactory(konsoleFactory ? new KDevKonsoleViewFactory(this) : nullptr)
{
    if (m_viewFactory) {
        core()->uiController()->addToolView("Konsole", m_viewFactory);
    }
}

void KDevKonsoleViewPlugin::unload()
{
    if (m_viewFactory) {
        core()->uiController()->removeToolView(m_viewFactory);
    }
}

bool KDevKonsoleViewPlugin::hasError() const
{
    return !m_viewFactory;
}

QString KDevKonsoleViewPlugin::errorDescription() const
{
    return !m_viewFactory ? i18n("Failed to load 'konsolepart' plugin") : QString();
}

KPluginFactory* KDevKonsoleViewPlugin::konsoleFactory() const
{
    return m_konsoleFactory;
}

KDevKonsoleViewPlugin::~KDevKonsoleViewPlugin()
{
}

#include "kdevkonsoleviewplugin.moc"

