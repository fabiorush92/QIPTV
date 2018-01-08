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
#include <QMetaEnum>
#include <QEventLoop>

#define URL_PANELAPI "http://[host]/panel_api.php?mode=auth&username=[username]&password=[password]"
#define URL_GET "http://[host]/get.php?username=[username]&password=[password]&type=m3u&output=ts"


// PanelInfo
struct PanelInfo
{
    QString host;
    QString username;
    QString password;
};


// PanelStatus
class PanelStatus : public QObject
{
    Q_OBJECT

public:
    enum AccountStatus {
        Active, Disabled, Expired, Banned, Unknown
    };
    Q_ENUM(AccountStatus)

    explicit PanelStatus(QObject *parent = nullptr) : QObject(parent) {}

    AccountStatus accountStatus() const { return m_accountStatus; }
    void setAccountStatus(const AccountStatus &accStatus) { m_accountStatus = accStatus; }
    QDateTime DTCreated() const { return m_DTCreated; }
    void setDTCreated(const QDateTime &DTCreated) { m_DTCreated = DTCreated; }
    QDateTime DTExpire() const { return m_DTExpire; }
    void setDTExpire(const QDateTime &DTExpire) { m_DTExpire = DTExpire; }
    QString username() const { return m_username; }
    void setUsername(const QString &username) { m_username = username; }
    QString password() const { return m_password; }
    void setPassword(const QString &password) { m_password = password; }
    int auth() const { return m_auth; }
    void setAuth(int auth) { m_auth = auth; }
    int maxConnections() const { return m_maxConnections; }
    void setMaxConnections(int maxConnections) { m_maxConnections = maxConnections; }
    int activeConnections() const { return m_activeConnections; }
    void setActiveConnections(int activeConnections) { m_activeConnections = activeConnections; }
    bool isTrial() const { return m_isTrial; }
    void setIsTrial(bool isTrial) { m_isTrial = isTrial; }
    QString url() const { return m_url; }
    void setUrl(const QString &url) { m_url = url; }
    int port() const { return m_port; }
    void setPort(int port) { m_port = port; }

private:
    AccountStatus m_accountStatus = Unknown;
    QDateTime m_DTCreated, m_DTExpire;
    QString m_username, m_password;
    int m_auth = 0;
    int m_maxConnections = 0;
    int m_activeConnections = 0;
    bool m_isTrial = false;
    QString m_url;
    int m_port;
};

inline QDebug operator << (QDebug dbg, PanelStatus *status)
{
    dbg.space() << "PanelStatus ["
                << QString("Username:%1").arg(status->username())
                << QString("Password:%1").arg(status->password())
                << QString("Auth:%1").arg(status->auth())
                << QString("Status:%1").arg(QMetaEnum::fromType<PanelStatus::AccountStatus>().valueToKey(status->accountStatus()))
                << QString("ActiveCons:%1").arg(status->activeConnections())
                << QString("MaxCons:%1").arg(status->maxConnections())
                << QString("Created:%1").arg(status->DTCreated().toString("dd/MM/yyyy-HH:mm"))
                << QString("Expire:%1").arg(status->DTExpire().toString("dd/MM/yyyy-HH:mm"))
                << "]";

    return dbg.maybeSpace();
}


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
