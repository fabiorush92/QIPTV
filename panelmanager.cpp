#include "panelmanager.h"

PanelManager::PanelManager(QObject *parent) : QObject(parent)
{
    // connection between timer timeout & request
    connect(_timer,&QTimer::timeout,this,&PanelManager::getSubscribersStatus);

    // connection between request & reply
    connect(_netman, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));   
}

void PanelManager::start()
{
    _timer->stop();
    _timer->start(_tick);
    emit managerStarted();
    getSubscribersStatus();
}

void PanelManager::stop()
{
    _timer->stop();
    emit managerStopped();
}

void PanelManager::getSubscribersStatus()
{
    for(PanelInfo info : _listPanels)
    {
        getPanelStatus(info);
    }
}

void PanelManager::getPanelStatus(PanelInfo info)
{
    // url construction
    QString url = QString(URL_PANELAPI);
    url.replace("[host]",info.host);
    url.replace("[username]",info.username);
    url.replace("[password]",info.password);

    // build request
    QNetworkRequest req = QNetworkRequest(QUrl(url));

    // inject info name into request's user attribute
    // (this permits reply association with the original request)
    req.setAttribute(QNetworkRequest::User, info.name);

    // start request
    _netman->get(req);
}

bool PanelManager::checkInternetConnection()
{
    QEventLoop loop;
    QNetworkReply *reply = _servicenetman->get(QNetworkRequest(QUrl("http://www.google.com")));
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
    reply->deleteLater();
    return reply->bytesAvailable();
}

void PanelManager::replyFinished(QNetworkReply *reply)
{
    PanelStatus *_lastStatus = new PanelStatus(this);
    _lastStatus->setReqName(reply->request().attribute(QNetworkRequest::User).toString());

    // check if the reply is finished
    if(!reply->bytesAvailable())
    {
        emit replyError(_lastStatus->reqName(),reply->errorString());
        return;
    }

    // json document parsing
    _json = QJsonDocument::fromJson(reply->readAll());
    if(_json.isEmpty() || _json.isNull())
    {
        //qCritical() << "JSON: Parsing error!" << QString("[%1]").arg(_lastStatus->reqName());
        emit replyError(_lastStatus->reqName(),"Panel reply parsing error!");
        return;
    }  

    QJsonObject userOBJ;
    QJsonObject serverOBJ;

    for (QString key : _json.object().keys())
    {
        if(key == "server_info")
            serverOBJ = _json.object().value(key).toObject();
        else if(key == "user_info")
            userOBJ = _json.object().value(key).toObject();
    }

    for (QString key :  serverOBJ.keys())
    {
        if(key == "url")
            _lastStatus->setUrl(serverOBJ.value(key).toString());
        else if(key == "port")
            _lastStatus->setPort(serverOBJ.value(key).toVariant().toInt());
    }

    for (QString key : userOBJ.keys())
    {
        if(key == "username")
            _lastStatus->setUsername(userOBJ.value(key).toString());
        else if(key == "password")
            _lastStatus->setPassword(userOBJ.value(key).toString());
        else if(key == "auth")
            _lastStatus->setAuth(userOBJ.value(key).toVariant().toInt());
        else if(key == "status")
        {
            PanelStatus::AccountStatus s;
            QString _str = userOBJ.value(key).toString();

            if(_str == "Active")
                s = PanelStatus::Active;
            else if(_str == "Disabled")
                s = PanelStatus::Disabled;
            else if(_str == "Expired")
                s = PanelStatus::Expired;
            else if(_str == "Banned")
                s = PanelStatus::Banned;

            _lastStatus->setAccountStatus(s);
        }
        else if(key == "exp_date")
            _lastStatus->setDTExpire(QDateTime::fromSecsSinceEpoch(userOBJ.value(key).toVariant().toUInt()));
        else if(key == "created_at")
            _lastStatus->setDTCreated(QDateTime::fromSecsSinceEpoch(userOBJ.value(key).toVariant().toUInt()));
        else if(key == "is_trial")
            _lastStatus->setIsTrial(userOBJ.value(key).toVariant().toBool());
        else if(key == "active_cons")
            _lastStatus->setActiveConnections(userOBJ.value(key).toVariant().toInt());
        else if(key == "max_connections")
            _lastStatus->setMaxConnections(userOBJ.value(key).toVariant().toInt());
    }

    // delete objects
    reply->deleteLater();
    _lastStatus->deleteLater();

    // throw new status
    emit newPanelStatus(_lastStatus);
}
