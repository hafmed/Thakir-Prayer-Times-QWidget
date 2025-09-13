/****************************************************************************
** HAFIANE 5-6-2009 - 25-6-2020
**
** # on Qt 5.9.5 GCC 7.3.0; 64 bit Qt Creator 4.5.2 Ubuntu 18.4
# sudo apt install g++
# sudo apt install cmake
# sudo apt-get install libgl-dev
# sudo apt-get install qt5-default
# sudo apt install qtmultimedia5-dev
#
# sudo apt-get install qtgstreamer-plugins-qt5
# sudo apt-get install libqt5multimedia5-plugins
#
# sudo apt-get update
# sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
#
# sudo ln -s /home/hafmed/Qt5.12.8/5.12.8/gcc_64/bin/qmake /usr/lib/x86_64-linux-gnu/qt4/bin/qmake
#
# ./linuxdeployqt-6-x86_64.AppImage Thakir_PrayerTimes_HAF -appimage
#------------Make *.AppImage----------------------
# in the directorie : compilation (vider le repertoire sauf .desctop .ico )
# chmod a+x linuxdeployqt-6-x86_64.AppImage
# ./linuxdeployqt-6-x86_64.AppImage Thakir_Prayer_Times -unsupported-allow-new-glibc -appimage
#-------------------------------------------------
****************************************************************************/

#include <QApplication>

#include "thakir_prayer_times.h"

#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Engine engine;

    if(argc > 1 && strcmp(argv[1], "--hidden") == 0)
    {
        engine.autostart_hidden(true);

    }

    thakir_prayer_times *dialog = new thakir_prayer_times;
    QPixmap pixmap = QPixmap (":/icons/io.github.hafmed.Thakir-Prayer-Times.ico");
    dialog->setWindowIcon(QIcon(pixmap));
    app.setApplicationName("ذكر لأوقات الصلاة");
    app.setOrganizationName("hafsoftdz");
    app.setOrganizationDomain("org.hafsoftdz");
    dialog->show();
    return app.exec();

}
