/****************************************************************************
** HAFIANE 7-9-2009 / 5-9-2020
** ./linuxdeployqt-6-x86_64.AppImage thakir_prayer_times -unsupported-allow-new-glibc -appimage
****************************************************************************/
//#include <stdio.h>
//#include <stdlib.h>
#include "prayer.h"
//#include <iostream>
//#include <string>
#include <QMessageBox>
#include <QtGui>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>
#include <QAction>
#include <QMenu>

#include <QMediaPlayer>

#include "thakir_prayer_times.h"

#include <QDebug>

#include <QMediaMetaData>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaFormat>
#include <QtWidgets>

#include <QUrl>
#include <iostream>
using namespace std;

#include "engine.h"
#include <QDBusInterface>

thakir_prayer_times::thakir_prayer_times(QWidget *parent)
    : QMainWindow(parent),
      athan(nullptr)
{
    setupUi(this);

    if (checkBox_autostart->isChecked()){
        on_checkBox_autostart_stateChanged(2);
    }else{
        on_checkBox_autostart_stateChanged(0);
    }

    // Connect to the session bus
    QDBusConnection bus = QDBusConnection::sessionBus();

    if (!bus.isConnected()) {
        qWarning() << "Cannot connect to D-Bus session bus";
        return;
    }

    // Create interface for the notification portal
    m_portalInterface = new QDBusInterface(
        "org.freedesktop.portal.Desktop",
        "/org/freedesktop/portal/desktop",
        "org.freedesktop.portal.Notification",
        bus,
        this
        );

    if (!m_portalInterface->isValid()) {
        qWarning() << "D-Bus interface is not valid:" << m_portalInterface->lastError().message();
        delete m_portalInterface;
        m_portalInterface = nullptr;
        return;
    }

    // Connect to the ActionInvoked signal
    bus.connect("org.freedesktop.portal.Desktop",
                "/org/freedesktop/portal/desktop",
                "org.freedesktop.portal.Notification",
                "ActionInvoked",
                this,
                SLOT(onNotificationResponse(uint, uint, QVariantMap)));
    ///--------------//


    QString path ;
    QString filename;

    path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) ;
    filename = "config.ini" ;
    ///QSettings HAF_settings(path + "/"+ filename,QSettings::IniFormat) ;
    // QSettings HAF_settings("/sdcard/settings.ini", QSettings::NativeFormat); //can be IniFormat, no difference

    //HAF_settings= new QSettings("/sdcard/settings.ini", QSettings::IniFormat); //can be IniFormat, no difference

    HAF_settings = new QSettings(path + "/"+ filename,QSettings::IniFormat);

    label_10->setVisible(false);
    label_12->setVisible(false);
    label_13->setVisible(false);

    timeEdit_imsak->setVisible(false);
    timeEdit_imsak_tomorow->setVisible(false);
    timeEdit_fadjra_tomorow->setVisible(false);

    connect(pushButton_about,SIGNAL(clicked()), this, SLOT(about()));
    connect(pushButton_close,SIGNAL(clicked()), this, SLOT(hide()));

    //----- HAF 29-0-2020 -----
    connect(pushButton_close,SIGNAL(clicked()), this, SLOT(stopAthan()));
    //connect(athan, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(showQMessageBox(QMediaPlayer::State)));
    //-------------------------

    connect(tabWidget_Athan,SIGNAL(currentChanged(int )), this, SLOT(getAthanType()));

    dateTimeEdit->setDate(QDate::currentDate());
    dateTimeEdit->setTime(QTime::currentTime());
    //--------------HAF 13-7-2009-------

    connect(comboBox_madina,SIGNAL(currentIndexChanged(int)),this,SLOT(COMMUN_Update(int)));

    //------------- HAF 19-6-2009 ------
    comboBox_madina->insertItems(0,QStringList()
                                 << QString::fromUtf8("أدرار")
                                 << QString::fromUtf8("شلف")
                                 << QString::fromUtf8("لغواط")
                                 << QString::fromUtf8("أم البواقي")
                                 << QString::fromUtf8("باتنة")
                                 << QString::fromUtf8("بجاية")
                                 << QString::fromUtf8("بسكرة")
                                 << QString::fromUtf8("بشار")
                                 << QString::fromUtf8("بليدة")
                                 << QString::fromUtf8("بويرة")
                                 << QString::fromUtf8("تمراسات")
                                 << QString::fromUtf8("تبسة")
                                 << QString::fromUtf8("تلمسان")
                                 << QString::fromUtf8("تيارت")
                                 << QString::fromUtf8("تيزي وزو")
                                 << QString::fromUtf8("الجزائر")
                                 << QString::fromUtf8("جلفة")
                                 << QString::fromUtf8("جيجل")
                                 << QString::fromUtf8("سطيف")
                                 << QString::fromUtf8("سعيدة")
                                 << QString::fromUtf8("سكيكدة")
                                 << QString::fromUtf8("سيدي بلعباس")
                                 << QString::fromUtf8("عنابة")
                                 << QString::fromUtf8("قالمة")
                                 << QString::fromUtf8("قسنطينة")
                                 << QString::fromUtf8("مدية")
                                 << QString::fromUtf8("مستغانم")
                                 << QString::fromUtf8("مسيلة")
                                 << QString::fromUtf8("معسكر")
                                 << QString::fromUtf8("ورقلة")
                                 << QString::fromUtf8("وهران")
                                 << QString::fromUtf8("البيض")
                                 << QString::fromUtf8("إليزي")
                                 << QString::fromUtf8("برج بوعريريج")
                                 << QString::fromUtf8("بمرداس")
                                 << QString::fromUtf8("الطارف")
                                 << QString::fromUtf8("تندوف")
                                 << QString::fromUtf8("تسمسيلت")
                                 << QString::fromUtf8("الوادي")
                                 << QString::fromUtf8("خنشلة")
                                 << QString::fromUtf8("سوق أهراس")
                                 << QString::fromUtf8("تبازة")
                                 << QString::fromUtf8("ميلة")
                                 << QString::fromUtf8("عين الدفلة")
                                 << QString::fromUtf8("نعامة")
                                 << QString::fromUtf8("عين تموشنت")
                                 << QString::fromUtf8("غرداية")
                                 << QString::fromUtf8("غليزان"));
    //----------------------------------


    //-- HAF config sauvegarde and restore 18-6-2009--
    thakir_prayer_times_config();

    QString m_name_city_DZ_ini = HAF_settings->value("Algerie/city/name").toString();
    double m_latitude_DZ_ini = HAF_settings->value("Algerie/location/latitude").toDouble();
    double m_longitude_DZ_ini = HAF_settings->value("Algerie/location/longitude").toDouble();
    double m_local_elevation_DZ_ini = HAF_settings->value("Algerie/location/elevation").toDouble();
    double m_time_zone_DZ_ini = HAF_settings->value("Algerie/location/time_zone").toDouble();


    if (m_name_city_DZ_ini=="" && m_latitude_DZ_ini==0 && m_longitude_DZ_ini==0 && m_local_elevation_DZ_ini==0 && m_time_zone_DZ_ini==0)
    {
        updateSettings();
    }
    else
    {
        readSettings();
        updateSettings();
    }
    //------------------------------------------------
    timer = new QTimer();
    timer->start(1000);
    //QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(thakir_prayer_times_Calculer()));


    connect(radioButton_sitting_Algerie, SIGNAL(clicked()), this, SLOT(thakir_prayer_times_Activer_toolBox1()));
    connect(radioButton_sitting_monde, SIGNAL(clicked()), this, SLOT(thakir_prayer_times_Activer_toolBox2()));

    connect(radioButton_sitting_HAF, SIGNAL(clicked()), this, SLOT(radioButton_sitting_user_setNotChecked()));
    connect(radioButton_sitting_user, SIGNAL(clicked()), this, SLOT(radioButton_sitting_HAF_setNotChecked()));


    // HAF 27-5-2009 TRYICON POUR windows

    pushButton_close->setText(QString::fromUtf8("إخفاء"));

    sticon = new QSystemTrayIcon(this); // on construit notre icône de notification

    connect(sticon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    // Création du menu contextuel de notre icône
    QMenu* stmenu = new QMenu(this);

    actTexte1 = new QAction(QString::fromUtf8("إخفاء ذكر لأوقات الصلاة"), this);
    connect(actTexte1, SIGNAL(triggered()), this, SLOT(hide()));

    actTexte2 = new QAction(QString::fromUtf8("فتح ذكر لأوقات الصلاة"), this);
    connect(actTexte2, SIGNAL(triggered()), this, SLOT(showNormal()));

    actTexte3 = new QAction(QString::fromUtf8("خروج نهائيا من ذكر لأوقات الصلاة"), this);
    connect(actTexte3, SIGNAL(triggered()), qApp, SLOT(quit()));

    actTexte4 = new QAction(QString::fromUtf8("حول البرنامج"), this);
    connect(actTexte4, SIGNAL(triggered()), this, SLOT(about()));

    stmenu->addAction(actTexte1);
    stmenu->addAction(actTexte2);
    stmenu->addAction(actTexte3);
    stmenu->insertSeparator(actTexte3);
    stmenu->addAction(actTexte4);
    stmenu->insertSeparator(actTexte4);

    QIcon iconactTexte1(":/icons/go-down.png");
    actTexte1->setIcon(iconactTexte1);
    QIcon iconactTexte2(":/icons/go-up.png");
    actTexte2->setIcon(iconactTexte2);
    QIcon iconactTexte3(":/icons/application-exit.png");
    actTexte3->setIcon(iconactTexte3);
    QIcon iconactTexte4(":/icons/help-about.png");
    actTexte4->setIcon(iconactTexte4);

    stmenu->setLayoutDirection(Qt::RightToLeft);

    sticon->setContextMenu(stmenu); // On assigne le menu contextuel à l'icône de notification

    QIcon icon(":/icons/io.github.hafmed.Thakir-Prayer-Times.png");

    sticon->setIcon(icon); // On assigne une image à notre icône

    sticon->show(); // On affiche l'icône
    //	sticon->showMessage("Bonjour","Hello, world!"); // On affiche une infobulle

    //-HAF 7-12-2021-----
    athan = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    athan->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(97);
    athan->setSource(QUrl("qrc:/sound/adhan_Makka.mp3"));
    connect(athan, &QMediaPlayer::mediaStatusChanged, this, &thakir_prayer_times::showQMessageBox);

    connect(groupBox_Athan, SIGNAL(toggled(bool )), this, SLOT(getAthanType()));


    //-----
}


