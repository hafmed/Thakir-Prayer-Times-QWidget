#include "engine.h"

#include <QTime>
#include <QTextStream>
#include <QDBusInterface>

#include <QGuiApplication>


Engine::Engine()
{
    init();
}

void Engine::autostart_hidden(bool flag)
{
    hidden = flag;
}

bool Engine::autostart_hidden()
{
    return hidden;
}

namespace
{
void autostart_snap(bool set)
{
    if(!QDir("/snap").exists())
    {
        return;
    }

    QDir autostartDir = QDir::homePath() + QDir::separator() + ".config/autostart";

    if(!autostartDir.exists())
    {
        if(!autostartDir.mkpath(autostartDir.path())) return;
    }

    QFile desktopFile("/snap/munadi/current/usr/share/applications/org.munadi.Munadi.desktop");
    QFile autostartDesktopFile(autostartDir.filePath("org.munadi.Munadi.desktop"));

    if(set)
    {
        if (!desktopFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open .desktop file!";
            return;
        }

        if (!autostartDesktopFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Failed to create .desktop file!";
            return;
        }

        QTextStream in(&desktopFile);
        QString data = in.readAll();
        data.replace("Exec=munadi", "Exec=munadi --hidden");
        QTextStream out(&autostartDesktopFile);
        out << data;
    }
    else
    {
        autostartDesktopFile.remove();
    }
}
void autostart_flatpak(bool set)
{
    QDBusInterface portal("org.freedesktop.portal.Desktop",
                          "/org/freedesktop/portal/desktop",
                          "org.freedesktop.portal.Background");

    QList<QString> commandline = {"munadi", "--hidden"};

    QHash<QString, QVariant> params;
    params.insert("reason", "Background Athan.");
    params.insert("autostart", set);
    params.insert("commandline", QVariant(commandline));

    portal.call("RequestBackground", "", params);
}
}

void Engine::autostart(bool set)
{


    // Can't tell if we're on Snap, Flatpak or otherwise, so do all!
    autostart_snap(set);
    autostart_flatpak(set);


}

bool Engine::init()
{

    return true;
}
