#ifndef PANELMANAGER_H
#define PANELMANAGER_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QEventLoop>

#include <paneltypes.h>

#define URL_PANELAPI "http://[host]/panel_api.php?mode=auth&username=[username]&password=[password]"
#define URL_GET "http://[host]/get.php?username=[username]&password=[password]&type=m3u&output=ts"

// PanelManager
class PanelManager : public QObject
{
    Q_OBJECT

public:
    explicit PanelManager(QObject *parent = nullptr);

    bool checkInternetConnection();
    void subscribePanel(PanelInfo info) { _listPanels << info; }

signals:
    void newPanelStatus(PanelStatus*);
    void managerStarted();
    void managerStopped();
    void replyError(QString reqName, QString errorText);

public slots:
    void unsubscribeAll() { _listPanels.clear(); }
    void setTick(int msec) { _tick = msec; }
    void start();
    void stop();

private slots:
    void getSubscribersStatus();
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *_netman = new QNetworkAccessManager(this);
    QNetworkAccessManager *_servicenetman = new QNetworkAccessManager(this);
    QTimer *_timer = new QTimer(this);

    QList<PanelInfo> _listPanels;
    QJsonDocument _json;
    int _tick = 5000;

    void getPanelStatus(PanelInfo info);
};

#endif