void thakir_prayer_times::thakir_prayer_times_Calculer()
{
    readDataFromUI();

    dateTimeEdit->setDate(QDate::currentDate());
    dateTimeEdit->setTime(QTime::currentTime());


    int deg, min;
    double sec;
    //	const char symb =   (char)0xB0;
    double qibla;
    QString cityName=QString::fromUtf8("سعيدة");
    Location loc;
    Method conf;
    Date date;

    Prayer ptList[6];
    Prayer imsaak;
    Prayer nextImsaak;
    Prayer nextFajr;

    /* fill the Date structure */
    date.day = dateTimeEdit->date().day();
    date.month = dateTimeEdit->date().month();
    date.year = dateTimeEdit->date().year();
    /* fill the location info. structure */

    //-------------  haf 12-6-2009-------------
    //-Algerie
    if (radioButton_sitting_Algerie->isChecked())
    {
        getMethod(5, &conf);  //methode pour Algerie

        if (radioButton_sitting_HAF->isChecked())
        {
            int index_comboBox_madina=comboBox_madina->currentIndex();
            int index_comboBox_madina_comune=comboBox_madina_comune->currentIndex();
            switch(index_comboBox_madina)
            {
            //voir http://www.fallingrain.com/world/AG/index.html (faite attention Elv est en ft. pour convertire en metre *0.3048
            // VOIR AUSSI http://www.qiblalocator.com/
            case 0:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("أدرار");loc.degreeLat = 27.9000;loc.degreeLong = -0.2833;loc.seaLevel = 263;loc.gmtDiff=1;break; //ok
                case 1:cityName=QString::fromUtf8("برج باجي مختار");loc.degreeLat = 21.3431;loc.degreeLong = 0.9264;loc.seaLevel = 422;loc.gmtDiff=1;break;//ok
                case 2:cityName=QString::fromUtf8("تميمون");loc.degreeLat = 29.2584;loc.degreeLong = 0.2336;loc.seaLevel = 0;loc.gmtDiff=1;break;//ok
                case 3:cityName=QString::fromUtf8("رقان");loc.degreeLat = 26.7186;loc.degreeLong = 0.1780;loc.seaLevel = 0;loc.gmtDiff=1;break;//ok
                }
                break;
            case 1:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("شلف");loc.degreeLat = 36.1647;loc.degreeLong = 1.3347;loc.seaLevel = 143;loc.gmtDiff=1;break;
                }
                break;
            case 2:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("لغواط");loc.degreeLat = 33.8000;loc.degreeLong = 2.8833;loc.seaLevel = 776;loc.gmtDiff=1;break;
                case 1:cityName=QString::fromUtf8("حاسي الرمل");loc.degreeLat = 32.9166;loc.degreeLong = 3.2666;loc.seaLevel = 774;loc.gmtDiff=1;break;
                }
                break;
            case 3:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("أم البواقي");loc.degreeLat = 35.8775;loc.degreeLong = 7.1136;loc.seaLevel = 955;loc.gmtDiff=1;break;
                }
                break;
            case 4:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("باتنة");loc.degreeLat = 35.5447;loc.degreeLong = 6.1597;loc.seaLevel = 1049;loc.gmtDiff=1;break;
                }
                break;
            case 5:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("بجاية");loc.degreeLat = 36.7500;loc.degreeLong = 5.0833;loc.seaLevel = 1;loc.gmtDiff=1;break;
                }
                break;
            case 6:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("بسكرة");loc.degreeLat = 34.8500;loc.degreeLong = 5.7333;loc.seaLevel = 87;loc.gmtDiff=1;break;
                }
                break;
            case 7:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("بشار");loc.degreeLat = 31.6167;loc.degreeLong = -2.2167;loc.seaLevel = 773;loc.gmtDiff=1;break;
                }
                break;
            case 8:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("بليدة");loc.degreeLat = 36.4686;loc.degreeLong = 2.8289;loc.seaLevel = 253;loc.gmtDiff=1;break;
                }
                break;
            case 9:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("بويرة");loc.degreeLat = 36.3800;loc.degreeLong = 3.9014;loc.seaLevel = 557;loc.gmtDiff=1;break;
                }
                break;
            case 10:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تمراسات");loc.degreeLat = 22.7850;loc.degreeLong = 5.5228;loc.seaLevel = 1377;loc.gmtDiff=1;break;
                }
                break;
            case 11:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تبسة");loc.degreeLat = 35.4042;loc.degreeLong = 8.1242;loc.seaLevel = 813;loc.gmtDiff=1;break;
                }
                break;
            case 12:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تلمسان");loc.degreeLat = 34.8783;loc.degreeLong = -1.3150;loc.seaLevel = 247;loc.gmtDiff=1;break;
                case 1:cityName=QString::fromUtf8("الغزوات");loc.degreeLat = 35.1000;loc.degreeLong = -1.8500;loc.seaLevel = 118;loc.gmtDiff=1;break;
                case 2:cityName=QString::fromUtf8("صبرة");loc.degreeLat = 34.8273;loc.degreeLong = -1.5280;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 3:cityName=QString::fromUtf8("ندرومة");loc.degreeLat = 35.0121;loc.degreeLong = -1.7446;loc.seaLevel = 974;loc.gmtDiff=1;break;
                case 4:cityName=QString::fromUtf8("الرمشي");loc.degreeLat = 35.0611;loc.degreeLong = -1.4261;loc.seaLevel = 256;loc.gmtDiff=1;break;
                case 5:cityName=QString::fromUtf8("سبدو");loc.degreeLat = 34.6425;loc.degreeLong = -1.3331;loc.seaLevel = 905;loc.gmtDiff=1;break;
                case 6:cityName=QString::fromUtf8("باب العسة");loc.degreeLat = 34.9648;loc.degreeLong = -2.0445;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 7:cityName=QString::fromUtf8("المنصورة");loc.degreeLat = 34.8650;loc.degreeLong = -1.3466;loc.seaLevel = 835;loc.gmtDiff=1;break;
                case 8:cityName=QString::fromUtf8("اولاد ميمون");loc.degreeLat = 34.9028;loc.degreeLong = -1.0364;loc.seaLevel = 717;loc.gmtDiff=1;break;
                case 9:cityName=QString::fromUtf8("سيدي الجيلالي");loc.degreeLat = 34.4658;loc.degreeLong = -1.5714;loc.seaLevel = 1659;loc.gmtDiff=1;break;
                case 10:cityName=QString::fromUtf8("شتوان");loc.degreeLat = 34.9205;loc.degreeLong = -1.2914;loc.seaLevel = 574;loc.gmtDiff=1;break;
                case 11:cityName=QString::fromUtf8("هنين");loc.degreeLat = 35.1667;loc.degreeLong = -1.6500;loc.seaLevel = 21;loc.gmtDiff=1;break;
                case 12:cityName=QString::fromUtf8("بني سنوس");loc.degreeLat = 34.6589;loc.degreeLong = -1.5475;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 13:cityName=QString::fromUtf8("سيدي مجاهد");loc.degreeLat = 34.7761;loc.degreeLong = -1.6369;loc.seaLevel = 533;loc.gmtDiff=1;break;
                case 14:cityName=QString::fromUtf8("مرسى بن مهيدي");loc.degreeLat = 35.0864;loc.degreeLong = -2.2041;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 15:cityName=QString::fromUtf8("عين تالوت");loc.degreeLat = 34.9267;loc.degreeLong = -0.9581;loc.seaLevel = 713;loc.gmtDiff=1;break;
                case 16:cityName=QString::fromUtf8("سيدي العبدلي");loc.degreeLat = 35.0708;loc.degreeLong = -1.1344;loc.seaLevel = 482;loc.gmtDiff=1;break;
                case 17:cityName=QString::fromUtf8("الحناية");loc.degreeLat = 34.9514;loc.degreeLong = -1.3681;loc.seaLevel = 477;loc.gmtDiff=1;break;
                case 18:cityName=QString::fromUtf8("مغنية");loc.degreeLat = 34.8454;loc.degreeLong = -1.7283;loc.seaLevel = 0;loc.gmtDiff=1;break;
                }
                break;
            case 13:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تيارت");loc.degreeLat = 35.3758;loc.degreeLong = 1.3131;loc.seaLevel = 1127;loc.gmtDiff=1;break;
                }
                break;
            case 14:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تيزي وزو");loc.degreeLat = 36.7169;loc.degreeLong = 4.0497;loc.seaLevel = 1127;loc.gmtDiff=1;break;
                }
                break;
            case 15:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("الجزائر");loc.degreeLat = 36.7833;loc.degreeLong = 3.0500;loc.seaLevel = 25;loc.gmtDiff=1;break;
                case 1:cityName=QString::fromUtf8("زرالدة");loc.degreeLat = 36.7147;loc.degreeLong = 2.8433;loc.seaLevel = 53;loc.gmtDiff=1;break;
                case 2:cityName=QString::fromUtf8("الشراقة");loc.degreeLat = 36.7669;loc.degreeLong = 2.9611;loc.seaLevel = 221;loc.gmtDiff=1;break;
                case 3:cityName=QString::fromUtf8("الدرارية");loc.degreeLat = 36.7360;loc.degreeLong = 3.0163;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 4:cityName=QString::fromUtf8("بئر توتة");loc.degreeLat = 36.6459;loc.degreeLong = 2.9974;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 5:cityName=QString::fromUtf8("بئر مراد رايس");loc.degreeLat = 36.7391;loc.degreeLong = 3.0504;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 6:cityName=QString::fromUtf8("بوزريعة");loc.degreeLat = 36.7910;loc.degreeLong = 3.0179;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 7:cityName=QString::fromUtf8("باب الواد");loc.degreeLat = 36.7867;loc.degreeLong = 3.0626;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 8:cityName=QString::fromUtf8("حسين داي");loc.degreeLat = 36.7344;loc.degreeLong = 3.1061;loc.seaLevel = 75;loc.gmtDiff=1;break;
                case 9:cityName=QString::fromUtf8("سيدي امحمد");loc.degreeLat = 36.7467;loc.degreeLong = 3.06768;loc.seaLevel = 75;loc.gmtDiff=1;break;
                case 10:cityName=QString::fromUtf8("الحراش");loc.degreeLat = 36.7203;loc.degreeLong = 3.1450;loc.seaLevel = 48;loc.gmtDiff=1;break;
                case 11:cityName=QString::fromUtf8("براقي");loc.degreeLat = 36.6692;loc.degreeLong = 3.0953;loc.seaLevel = 34;loc.gmtDiff=1;break;
                case 12:cityName=QString::fromUtf8("الدار البيضاء");loc.degreeLat = 36.7133;loc.degreeLong = 3.2125;loc.seaLevel = 21;loc.gmtDiff=1;break;
                case 13:cityName=QString::fromUtf8("الرويبة");loc.degreeLat = 36.7392;loc.degreeLong = 3.2853;loc.seaLevel = 19;loc.gmtDiff=1;break;
                }
                break;
            case 16:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("جلفة");loc.degreeLat = 34.6667;loc.degreeLong = 3.2500;loc.seaLevel = 1144;loc.gmtDiff=1;break;
                }
                break;
            case 17:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("جيجل");loc.degreeLat = 36.8000;loc.degreeLong = 5.7667;loc.seaLevel = 2;loc.gmtDiff=1;break;
                }
                break;
            case 18:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("سطيف");loc.degreeLat = 36.1914;loc.degreeLong = 5.4094;loc.seaLevel = 1038;loc.gmtDiff=1;break;
                }
                break;
            case 19:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("سعيدة");loc.degreeLat = 34.8304;loc.degreeLong = 0.1518;loc.seaLevel = 869;loc.gmtDiff=1;break; //OK
                case 1:cityName=QString::fromUtf8("عين الحجر");loc.degreeLat = 34.7600;loc.degreeLong = 0.1539;loc.seaLevel = 1054;loc.gmtDiff=1;break; //OK
                case 2:cityName=QString::fromUtf8("دود");loc.degreeLat = 34.9283;loc.degreeLong = -0.2136;loc.seaLevel = 656;loc.gmtDiff=1;break; //OK
                case 3:cityName=QString::fromUtf8("يوب");loc.degreeLat = 34.9283;loc.degreeLong = -0.2136;loc.seaLevel = 656;loc.gmtDiff=1;break; //OK
                case 4:cityName=QString::fromUtf8("سدي بوبكر");loc.degreeLat = 35.0286;loc.degreeLong = 0.0569;loc.seaLevel = 516;loc.gmtDiff=1;break; //OK
                case 5:cityName=QString::fromUtf8("سدي عمر");loc.degreeLat = 35.0247;loc.degreeLong = 0.1089;loc.seaLevel = 599;loc.gmtDiff=1;break; //OK
                case 6:cityName=QString::fromUtf8("سدي أحمد");loc.degreeLat = 35.0725;loc.degreeLong = 0.0122;loc.seaLevel = 445;loc.gmtDiff=1;break; //OK
                case 7:cityName=QString::fromUtf8("عين السخونة");loc.degreeLat = 34.5053;loc.degreeLong = 0.8456;loc.seaLevel = 0;loc.gmtDiff=1;break; //OK
                case 8:cityName=QString::fromUtf8("مولاي العربي");loc.degreeLat = 34.6333;loc.degreeLong = 0.0167;loc.seaLevel = 1153;loc.gmtDiff=1;break; //OK
                }
                break;
            case 20:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("سكيكدة");loc.degreeLat = 36.8622;loc.degreeLong = 6.9472;loc.seaLevel = 94;loc.gmtDiff=1;break;
                }
                break;
            case 21:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("سيدي بلعباس");loc.degreeLat = 35.1939;loc.degreeLong = -0.6414;loc.seaLevel = 476;loc.gmtDiff=1;break;
                }
                break;
            case 22:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("عنابة");loc.degreeLat = 36.9075;loc.degreeLong = 7.7572;loc.seaLevel = 1;loc.gmtDiff=1;break;
                }
                break;
            case 23:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("قالمة");loc.degreeLat = 36.4661;loc.degreeLong = 7.4339;loc.seaLevel = 257;loc.gmtDiff=1;break;
                }
                break;
            case 24:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("قسنطينة");loc.degreeLat = 36.3650 ;loc.degreeLong = 6.6147;loc.seaLevel = 501;loc.gmtDiff=1;break;
                }
                break;
            case 25:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("مدية");loc.degreeLat = 36.2675;loc.degreeLong = 2.7500;loc.seaLevel = 881;loc.gmtDiff=1;break;
                }
                break;
            case 26:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("مستغانم");loc.degreeLat = 35.9336;loc.degreeLong = 0.0868;loc.seaLevel = 104;loc.gmtDiff=1;break;
                }
                break;
            case 27:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("مسيلة");loc.degreeLat = 34.5833;loc.degreeLong = 4.5667;loc.seaLevel = 467;loc.gmtDiff=1;break;
                }
                break;
            case 28:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("معسكر");loc.degreeLat = 35.3944;loc.degreeLong = 0.1397;loc.seaLevel = 493;loc.gmtDiff=1;break; //ok
                case 1:cityName=QString::fromUtf8("بوحنفية");loc.degreeLat = 35.3156;loc.degreeLong = -0.0472;loc.seaLevel = 220;loc.gmtDiff=1;break; //ok
                case 2:cityName=QString::fromUtf8("شورفة");loc.degreeLat = 35.5211;loc.degreeLong = 0.4805;loc.seaLevel = 262;loc.gmtDiff=1;break; //ok
                case 3:cityName=QString::fromUtf8("غريس");loc.degreeLat = 35.2483;loc.degreeLong = 0.1597;loc.seaLevel = 483;loc.gmtDiff=1;break; //ok
                case 4:cityName=QString::fromUtf8("محمدية");loc.degreeLat = 35.5886;loc.degreeLong = 0.0686;loc.seaLevel = 59;loc.gmtDiff=1;break; //ok
                case 5:cityName=QString::fromUtf8("سيق");loc.degreeLat = 35.5278;loc.degreeLong = -0.1953;loc.seaLevel = 74;loc.gmtDiff=1;break; //ok
                case 6:cityName=QString::fromUtf8("تغنيف");loc.degreeLat = 35.4158;loc.degreeLong = 0.3272;loc.seaLevel = 524;loc.gmtDiff=1;break; //ok
                case 7:cityName=QString::fromUtf8("زهانة");loc.degreeLat = 35.5122;loc.degreeLong = -0.4083;loc.seaLevel = 0;loc.gmtDiff=1;break; //ok
                }
                break;
            case 29:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("ورقلة");loc.degreeLat = 31.9575;loc.degreeLong = 5.3278;loc.seaLevel = 150;loc.gmtDiff=1;break;
                }
                break;
            case 30:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("وهران");loc.degreeLat = 35.6977;loc.degreeLong = -0.6350;loc.seaLevel = 111;loc.gmtDiff=1;break;
                case 1:cityName=QString::fromUtf8("أرزيو");loc.degreeLat = 35.8517;loc.degreeLong = -0.3214;loc.seaLevel = 53;loc.gmtDiff=1;break;
                case 2:cityName=QString::fromUtf8("بوتليليس");loc.degreeLat = 35.5703;loc.degreeLong = -0.9011;loc.seaLevel = 98;loc.gmtDiff=1;break;
                case 3:cityName=QString::fromUtf8("قديل");loc.degreeLat = 35.7842;loc.degreeLong = -0.4278;loc.seaLevel = 115;loc.gmtDiff=1;break;
                case 4:cityName=QString::fromUtf8("بئر الجير");loc.degreeLat = 35.7200;loc.degreeLong = -0.5450;loc.seaLevel = 154;loc.gmtDiff=1;break;
                case 5:cityName=QString::fromUtf8("بطيوة");loc.degreeLat = 35.8025;loc.degreeLong = -0.2611;loc.seaLevel = 20;loc.gmtDiff=1;break;
                case 6:cityName=QString::fromUtf8("السانية");loc.degreeLat = 35.6511;loc.degreeLong = -0.6342;loc.seaLevel = 78;loc.gmtDiff=1;break;
                case 7:cityName=QString::fromUtf8("وادي تليلات");loc.degreeLat = 35.5489;loc.degreeLong = -0.4472;loc.seaLevel = 153;loc.gmtDiff=1;break;
                case 8:cityName=QString::fromUtf8("عين الترك");loc.degreeLat = 35.7433;loc.degreeLong = -0.7583;loc.seaLevel = 32;loc.gmtDiff=1;break;
                }
                break;
            case 31:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("البيض");loc.degreeLat = 33.6935;loc.degreeLong = 1.0153;loc.seaLevel = 1305;loc.gmtDiff=1;break;
                }
                break;
            case 32:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("إليزي");loc.degreeLat = 26.4833;loc.degreeLong = 8.4667;loc.seaLevel = 558;loc.gmtDiff=1;break;
                }
                break;
            case 33:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("برج بوعريريج");loc.degreeLat = 36.0750;loc.degreeLong = 4.7750;loc.seaLevel = 917;loc.gmtDiff=1;break;
                }
                break;
            case 34:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("بمرداس");loc.degreeLat = 	36.7667;loc.degreeLong = 3.4772;loc.seaLevel = 42;loc.gmtDiff=1;break;
                }
                break;
            case 35:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("الطارف");loc.degreeLat = 36.7686;loc.degreeLong = 8.3167;loc.seaLevel = 65;loc.gmtDiff=1;break;
                }
                break;
            case 36:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تندوف");loc.degreeLat = 27.6742;loc.degreeLong = -8.1478;loc.seaLevel = 387;loc.gmtDiff=1;break;
                }
                break;
            case 37:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تسمسيلت");loc.degreeLat = 35.6078;loc.degreeLong = 1.8111;loc.seaLevel = 850;loc.gmtDiff=1;break;
                }
                break;
            case 38:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("الوادي");loc.degreeLat = 33.3333;loc.degreeLong = 6.8833;loc.seaLevel = 68;loc.gmtDiff=1;break;
                }
                break;
            case 39:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("خنشلة");loc.degreeLat = 35.4358;loc.degreeLong = 7.1433;loc.seaLevel = 1153;loc.gmtDiff=1;break;
                }
                break;
            case 40:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("سوق أهراس");loc.degreeLat = 36.2686;loc.degreeLong = 7.9356;loc.seaLevel = 680;loc.gmtDiff=1;break;
                }
                break;
            case 41:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("تبازة");loc.degreeLat = 35.4042;loc.degreeLong = 8.1242;loc.seaLevel = 14;loc.gmtDiff=1;break;
                }
                break;
            case 42:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("ميلة");loc.degreeLat = 36.4503;loc.degreeLong = 6.2644;loc.seaLevel = 487;loc.gmtDiff=1;break;
                }
                break;
            case 43:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("عين الدفلة");loc.degreeLat = 	36.2681;loc.degreeLong = 1.9675;loc.seaLevel = 284;loc.gmtDiff=1;break;
                }
                break;
            case 44:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("نعامة");loc.degreeLat = 33.2667;loc.degreeLong = -0.3167;loc.seaLevel = 1176;loc.gmtDiff=1;break; //ok
                case 1:cityName=QString::fromUtf8("مشرية");loc.degreeLat = 33.5528;loc.degreeLong = -0.2856;loc.seaLevel = 1175;loc.gmtDiff=1;break; //ok
                case 2:cityName=QString::fromUtf8("عين الصفراء");loc.degreeLat = 32.7564;loc.degreeLong = -0.5854;loc.seaLevel = 1081;loc.gmtDiff=1;break; //ok
                case 3:cityName=QString::fromUtf8("عسلة");loc.degreeLat = 33.0000;loc.degreeLong = -0.0761;loc.seaLevel = 0;loc.gmtDiff=1;break;//ok
                case 4:cityName=QString::fromUtf8("تيوت");loc.degreeLat = 32.7711;loc.degreeLong = -0.4150;loc.seaLevel = 993;loc.gmtDiff=1;break; //ok
                case 5:cityName=QString::fromUtf8("مقرار");loc.degreeLat = 32.4931;loc.degreeLong = -0.4681;loc.seaLevel = 894;loc.gmtDiff=1;break; //ok
                case 6:cityName=QString::fromUtf8("مكليس");loc.degreeLat = 32.9897;loc.degreeLong = -0.4944;loc.seaLevel = 1366;loc.gmtDiff=1;break; //ok
                case 7:cityName=QString::fromUtf8("عين بن غليل");loc.degreeLat = 33.2892;loc.degreeLong = -0.7697;loc.seaLevel = 0;loc.gmtDiff=1;break;//ok
                case 8:cityName=QString::fromUtf8("فرطاسة");loc.degreeLat = 	32.8453;loc.degreeLong = -1.2492;loc.seaLevel = 1437;loc.gmtDiff=1;break;//ok
                case 9:cityName=QString::fromUtf8("مكمن بن عمار");loc.degreeLat = 33.7272;loc.degreeLong = -0.7457;loc.seaLevel = 0;loc.gmtDiff=1;break;//ok
                }
                break;
            case 45:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("عين تموشنت");loc.degreeLat = 35.2950;loc.degreeLong = -1.1375;loc.seaLevel = 297;loc.gmtDiff=1;break;
                case 1:cityName=QString::fromUtf8("عين الطلبة");loc.degreeLat = 35.2483;loc.degreeLong = -1.2489;loc.seaLevel = 239;loc.gmtDiff=1;break;
                case 2:cityName=QString::fromUtf8("المالح");loc.degreeLat = 35.3919;loc.degreeLong = -1.0953;loc.seaLevel = 76;loc.gmtDiff=1;break;
                case 3:cityName=QString::fromUtf8("حمام بوحجر");loc.degreeLat = 35.3789;loc.degreeLong = -0.9678;loc.seaLevel = 169;loc.gmtDiff=1;break;
                case 4:cityName=QString::fromUtf8("بني صاف");loc.degreeLat = 35.3039;loc.degreeLong = -1.3898;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 5:cityName=QString::fromUtf8("شعبة اللحم");loc.degreeLat = 35.3400;loc.degreeLong = -1.0980;loc.seaLevel = 144;loc.gmtDiff=1;break;
                case 6:cityName=QString::fromUtf8("سيدي الصافي");loc.degreeLat = 35.2808;loc.degreeLong = -1.3081;loc.seaLevel = 185;loc.gmtDiff=1;break;
                case 7:cityName=QString::fromUtf8("تامازوغة");loc.degreeLat = 35.4106;loc.degreeLong = -0.6561;loc.seaLevel = 184;loc.gmtDiff=1;break;
                case 8:cityName=QString::fromUtf8("وادي الصباح");loc.degreeLat = 35.3721;loc.degreeLong = -0.8113;loc.seaLevel = 0;loc.gmtDiff=1;break;
                case 9:cityName=QString::fromUtf8("اغلال");loc.degreeLat = 35.2039;loc.degreeLong = -1.0639;loc.seaLevel = 491;loc.gmtDiff=1;break;
                case 10:cityName=QString::fromUtf8("عين الكيحل");loc.degreeLat = 35.2047;loc.degreeLong = -1.1975;loc.seaLevel = 434;loc.gmtDiff=1;break;
                case 11:cityName=QString::fromUtf8("حاسى الغلة");loc.degreeLat = 35.4544;loc.degreeLong = -1.0519;loc.seaLevel = 91;loc.gmtDiff=1;break;
                case 12:cityName=QString::fromUtf8("العامرية");loc.degreeLat = 35.5278;loc.degreeLong = -1.0117;loc.seaLevel = 96;loc.gmtDiff=1;break;
                }
                break;
            case 46:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("غرداية");loc.degreeLat = 32.4833;loc.degreeLong = 3.6667;loc.seaLevel = 450;loc.gmtDiff=1;break;
                }
                break;
            case 47:
                switch(index_comboBox_madina_comune)
                {
                case 0:cityName=QString::fromUtf8("غليزان");loc.degreeLat = 35.7425;loc.degreeLong = 0.5592;loc.seaLevel = 99;loc.gmtDiff=1;break; // averifier avec google earth
                }
                break;
            }
            loc.dst = 0;
        }

        if (radioButton_sitting_user->isChecked())
        {
            cityName=lineEdit_name_madina->text();
            loc.degreeLat=lineEdit_degreeLat->text().toDouble();
            loc.degreeLong=lineEdit_degreeLong->text().toDouble();
            loc.seaLevel=lineEdit_seaLevel->text().toDouble();
            loc.gmtDiff=1;
            loc.dst=0;
        }
    }
    //-monde
    if (radioButton_sitting_monde->isChecked())
    {
        cityName=lineEdit_name_madina_monde->text();
        loc.degreeLat=lineEdit_degreeLat_monde->text().toDouble();
        loc.degreeLong=lineEdit_degreeLong_monde->text().toDouble();
        loc.seaLevel=lineEdit_seaLevel_monde->text().toDouble();
        loc.gmtDiff=lineEdit_gmtDiff_monde->text().toDouble();
        if (checkBox_Heure_ete->isChecked())
        {
            loc.dst = 1;
        }
        else
        {
            loc.dst = 0;
        }

        int index_comboBox_Method_monde=comboBox_Method_monde->currentIndex();
        switch(index_comboBox_Method_monde)
        {
        case 0:getMethod(1, &conf);break;
        case 1:getMethod(2, &conf);break;
        case 2:getMethod(3, &conf);break;
        case 3:getMethod(4, &conf);break;
        case 4:getMethod(5, &conf);break;
        case 5:getMethod(6, &conf);break;
        case 6:getMethod(7, &conf);break;
        case 7:getMethod(8, &conf);break;
        }
    }
    /* auto fill the method structure. Have a look at prayer.h for a
     * list of supported methods */
    //	getMethod(5, &conf);
    conf.round = 3;

    //-- HAF 20-6-2009 --Correction-ajout 1 mn pour tous les prieres--
    // cf http://www.islamicfinder.org/  noter que pour makka et dans ramadhan +isha 30mn
    conf.offset = 1; //zero pour desactiver
    int i;

    for (i = 0; i < 6 ; i++)
    {
        if (i!=1)
        {
            conf.offList[i]+=1; //+1mn
            if (radioButton_sitting_Algerie->isChecked())
            {
                if (i==4)
                {
                   // ptList[i].minute+=4; //ajout pour meghreb de 1+(4mn pour algerie seulement) //correction Bug 21-5-2022 00:00
                    conf.offList[i]+=4;
                }
            }
            if (ptList[i].minute==60)
            {
                ptList[i].minute=0;
                ptList[i].hour+=1;
            }
        }
    }

    nextFajr.minute+=1; //+1mn
    if (nextFajr.minute==60)
    {
        nextFajr.minute=0;
        nextFajr.hour+=1;
    }

    //-----------------------------------------
    loc.pressure = 1010;
    loc.temperature= 10;



    /* Call the main function to fill the Prayer times array of
     * structures */
    getPrayerTimes (&loc, &conf, &date, ptList);

    /* Call functions for other prayer times and qibla */
    getImsaak (&loc, &conf, &date, &imsaak);
    getNextDayFajr (&loc, &conf, &date, &nextFajr);
    getNextDayImsaak (&loc, &conf, &date, &nextImsaak);
    qibla = getNorthQibla(&loc);


    //--------------- HAF 5-6-2009 -----------------------
    QString outputtex;

    QString str1 = QString::fromUtf8("مدينة : %1")
            .arg(cityName);

    outputtex.append(str1);

    decimal2Dms (loc.degreeLat, &deg, &min, &sec);
    QString str2 = QString::fromUtf8("\n خط العرض :  %1 ° %2 ' %3 ''")
            .arg(abs (deg))
            .arg(abs (min))
            .arg(int(fabs (sec)));

    if (loc.degreeLat>=0)
    {
        str2.append(QString::fromUtf8(" شمال"));
    }
    else
    {
        str2.append(QString::fromUtf8(" جنوب"));
    }


    outputtex.append(str2);

    decimal2Dms (loc.degreeLong, &deg, &min, &sec);
    QString str3 = QString::fromUtf8("\n خط الطول : %1 ° %2 ' %3 ''")
            .arg(abs (deg))
            .arg(abs (min))
            .arg(int(fabs (sec)));

    if (loc.degreeLong>=0)
    {
        str3.append(QString::fromUtf8(" شرق"));
    }
    else
    {
        str3.append(QString::fromUtf8(" غرب"));
    }

    outputtex.append(str3);

    QString str3bis = QString::fromUtf8("إرتفاع على مستوى البحر : %1 م ")
            .arg(loc.seaLevel);

    decimal2Dms (qibla, &deg, &min, &sec);
    //--Bug dans prayer ...Mekka...
    bool mekka=false;
    if (loc.degreeLat==21.4233 && loc.degreeLong==39.8233 )
    {
        deg=0;
        min=0;
        sec=0;
        mekka=true;
    }
    //-----------------------------
    QString str4 = QString::fromUtf8("\n إتجاه القبلة :  %1 ° %2 ' %3 ''")
            .arg(abs (deg))
            .arg(abs (min))
            .arg(int(fabs (sec)));

    if (qibla==0 || mekka==true)
    {
        str4.append(QString::fromUtf8(""));
    }
    if (qibla>0 && mekka==false)
    {
        str4.append(QString::fromUtf8(" غربا  بنسبة لشمال الحقيقي "));
    }
    if (qibla<0 && mekka==false)
    {
        str4.append(QString::fromUtf8(" شرقا  بنسبة لشمال الحقيقي "));
    }

    outputtex.append(str4);

    //---HAF DIA QUIBLA 15-6-2009-----
    dial_Direction_Quibla->setValue(int(180-(deg+min/60+sec/3600)));
    ///---
    label_madina_name->setText(str1);
    label_madina_long->setText(str2);
    label_madina_latitu->setText(str3);
    label_elevation->setText(str3bis);
    label_quibla_info->setText(str4);
    //--------------------------------


    //----------------
    /* current time */
    int tm_hour=dateTimeEdit->time().hour();
    int tm_min=dateTimeEdit->time().minute();
    int tm_sec=dateTimeEdit->time().second();


    //	int i;
    int next_prayer_id = -1;
    for (i = 0; i < 6; i++)
    {
        if ( i == 1 ) { continue ;} /* skip shorouk */
        if(ptList[i].hour > tm_hour ||
                (ptList[i].hour == tm_hour &&
                 ptList[i].minute >= tm_min))
        {
            next_prayer_id = i;break;
        }
    }

    if (next_prayer_id==-1)
    {
        label_nextsalat->setText(QString::fromUtf8("فجر الغد"));
        label_temps_restant->setText(QString::fromUtf8("الوقت المتبقي حتى صلاة فجر الغد"));
        label_oldsalat->setText(QString::fromUtf8("العشاء"));
    }
    if (next_prayer_id==0)
    {
        label_nextsalat->setText(QString::fromUtf8("فجر اليوم"));
        label_temps_restant->setText(QString::fromUtf8("الوقت المتبقي حتى صلاة الفجر"));
        label_oldsalat->setText(QString::fromUtf8("عشاء الأمس"));
    }
    if (next_prayer_id==2)
    {
        label_nextsalat->setText(QString::fromUtf8("الظهر"));
        label_temps_restant->setText(QString::fromUtf8("الوقت المتبقي حتى صلاة الظهر"));
        label_oldsalat->setText(QString::fromUtf8("الفجر"));
    }
    if (next_prayer_id==3)
    {
        label_nextsalat->setText(QString::fromUtf8("العصر"));
        label_temps_restant->setText(QString::fromUtf8("الوقت المتبقي حتى صلاة العصر"));
        label_oldsalat->setText(QString::fromUtf8("الظهر"));
    }
    if (next_prayer_id==4)
    {
        label_nextsalat->setText(QString::fromUtf8("المغرب"));
        label_temps_restant->setText(QString::fromUtf8("الوقت المتبقي حتى صلاة المغرب"));
        label_oldsalat->setText(QString::fromUtf8("العصر"));
    }
    if (next_prayer_id==5)
    {
        label_nextsalat->setText(QString::fromUtf8("العشاء"));
        label_temps_restant->setText(QString::fromUtf8("الوقت المتبقي حتى صلاة العشاء"));
        label_oldsalat->setText(QString::fromUtf8("المغرب"));
    }

    //----- to do haf 12-6-2009------------------
    //	-voir tray (demo et exemples fornies avec qt4.5.1
    //	-Athan
    //-------------------------------------------

    timeEdit_fadjra->setTime(QTime(ptList[0].hour, ptList[0].minute, 0));
    timeEdit_chork->setTime(QTime(ptList[1].hour, ptList[1].minute, 0));
    timeEdit_thohor->setTime(QTime(ptList[2].hour, ptList[2].minute, 0));
    timeEdit_asar->setTime(QTime(ptList[3].hour, ptList[3].minute, 0));
    timeEdit_maghrab->setTime(QTime(ptList[4].hour, ptList[4].minute, 0));
    timeEdit_icha->setTime(QTime(ptList[5].hour, ptList[5].minute, 0));
    timeEdit_imsak->setTime(QTime(imsaak.hour, imsaak.minute, 0));
    timeEdit_imsak_tomorow->setTime(QTime(nextImsaak.hour, nextImsaak.minute, 0));
    timeEdit_fadjra_tomorow->setTime(QTime(nextFajr.hour, nextFajr.minute, 0));
    //	tabWidget_timesprayer->setCurrentIndex(1);


    //---------12-6-2009--------------
    /* converts times to minutes */
    int next_minutes = ptList[next_prayer_id].minute + ptList[next_prayer_id].hour*60;
    int next_old = ptList[next_prayer_id-1].minute + ptList[next_prayer_id-1].hour*60;
    int next_secondes=60 - tm_sec;


    int cur_minutes = tm_min + tm_hour * 60;
    if(ptList[next_prayer_id].hour < tm_hour)
    {
        /* salat is on next day (subh, and even Isha sometimes) after midnight */
        next_minutes += 60*24+12;  //+12 avoir
    }
    //qDebug() << "tm_hour:" << tm_hour;
    //qDebug() << "next_minutes 2:" << next_minutes;

    //// qDebug() << "cur_minutes:" << cur_minutes;
    int difference = next_minutes - cur_minutes;

    if (next_prayer_id==-1)
    {
        difference=24*60-cur_minutes+ptList[0].minute + ptList[0].hour*60;
    }
    //qDebug() << "difference 2:" << difference;

    int hours = difference / 60;
    int minutes = difference % 60-1; //-1 avoir

    if (difference == 0)
    {
        timeEdit_pour_attendre_nextsalat->setTime(QTime(0,0,0));
    }
    else if (difference < 60 )
    {
        timeEdit_pour_attendre_nextsalat->setTime(QTime(0,minutes,next_secondes));
    }
    else if (difference % 60 == 0)
    {
        timeEdit_pour_attendre_nextsalat->setTime(QTime(hours,0,next_secondes));
    }
    else
    {
        timeEdit_pour_attendre_nextsalat->setTime(QTime(hours,minutes,next_secondes));
    }

    //------------12-6-2009-------------------
    int diffminuterestente=hours*60+minutes;
    int diffminuteentreoldandnext;
    if(ptList[next_prayer_id].hour < tm_hour || next_prayer_id== 0)
    {

        diffminuteentreoldandnext=(nextFajr.hour*60+nextFajr.minute)+(24*60-(ptList[5].hour*60+ptList[5].minute));

    }
    else
    {
        diffminuteentreoldandnext=next_minutes-next_old;
    }
    int pourcentminut=int((diffminuterestente*100)/diffminuteentreoldandnext);

    int pourcentminutExacte=100-pourcentminut;

    if (pourcentminutExacte>100)
    {
        pourcentminutExacte=100;
    }
    if (pourcentminutExacte<5)
    {
        pourcentminutExacte=5;
    }
    progressBar_temps_restant->setValue(pourcentminutExacte);
    //----------------------------------------
    if (radioButton_sitting_HAF->isChecked() && radioButton_sitting_Algerie->isChecked())
    {
        QString strEnTete = QString::fromUtf8("أوقات الصلاة ل : %1 ")
                .arg(cityName);
        strEnTete.append(QString::fromUtf8("ولاية %2")
                         .arg(comboBox_madina->currentText()));
        label_entete->setText(strEnTete);
    }
    else
    {
        QString strEnTete = QString::fromUtf8("جدول أوقات الصلاة ل : %1")
                .arg(cityName);
        label_entete->setText(strEnTete);
    }
    //----------------------------------------

    //---- HAF le 22-6-2009  ATHAN -----------
    QTime TempsRestanthour = timeEdit_pour_attendre_nextsalat->time();
    QString TempsRestanthourString=TempsRestanthour.toString();
    QString next_prayer_text;
    if (next_prayer_id==-1)
    {
        next_prayer_text=QString::fromUtf8("فجر الغد");
    }
    if (next_prayer_id==0)
    {
        next_prayer_text=QString::fromUtf8("الفجر");
    }
    if (next_prayer_id==2)
    {
        next_prayer_text=QString::fromUtf8("الظهر");
    }
    if (next_prayer_id==3)
    {
        next_prayer_text=QString::fromUtf8("العصر");
    }
    if (next_prayer_id==4)
    {
        next_prayer_text=QString::fromUtf8("المغرب");
    }
    if (next_prayer_id==5)
    {
        next_prayer_text=QString::fromUtf8("العشاء");
    }
    int temps_actuel_athan;
    // 29-8-2009

    msgBox.setText(QString::fromUtf8("حان الوقت لصلاة : %1")
                   .arg(next_prayer_text));
    msgBox.setWindowTitle(QString::fromUtf8("الأذان"));
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
    msgBox.setModal(true);
    msgBox.setLayoutDirection(Qt::LeftToRight);
    if (m_radioButton_sansATHAN_isChecked==false)
    {
        msgBox.setButtonText(QMessageBox::Ok, QString::fromUtf8("نعم"));
    }else{
        msgBox.setButtonText(QMessageBox::Ok, QString::fromUtf8("إيقاف الأذان"));
    }
    msgBox.setBaseSize(125,31);
    // QPixmap pixmap = QPixmap (":/icons/thakir_prayer_times.ico");
    //msgBox.setIconPixmap(pixmap);
    msgBox.setIcon(QMessageBox::Warning);

    if(difference == 0 && !athanwasplayed)
        //if (hours==0 && minutes==0 && next_secondes==0)
    {
        athanwasplayed=true;
        //showNormal();
        if (m_radioButton_sansATHAN_isChecked==false) playAthan(next_prayer_text,athan);
        //msgBox.show();
        //msgBox.exec();
        //-- bug :29-8-2009 l'application se ferme si on clik sur "fermer athan" apres que athan est terminé
        if (msgBox.exec() == QMessageBox::Ok)
        {
            showNormal();
            athan->stop();
            hide();

        }
        //---
        temps_actuel_athan=cur_minutes;

    }
    //--bug :29-8-2009 a voir si marche avec QT 4.6 peut etre un bug dans 4.5.2
    //    if (athan->isFinished())
    //    {
    //     msgBox.close();
    //    }
    //--

    if (radioButton_Alerte_Avant->isChecked() && alerteisdone==false)
    {
        int tempsAlerteAvant=spinBox_Temps_Alerte_Avant->value();
        if (minutes==tempsAlerteAvant && hours==0)
        {
            #ifdef Q_OS_WIN
            // if(minutes+1!=0){
            sticon->showMessage(QString::fromUtf8("إنتبه من فضلك"),QString::fromUtf8("تبقى لصلاة %1 اقل من %2 دقيقة")
                                .arg(next_prayer_text)
                                .arg(minutes),QSystemTrayIcon::Warning,10000); // On affiche une infobulle
            // }
            alerteisdone=true;
            #elif defined(Q_OS_LINUX)
            ///---------------HAF FOR FLATPAK 25-8-2025-----//
            sendNotification(
                QString::fromUtf8("إنتبه من فضلك"),QString::fromUtf8("تبقى لصلاة %1 اقل من %2 دقيقة")
                    .arg(next_prayer_text)
                    .arg(minutes),"dialog-information"
                );
            alerteisdone=true;
            #endif
            /// --------------------------------------------//
        }
    }
    QTime TempsRestanthour2 = timeEdit_pour_attendre_nextsalat->time();
    int TempsRestanthourminite=TempsRestanthour2.minute();
    QString TempsRestanthourminiteDisplay;
    if (TempsRestanthourminite<10){
        TempsRestanthourminiteDisplay= "0"+QString::fromUtf8("%1")
                .arg(TempsRestanthourminite);
    }else{
        TempsRestanthourminiteDisplay= QString::fromUtf8("%1")
                .arg(TempsRestanthourminite);
    }
    int TempsRestanthourhour=TempsRestanthour2.hour();
    QString TempsRestanthourhourDisplay;
    if (TempsRestanthourhour<10){
        TempsRestanthourhourDisplay= "0"+QString::fromUtf8("%1")
                .arg(TempsRestanthourhour);
    }else{
        TempsRestanthourhourDisplay= QString::fromUtf8("%1")
                .arg(TempsRestanthourhour);
    }

    QString TempstoDisplay= TempsRestanthourhourDisplay+":"+TempsRestanthourminiteDisplay;

    sticon->setToolTip(QString::fromUtf8(" الوقت المتبقي حتى %1 %2")
                       .arg(next_prayer_text)
                       .arg(TempstoDisplay));


    //----HAF le 17-7-2009------
    //	if (radioButton_sitting_Algerie->isChecked())
    //	{
    //        timeEdit_thohor->setToolTip(QString::fromUtf8("وقت الرسمي لأذان الظهر في الجزائر هو 13:20"));
    //	}
    //	else
    //	{
    //	timeEdit_thohor->setToolTip("");
    //	}
    //--------------------------
}
void thakir_prayer_times::thakir_prayer_times_Activer_toolBox1()
{
    toolBox->setCurrentIndex(0);
    radioButton_sitting_monde->setChecked(false);
}

