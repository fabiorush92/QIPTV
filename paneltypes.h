#ifndef PANELTYPES_H
#define PANELTYPES_H

#include <QObject>
#include <QDebug>
#include <QMetaEnum>
#include <QDateTime>

// PanelInfo
struct PanelInfo
{
    QString name;
    QString host;
    QString username;
    QString password;

    bool isValid()
    {
        return (!name.isEmpty() &&
                !host.isEmpty() &&
                !username.isEmpty() &&
                !password.isEmpty());
    }

    bool operator == (const PanelInfo &rhs)
    {
        return (name == rhs.name &&
                host == rhs.host &&
                username == rhs.username &&
                password == rhs.password);
    }

    bool operator != (const PanelInfo &rhs)
    {
        return !(this == &rhs);
    }
};

inline QDebug operator << (QDebug dbg, PanelInfo info)
{
    dbg.space() << "PanelInfo ["
                << QString("IsValid:%1").arg((bool)info.isValid())
                << QString("Name:%1").arg(info.name)
                << QString("Host:%1").arg(info.host)
                << QString("Username:%1").arg(info.username)
                << QString("Password:%1").arg(info.password)
                << "]";

    return dbg.maybeSpace();
}


// PanelStatus
class PanelStatus : public QObject
{
    Q_OBJECT

public:
    enum AccountStatus {
        Active, Disabled, Expired, Banned, Unknown
    };
    Q_ENUM(AccountStatus)

    //PanelStatus(QObject *parent = nullptr) : QObject(parent) {}
    PanelStatus() {}
    PanelStatus &operator=(const PanelStatus &other);
    PanelStatus(const PanelStatus &other);

    QString reqName() const { return m_reqName; }
    void setReqName(const QString &reqName) { m_reqName = reqName; }
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
    QString m_reqName;
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
                << QString("RequestName:%1").arg(status->reqName())
                //<< QString("Username:%1").arg(status->username())
                //<< QString("Password:%1").arg(status->password())
                //<< QString("Auth:%1").arg(status->auth())
                << QString("Status:%1").arg(QMetaEnum::fromType<PanelStatus::AccountStatus>().valueToKey(status->accountStatus()))
                << QString("Active:%1").arg(status->activeConnections())
                << QString("Max:%1").arg(status->maxConnections())
                << QString("Created:%1").arg(status->DTCreated().toString("dd/MM/yyyy-HH:mm"))
                << QString("Expire:%1").arg(status->DTExpire().toString("dd/MM/yyyy-HH:mm"))
                << "]";

    return dbg.maybeSpace();
}


#endif // PANELTYPES_H
