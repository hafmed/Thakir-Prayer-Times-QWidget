#ifndef MUNADIENGINE_H
#define MUNADIENGINE_H

#include <QtGui>
#include <QtCore>
#include <QtDebug>


class Engine : public QObject
{
    Q_OBJECT

public:

    Engine();

    Q_INVOKABLE void autostart(bool set);

    Q_INVOKABLE QString ff()    // Form factor
    {
#if defined Q_OS_WIN || defined Q_OS_OSX || (defined Q_OS_LINUX && !defined Q_OS_ANDROID)
    return "d";
#else
    return "m";
#endif
    }

    //Updater * updater;
    void autostart_hidden(bool flag);
    Q_INVOKABLE bool autostart_hidden();

private:

    bool hidden = false;

    bool init();
};

#endif // MUNADIENGINE_H