void thakir_prayer_times::thakir_prayer_times_Activer_toolBox2()
{
    toolBox->setCurrentIndex(1);
    radioButton_sitting_Algerie->setChecked(false);
}
void thakir_prayer_times::radioButton_sitting_user_setNotChecked()
{
    if (radioButton_sitting_HAF->isChecked())
    {
        radioButton_sitting_user->setChecked(false);
    }
    else
    {
        radioButton_sitting_user->setChecked(true);
    }

}

void thakir_prayer_times::radioButton_sitting_HAF_setNotChecked()
{
    if (radioButton_sitting_user->isChecked())
    {
        radioButton_sitting_HAF->setChecked(false);
    }
    else
    {
        radioButton_sitting_HAF->setChecked(true);
    }
}

void thakir_prayer_times::thakir_prayer_times_config()
{
    m_name_city_DZ=QString::fromUtf8("سعيدة");
    m_latitude_DZ = +34.8304;
    m_longitude_DZ = +0.1518;
    m_local_elevation_DZ = 0;

    m_name_city_Monde=QString::fromUtf8("مكة المكرمة");
    m_latitude_Monde = +21.423333;
    m_longitude_Monde = +39.823333;
    m_local_elevation_Monde = 0;
    m_time_zone_Monde=+3;

    m_itl_method_Monde =5;
    m_name_city_preconfig_DZ=19; //Saida
    m_name_city_commune_preconfig_DZ=0; //Saida

    m_radioButton_sitting_Algerie_isChecked = true;
    m_radioButton_sitting_HAF_isChecked = true;

    m_checkBox_Heure_ete_isChecked=false;

    m_radioButton_sansATHAN_isChecked=false;
    m_radioButton_Athan_Makka_isChecked=false;
    m_radioButton_Athan_Maddina_isChecked=true;
    m_radioButton_Athan_Aquassa_isChecked=false;
    m_radioButton_Athan_Algeria_isChecked=false;
    m_radioButton_sansALERTE_isChecked=false;
    m_radioButton_Alerte_Avant_isChecked=true;
    m_spinBox_Temps_Alerte_Avant=5;

    m_checkBox_autostart_isChecked = true;
}

