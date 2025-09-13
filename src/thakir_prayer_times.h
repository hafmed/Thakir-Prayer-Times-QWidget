/****************************************************************************
** HAFIANE 29-8-2009
**
****************************************************************************/

#ifndef THAKIR_PRAYER_TIMES_H
#define THAKIR_PRAYER_TIMES_H

#include <QMainWindow>
#include "ui_thakir_prayer_times.h"
#include "about.h"
#include <QAction>

#include <QMediaPlayer>
#include <QMessageBox>
#include <QSettings>

#include <QSystemTrayIcon>

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QVariantMap>
#include <QString>

class thakir_prayer_times : public QMainWindow, public Ui::thakir_prayer_times
{
    Q_OBJECT

public:
    QSettings * HAF_settings;
    QString m_name_city_DZ;
    double m_latitude_DZ;
    double m_longitude_DZ;
    double m_local_elevation_DZ;
    QString m_name_city_Monde;
    double m_latitude_Monde;
    double m_longitude_Monde;
    double m_local_elevation_Monde;
    double m_time_zone_Monde;
    int m_itl_method_Monde;
    int  m_name_city_preconfig_DZ;
    int  m_name_city_commune_preconfig_DZ;

    bool m_radioButton_sitting_Algerie_isChecked;
    bool m_radioButton_sitting_HAF_isChecked;
    bool m_checkBox_Heure_ete_isChecked;

    bool m_radioButton_sansATHAN_isChecked;
    bool m_radioButton_Athan_Makka_isChecked;
    bool m_radioButton_Athan_Maddina_isChecked;
    bool m_radioButton_Athan_Aquassa_isChecked;
    bool m_radioButton_Athan_Algeria_isChecked;
    bool m_radioButton_sansALERTE_isChecked;
    bool m_radioButton_Alerte_Avant_isChecked;
    int m_spinBox_Temps_Alerte_Avant;

    bool m_checkBox_autostart_isChecked;

    thakir_prayer_times(QWidget *parent = 0);

    void setVisible(bool visible);
    QSystemTrayIcon* sticon;

    bool alerteisdone=false;
    bool AppisVisble=true;
    bool athanwasplayed=false;

    Q_INVOKABLE void sendNotification(const QString &title,
                                      const QString &body,
                                      const QString &icon = "");
    Q_INVOKABLE void sendNotificationWithActions(const QString &title,
                                                 const QString &body,
                                                 const QVariantMap &actions,
                                                 const QString &icon = "");

public slots:
    void stopAthan();

private slots:
    void onNotificationResponse(uint id, uint response, const QVariantMap &results);
    void thakir_prayer_times_Calculer();
    void thakir_prayer_times_Activer_toolBox1();
    void thakir_prayer_times_Activer_toolBox2();
    void radioButton_sitting_HAF_setNotChecked();
    void radioButton_sitting_user_setNotChecked();
    void thakir_prayer_times_config();
    void readSettings();
    void writeSettings();
    void updateSettings();
    void readDataFromUI();
    void aboutbis();
    void closeEvent(QCloseEvent *event);
    void playAthan(QString next_prayer_text,QMediaPlayer* athan);
    void about();
    void COMMUN_Update(int);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void getAthanType();


    void showQMessageBox(QMediaPlayer::MediaStatus status);

    void on_tabWidget_Athan_currentChanged(int index);

    void on_checkBox_autostart_stateChanged(int arg1);

private:
    QDBusInterface *m_portalInterface;
    uint m_lastNotificationId;
    QVariantMap m_pendingActions;

    QAction *actTexte1;
    QAction *actTexte2;
    QAction *actTexte3;
    QAction *actTexte4;

    About dlgAbout;
    QString next_prayer_text;
    QTimer *timer;
    QMessageBox msgBox;
    QMediaPlayer *athan = nullptr;
    QAudioOutput *m_audioOutput = nullptr;

signals:
    void notificationSent(bool success, const QString &error = "");
    void actionInvoked(const QString &actionId);

};

#endif // THAKIR_PRAYER_TIMES_H