void thakir_prayer_times::readSettings()
{
    m_name_city_preconfig_DZ = HAF_settings->value("Algerie/city_preconfig/name").toInt();
    m_name_city_commune_preconfig_DZ= HAF_settings->value("Algerie/city_preconfig/name_commune").toInt();

    m_name_city_DZ = HAF_settings->value("Algerie/city/name").toString();
    m_latitude_DZ = HAF_settings->value("Algerie/location/latitude").toDouble();
    m_longitude_DZ = HAF_settings->value("Algerie/location/longitude").toDouble();
    m_local_elevation_DZ = HAF_settings->value("Algerie/location/elevation").toDouble();

    m_name_city_Monde = HAF_settings->value("Monde/city/name").toString();
    m_latitude_Monde = HAF_settings->value("Monde/location/latitude").toDouble();
    m_longitude_Monde = HAF_settings->value("Monde/location/longitude").toDouble();
    m_local_elevation_Monde = HAF_settings->value("Monde/location/elevation").toDouble();
    m_time_zone_Monde = HAF_settings->value("Monde/location/time_zone").toDouble();

    m_itl_method_Monde = HAF_settings->value("Monde/methode/methode").toInt();

    m_radioButton_sitting_Algerie_isChecked  = HAF_settings->value("Algerie/Activation/sitting_Algerie").toBool();
    m_radioButton_sitting_HAF_isChecked  = HAF_settings->value("Algerie/Activation/sitting_HAF").toBool();

    m_checkBox_Heure_ete_isChecked = HAF_settings->value("Monde/Activation/Heure_Ete").toBool();

    m_radioButton_sansATHAN_isChecked=HAF_settings->value("AthanAlerte/Activation/sansATHAN").toBool();
    m_radioButton_Athan_Makka_isChecked=HAF_settings->value("AthanAlerte/Activation/Athan_Makka").toBool();
    m_radioButton_Athan_Maddina_isChecked=HAF_settings->value("AthanAlerte/Activation/Athan_Maddina").toBool();
    m_radioButton_Athan_Aquassa_isChecked=HAF_settings->value("AthanAlerte/Activation/Athan_Aquassa").toBool();
    m_radioButton_Athan_Algeria_isChecked=HAF_settings->value("AthanAlerte/Activation/Athan_Algeria").toBool();
    m_radioButton_sansALERTE_isChecked=HAF_settings->value("AthanAlerte/Activation/sansALERTE").toBool();
    m_radioButton_Alerte_Avant_isChecked=HAF_settings->value("AthanAlerte/Activation/Alerte_Avant").toBool();
    m_spinBox_Temps_Alerte_Avant=HAF_settings->value("AthanAlerte/TimeAlerte/valeur").toInt();

    m_checkBox_autostart_isChecked = HAF_settings->value("setting_autostart").toBool();

}

void thakir_prayer_times::writeSettings()
{
    HAF_settings->setValue("Algerie/city_preconfig/name", m_name_city_preconfig_DZ);
    HAF_settings->setValue("Algerie/city_preconfig/name_commune", m_name_city_commune_preconfig_DZ);

    HAF_settings->setValue("Algerie/city/name", m_name_city_DZ);
    HAF_settings->setValue("Algerie/location/latitude", m_latitude_DZ);
    HAF_settings->setValue("Algerie/location/longitude", m_longitude_DZ);
    HAF_settings->setValue("Algerie/location/elevation", m_local_elevation_DZ);

    HAF_settings->setValue("Monde/city/name", m_name_city_Monde);
    HAF_settings->setValue("Monde/location/latitude", m_latitude_Monde);
    HAF_settings->setValue("Monde/location/longitude", m_longitude_Monde);
    HAF_settings->setValue("Monde/location/elevation", m_local_elevation_Monde);
    HAF_settings->setValue("Monde/location/time_zone", m_time_zone_Monde);

    HAF_settings->setValue("Monde/methode/methode", m_itl_method_Monde);

    HAF_settings->setValue("Algerie/Activation/sitting_Algerie", m_radioButton_sitting_Algerie_isChecked);
    HAF_settings->setValue("Algerie/Activation/sitting_HAF", m_radioButton_sitting_HAF_isChecked);

    HAF_settings->setValue("Monde/Activation/Heure_Ete", m_checkBox_Heure_ete_isChecked);

    HAF_settings->setValue("AthanAlerte/Activation/sansATHAN", m_radioButton_sansATHAN_isChecked);
    HAF_settings->setValue("AthanAlerte/Activation/Athan_Makka", m_radioButton_Athan_Makka_isChecked);
    HAF_settings->setValue("AthanAlerte/Activation/Athan_Maddina", m_radioButton_Athan_Maddina_isChecked);
    HAF_settings->setValue("AthanAlerte/Activation/Athan_Aquassa", m_radioButton_Athan_Aquassa_isChecked);
    HAF_settings->setValue("AthanAlerte/Activation/Athan_Algeria", m_radioButton_Athan_Algeria_isChecked);
    HAF_settings->setValue("AthanAlerte/Activation/sansALERTE", m_radioButton_sansALERTE_isChecked);
    HAF_settings->setValue("AthanAlerte/Activation/Alerte_Avant", m_radioButton_Alerte_Avant_isChecked);
    HAF_settings->setValue("AthanAlerte/TimeAlerte/valeur", m_spinBox_Temps_Alerte_Avant);

    HAF_settings->setValue("setting_autostart", m_checkBox_autostart_isChecked);

    HAF_settings->sync();
}


void thakir_prayer_times::updateSettings()
{
    QString TEMP0 = QString::fromUtf8("%1")
            .arg(m_name_city_DZ);
    lineEdit_name_madina->setText(TEMP0);
    QString TEMP1 = QString::fromUtf8("%1")
            .arg(m_latitude_DZ);
    lineEdit_degreeLat->setText(TEMP1);
    QString TEMP2 = QString::fromUtf8("%1")
            .arg(m_longitude_DZ);
    lineEdit_degreeLong->setText(TEMP2);
    QString TEMP3 = QString::fromUtf8("%1")
            .arg(m_local_elevation_DZ);
    lineEdit_seaLevel->setText(TEMP3);

    QString TEMP5 = QString::fromUtf8("%1")
            .arg(m_name_city_Monde);
    lineEdit_name_madina_monde->setText(TEMP5);
    QString TEMP6 = QString::fromUtf8("%1")
            .arg(m_latitude_Monde);
    lineEdit_degreeLat_monde->setText(TEMP6);
    QString TEMP7 = QString::fromUtf8("%1")
            .arg(m_longitude_Monde);
    lineEdit_degreeLong_monde->setText(TEMP7);
    QString TEMP8 = QString::fromUtf8("%1")
            .arg(m_local_elevation_Monde);
    lineEdit_seaLevel_monde->setText(TEMP8);
    QString TEMP9 = QString::fromUtf8("%1")
            .arg(m_time_zone_Monde);
    lineEdit_gmtDiff_monde->setText(TEMP9);

    comboBox_Method_monde->setCurrentIndex(m_itl_method_Monde);
    comboBox_madina->setCurrentIndex(m_name_city_preconfig_DZ);
    comboBox_madina_comune->setCurrentIndex(m_name_city_commune_preconfig_DZ);

    radioButton_sitting_HAF->setChecked(m_radioButton_sitting_HAF_isChecked);
    radioButton_sitting_Algerie->setChecked(m_radioButton_sitting_Algerie_isChecked);
    radioButton_sitting_monde->setChecked(not(m_radioButton_sitting_Algerie_isChecked));
    radioButton_sitting_user->setChecked(not(m_radioButton_sitting_HAF_isChecked));

    if (radioButton_sitting_Algerie->isChecked())
    {
        thakir_prayer_times_Activer_toolBox1();
    }
    else
    {
        thakir_prayer_times_Activer_toolBox2();
    }

    checkBox_Heure_ete->setChecked(m_checkBox_Heure_ete_isChecked);

    radioButton_sansATHAN->setChecked(m_radioButton_sansATHAN_isChecked);
    radioButton_Athan_Makka->setChecked(m_radioButton_Athan_Makka_isChecked);
    radioButton_Athan_Maddina->setChecked(m_radioButton_Athan_Maddina_isChecked);
    radioButton_Athan_Aquassa->setChecked(m_radioButton_Athan_Aquassa_isChecked);
    radioButton_Athan_Algeria->setChecked(m_radioButton_Athan_Algeria_isChecked);
    radioButton_sansALERTE->setChecked(m_radioButton_sansALERTE_isChecked);
    radioButton_Alerte_Avant->setChecked(m_radioButton_Alerte_Avant_isChecked);
    spinBox_Temps_Alerte_Avant->setValue(m_spinBox_Temps_Alerte_Avant);

    checkBox_autostart->setChecked(m_checkBox_autostart_isChecked);
}

void thakir_prayer_times::closeEvent(QCloseEvent *event)
{
    writeSettings();

    hide();
    event->ignore();

}

void thakir_prayer_times::readDataFromUI()
{
    m_name_city_DZ=lineEdit_name_madina->text();
    m_latitude_DZ=lineEdit_degreeLat->text().toDouble();
    m_longitude_DZ=lineEdit_degreeLong->text().toDouble();
    m_local_elevation_DZ=lineEdit_seaLevel->text().toDouble();

    m_name_city_Monde=lineEdit_name_madina_monde->text();
    m_latitude_Monde=lineEdit_degreeLat_monde->text().toDouble();
    m_longitude_Monde=lineEdit_degreeLong_monde->text().toDouble();
    m_local_elevation_Monde=lineEdit_seaLevel_monde->text().toDouble();
    m_time_zone_Monde=lineEdit_gmtDiff_monde->text().toDouble();

    m_itl_method_Monde=comboBox_Method_monde->currentIndex();
    m_name_city_preconfig_DZ=comboBox_madina->currentIndex();
    m_name_city_commune_preconfig_DZ=comboBox_madina_comune->currentIndex();

    m_radioButton_sitting_HAF_isChecked=radioButton_sitting_HAF->isChecked();
    m_radioButton_sitting_Algerie_isChecked=radioButton_sitting_Algerie->isChecked();

    m_checkBox_Heure_ete_isChecked=checkBox_Heure_ete->isChecked();

    m_radioButton_sansATHAN_isChecked=radioButton_sansATHAN->isChecked();
    m_radioButton_Athan_Makka_isChecked=radioButton_Athan_Makka->isChecked();
    m_radioButton_Athan_Maddina_isChecked=radioButton_Athan_Maddina->isChecked();
    m_radioButton_Athan_Aquassa_isChecked=radioButton_Athan_Aquassa->isChecked();
    m_radioButton_Athan_Algeria_isChecked=radioButton_Athan_Algeria->isChecked();
    m_radioButton_sansALERTE_isChecked=radioButton_sansALERTE->isChecked();
    m_radioButton_Alerte_Avant_isChecked=radioButton_Alerte_Avant->isChecked();
    m_spinBox_Temps_Alerte_Avant=spinBox_Temps_Alerte_Avant->value();

    m_checkBox_autostart_isChecked=checkBox_autostart->isChecked();

}

void thakir_prayer_times::playAthan(QString next_prayer_text,QMediaPlayer* athan)
{
    timeEdit_pour_attendre_nextsalat->setTime(QTime(0,0,0));
    if (m_radioButton_sansATHAN_isChecked==true)
    {
        msgBox.setButtonText(QMessageBox::Ok, QString::fromUtf8("نعم"));
    }else{
        msgBox.setButtonText(QMessageBox::Ok, QString::fromUtf8("إيقاف الأذان"));
    }
    //   athan->setLoops(1);
    athan->play();

    alerteisdone=false;
}

void thakir_prayer_times::setVisible(bool visible)
{
    actTexte1->setEnabled(visible);
    actTexte2->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
    if(visible){
        timer->start(1000);
    }else{
        timer->start(60000);
    }
}
void thakir_prayer_times::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if (QMainWindow::isVisible())
        {
            dlgAbout.close();
            hide();
        }
        else
        {
            showNormal();
        }
        break;
    case QSystemTrayIcon::MiddleClick:
        if (QMainWindow::isVisible())
        {
            dlgAbout.close();
            hide();
        }
        else
        {
            showNormal();
        }
        break;
    default:
        ;
    }
}

//------------- HAF 19-6-2009 ------
void thakir_prayer_times::COMMUN_Update(int i)
{
    comboBox_madina_comune->clear();
    //	cout<<i;
    if (i==0)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("أدرار")
                                            << QString::fromUtf8("برج باجي مختار")
                                            << QString::fromUtf8("تميمون")
                                            << QString::fromUtf8("رقان")
                                            );
    }
    if (i==1)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("شلف")
                                            );
    }
    if (i==2)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("لغواط")
                                            << QString::fromUtf8("حاسي الرمل")
                                            );
    }
    if (i==3)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("أم البواقي")
                                            );
    }
    if (i==4)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("باتنة")
                                            );
    }
    if (i==5)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("بجاية")
                                            );
    }
    if (i==6)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("بسكرة")
                                            );
    }
    if (i==7)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("بشار")
                                            );
    }
    if (i==8)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("بليدة")
                                            );
    }
    if (i==9)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("بويرة")
                                            );
    }
    if (i==10)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تمراسات")
                                            );
    }
    if (i==11)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تبسة")
                                            );
    }
    if (i==12)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تلمسان")
                                            << QString::fromUtf8("الغزوات")
                                            << QString::fromUtf8("صبرة")
                                            << QString::fromUtf8("ندرومة")
                                            << QString::fromUtf8("الرمشي")
                                            << QString::fromUtf8("سبدو")
                                            << QString::fromUtf8("باب العسة")
                                            << QString::fromUtf8("المنصورة")
                                            << QString::fromUtf8("اولاد ميمون")
                                            << QString::fromUtf8("سيدي الجيلالي")
                                            << QString::fromUtf8("شتوان")
                                            << QString::fromUtf8("هنين")
                                            << QString::fromUtf8("بني سنوس")
                                            << QString::fromUtf8("سيدي مجاهد")
                                            << QString::fromUtf8("مرسى بن مهيدي")
                                            << QString::fromUtf8("عين تالوت")
                                            << QString::fromUtf8("سيدي العبدلي")
                                            << QString::fromUtf8("الحناية")
                                            << QString::fromUtf8("مغنية")
                                            );
    }
    if (i==13)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تيارت")
                                            );
    }
    if (i==14)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تيزي وزو")
                                            );
    }
    if (i==15)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("الجزائر")
                                            << QString::fromUtf8("زرالدة")
                                            << QString::fromUtf8("الشراقة")
                                            << QString::fromUtf8("الدرارية")
                                            << QString::fromUtf8("بئر توتة")
                                            << QString::fromUtf8("بئر مراد رايس")
                                            << QString::fromUtf8("بوزريعة")
                                            << QString::fromUtf8("باب الواد")
                                            << QString::fromUtf8("حسين داي")
                                            << QString::fromUtf8("سيدي امحمد")
                                            << QString::fromUtf8("الحراش")
                                            << QString::fromUtf8("براقي")
                                            << QString::fromUtf8("الدار البيضاء")
                                            << QString::fromUtf8("الرويبة")
                                            );
    }
    if (i==16)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("جلفة")
                                            );
    }
    if (i==17)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("جيجل")
                                            );
    }
    if (i==18)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("سطيف")
                                            );
    }
    if (i==19)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("سعيدة")
                                            << QString::fromUtf8("عين الحجر")
                                            << QString::fromUtf8("داود")
                                            << QString::fromUtf8("يوب")
                                            << QString::fromUtf8("سدي بوبكر")
                                            << QString::fromUtf8("سدي عمر")
                                            << QString::fromUtf8("سدي أحمد")
                                            << QString::fromUtf8("عين السخونة")
                                            << QString::fromUtf8("مولاي العربي")
                                            );
    }
    if (i==20)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("سكيكدة")
                                            );
    }
    if (i==21)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("سيدي بلعباس")
                                            );
    }
    if (i==22)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("عنابة")
                                            );
    }
    if (i==23)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("قالمة")
                                            );
    }
    if (i==24)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("قسنطينة")
                                            );
    }

    if (i==25)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("مدية")
                                            );
    }

    if (i==26)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("مستغانم")
                                            );
    }

    if (i==27)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("أمسيلة")
                                            );
    }

    if (i==28)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("معسكر")
                                            << QString::fromUtf8("بوحنفية")
                                            << QString::fromUtf8("شورفة")
                                            << QString::fromUtf8("غريس")
                                            << QString::fromUtf8("محمدية")
                                            << QString::fromUtf8("سيق")
                                            << QString::fromUtf8("تغنيف")
                                            << QString::fromUtf8("زهانة")
                                            );
    }

    if (i==29)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("ورقلة")
                                            );
    }

    if (i==30)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("وهران")
                                            << QString::fromUtf8("أرزيو")
                                            << QString::fromUtf8("بوتليليس")
                                            << QString::fromUtf8("قديل")
                                            << QString::fromUtf8("بئر الجير")
                                            << QString::fromUtf8("بطيوة")
                                            << QString::fromUtf8("السانية")
                                            << QString::fromUtf8("وادي تليلات")
                                            << QString::fromUtf8("عين الترك")
                                            );
    }

    if (i==31)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("البيض")
                                            );
    }

    if (i==32)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("إليزي")
                                            );
    }

    if (i==33)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("برج بوعريريج")
                                            );
    }

    if (i==34)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("بمرداس")
                                            );
    }

    if (i==35)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("الطارف")
                                            );
    }

    if (i==36)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تندوف")
                                            );
    }

    if (i==37)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تسمسيلت")
                                            );
    }

    if (i==38)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("الوادي")
                                            );
    }

    if (i==39)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("خنشلة")
                                            );
    }

    if (i==40)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("سوق أهراس")
                                            );
    }

    if (i==41)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("تبازة")
                                            );
    }

    if (i==42)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("ميلة")
                                            );
    }

    if (i==43)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("عين الدفلة")
                                            );
    }

    if (i==44)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("نعامة")
                                            << QString::fromUtf8("مشرية")
                                            << QString::fromUtf8("عين الصفراء")
                                            << QString::fromUtf8("عسلة")
                                            << QString::fromUtf8("تيوت")
                                            << QString::fromUtf8("مقرار")
                                            << QString::fromUtf8("مكليس")
                                            << QString::fromUtf8("عين بن غليل")
                                            << QString::fromUtf8("فرطاسة")
                                            << QString::fromUtf8("مكمن بن عمار")
                                            );
    }
    if (i==45)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("عين تموشنت")
                                            << QString::fromUtf8("عين الطلبة")
                                            << QString::fromUtf8("المالح")
                                            << QString::fromUtf8("حمام بوحجر")
                                            << QString::fromUtf8("بني صاف")
                                            << QString::fromUtf8("شعبة اللحم")
                                            << QString::fromUtf8("سيدي الصافي")
                                            << QString::fromUtf8("تامازوغة")
                                            << QString::fromUtf8("وادي الصباح")
                                            << QString::fromUtf8("اغلال")
                                            << QString::fromUtf8("عين الكيحل")
                                            << QString::fromUtf8("حاسى الغلة")
                                            << QString::fromUtf8("العامرية")
                                            );
    }
    if (i==46)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("غرداية")
                                            );
    }
    if (i==47)
    {
        comboBox_madina_comune->insertItems(0,QStringList()
                                            << QString::fromUtf8("غليزان"));
    }
}

//void thakir_prayer_times::about()
//{
//       QMessageBox msgabout;

//       msgabout.setText(QString::fromUtf8("<p align=\"center\""

//      "<h1><b>ذكر لأوقات الصلاة، إصدار 1.035</b></h1><br /><br />"

//	"<font color=\"#347235\"><b>   هذا البرنامج من تصميم : حفيان محمد </font> <br /><br />"
//
//       "&lt;<a href='mailto:thakir.dz@gmail.com'>thakir.dz@gmail.com</a>&gt;<br />"

//      "<div align=justify>"
//      "<UL>"
//      "<LI> هذا البرنامج يوزع مجانا في أمل أن يكون نافعا بإذن الله، لكنه غير مصحوب بأية ضمانة"
//       "<LI> إذا كان أي اختلاف في أوقات الصلاة مقارنة مع أذان أقرب مسجد"
//       " يرجى الاتصال بي في البريد الإلكتروني لكي أصحح الخطأ بإذن الله "
//      "<LI>ترقبوا ميزات جديدة في الإصدار المقبل للبرنامج ونسخ جديدة منه تعمل على لينكس ومحمول نوكيا"
//      "S60V3.1 , S60V3.2 , S60V5"
//      "</UL>"


//	"<a href=\"http://www.thakir.dz/\">http://www.thakir.dz/</a> : لتحميل أي تحديث"

//	"</p>"));
//       msgabout.setWindowTitle(QString::fromUtf8("حول البرنامج"));
//       msgabout.setWindowFlags(Qt::WindowStaysOnTopHint);
//       msgabout.setModal(true);
//       msgabout.setLayoutDirection(Qt::RightToLeft);
//      msgabout.show();
//      msgabout.exec();

//    QMessageBox::about(this,QString::fromUtf8("About App"),
//    		QString::fromUtf8("ذكر لأوقات الصلاة.\n"
//                "حفيان محمد إصدار 1.009\n\n"
//			"Programmé avec C++ (mingw32) avec commme IDE (Eclipse) et avec Qt Ver. "QT_VERSION_STR "\n\n"
//			"Sous Win XP\n\n"
//                "thakir.dz@gmail.com.\n"));
//}


void thakir_prayer_times::about()
{

    //athan->play();
    //releaseKb();
    showNormal(); //3-9-2009 pour evitter un crache si app et hide et clique sur ok de about
    actTexte1->setEnabled(false);
    dlgAbout.exec();
    actTexte1->setEnabled(true);
    // hide();
    // grabKb();

}

void thakir_prayer_times::aboutbis()
{
    //athan->play();
    //releaseKb();
    actTexte1->setEnabled(false);
    dlgAbout.exec();
    actTexte1->setEnabled(true);
    // grabKb();
}

void thakir_prayer_times::getAthanType()
{
   // qDebug() << "tabWidget_Athan->currentIndex()=" << tabWidget_Athan->currentIndex();

    if (radioButton_Athan_Makka->isChecked())
    {
        athan->setSource(QUrl("qrc:/sound/adhan_Makka.mp3"));
    }

    if (radioButton_Athan_Maddina->isChecked())
    {
        athan->setSource(QUrl("qrc:/sound/adhan_Maddina.mp3"));
    }

    if (radioButton_Athan_Aquassa->isChecked())
    {
        athan->setSource(QUrl("qrc:/sound/adthan_Quodss.wav"));
    }

    if (radioButton_Athan_Algeria->isChecked())
    {
        athan->setSource(QUrl("qrc:/sound/athan_Algeria.wav"));
    }
    alerteisdone=false;

}
void thakir_prayer_times::stopAthan()
{
    athan->stop();
}
void thakir_prayer_times::showQMessageBox(QMediaPlayer::MediaStatus MediaStatus)
{
    //    if(state==QMediaPlayer::PlayingState){
    //        pushButton_close->setText(QString::fromUtf8("ايقاف الاذان"));
    //        msgBox.show();
    //        athanwasplayed=true;
    //        alerteisdone=false;
    //    }else{
    //        pushButton_close->setText(QString::fromUtf8("إخفاء"));
    //        msgBox.close();
    //        athanwasplayed=false;
    //        alerteisdone=false;
    //    }

    switch (MediaStatus) {
    case QMediaPlayer::NoMedia:
        pushButton_close->setText(QString::fromUtf8("إخفاء"));
        //  msgBox.close();
        athanwasplayed=false;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
    case QMediaPlayer::LoadedMedia:
        // pushButton_close->setText(QString::fromUtf8("ايقاف الاذان"));
        //   msgBox.exec();
        athanwasplayed=true;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
    case QMediaPlayer::LoadingMedia:
        // msgBox.exec();
        // pushButton_close->setText(QString::fromUtf8("ايقاف الاذان"));
        athanwasplayed=true;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
    case QMediaPlayer::BufferingMedia:
        athanwasplayed=true;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
    case QMediaPlayer::BufferedMedia:
        //  msgBox.exec();
        athanwasplayed=true;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
    case QMediaPlayer::StalledMedia:
        // msgBox.close();
        //  msgBox.exec();
        athanwasplayed=false;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
    case QMediaPlayer::EndOfMedia:
        // msgBox.close();
        athanwasplayed=false;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
        break;
    case QMediaPlayer::InvalidMedia:
        // msgBox.close();
        athanwasplayed=false;
        alerteisdone=false;
       // qDebug() << "m_player->errorString()=" << athan->mediaStatus();
        break;
    }

}


/////-HAF 25-08-2025  v2.0.5 flatpak for flathub.org
void thakir_prayer_times::on_tabWidget_Athan_currentChanged(int index)
{
    readSettings();
    readDataFromUI();
    writeSettings();
}
void thakir_prayer_times::sendNotification(const QString &title,
                                          const QString &body,
                                          const QString &icon)
{
    if (!m_portalInterface) {
        emit notificationSent(false, "D-Bus interface not available");
        return;
    }

    QVariantMap options;
    options["title"] = title;
    options["body"] = body;

    if (!icon.isEmpty()) {
        options["icon"] = icon;
    }

    options["priority"] = "normal";

    QDBusMessage message = m_portalInterface->call(
        "AddNotification",
        "qt-app-notification",  // unique ID for this notification group
        options
        );

    if (message.type() == QDBusMessage::ErrorMessage) {
        emit notificationSent(false, message.errorMessage());
    } else {
        emit notificationSent(true);
    }
}

void thakir_prayer_times::sendNotificationWithActions(const QString &title,
                                                     const QString &body,
                                                     const QVariantMap &actions,
                                                     const QString &icon)
{
    if (!m_portalInterface) {
        emit notificationSent(false, "D-Bus interface not available");
        return;
    }

    QVariantMap options;
    options["title"] = title;
    options["body"] = body;

    if (!icon.isEmpty()) {
        options["icon"] = icon;
    }

    options["priority"] = "normal";
    options["default-action"] = "default";
    options["buttons"] = QVariant::fromValue(actions);

    QDBusPendingCall call = m_portalInterface->asyncCall(
        "AddNotification",
        "qt-app-notification",
        options
        );

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher]() {
        watcher->deleteLater();
        QDBusPendingReply<uint> reply = *watcher;
        if (reply.isError()) {
            emit notificationSent(false, reply.error().message());
        } else {
            m_lastNotificationId = reply.value();
            emit notificationSent(true);
        }
    });
}

void thakir_prayer_times::onNotificationResponse(uint id, uint response, const QVariantMap &results)
{
    Q_UNUSED(results);

    if (id == m_lastNotificationId) {
        // Handle the response (button click)
        QString actionId;
        switch (response) {
        case 0: actionId = "default"; break;
        case 1: actionId = "action1"; break;
        case 2: actionId = "action2"; break;
        default: actionId = QString::number(response);
        }

        emit actionInvoked(actionId);
    }
}


void thakir_prayer_times::on_checkBox_autostart_stateChanged(int arg1)
{
    if (arg1==0){
        QDBusInterface portal("org.freedesktop.portal.Desktop",
                              "/org/freedesktop/portal/desktop",
                              "org.freedesktop.portal.Background");

        QList<QString> commandline = {"thakir_prayer_times", "--hidden"};

        QHash<QString, QVariant> params;
        params.insert("reason", "Background Athan.");
        params.insert("autostart", false);
        params.insert("commandline", QVariant(commandline));

        portal.call("RequestBackground", "", params);
    }else{
        QDBusInterface portal("org.freedesktop.portal.Desktop",
                              "/org/freedesktop/portal/desktop",
                              "org.freedesktop.portal.Background");

        QList<QString> commandline = {"thakir_prayer_times", "--hidden"};

        QHash<QString, QVariant> params;
        params.insert("reason", "Background Athan.");
        params.insert("autostart", true);
        params.insert("commandline", QVariant(commandline));

        portal.call("RequestBackground", "", params);
    }
}

