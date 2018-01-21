#include "panelrepository.h"

PanelRepository::PanelRepository(QObject *parent) : QObject(parent)
{
    // enter into "Repository" group, needed to retrieve childGroup() list
    _reg->beginGroup("Repository");
}

PanelInfo PanelRepository::getInfo(QString name)
{
    PanelInfo info;

    if(!getInfoNames().contains(name))
    {
        qWarning() << "Name not found into repository!";
        return info;
    }

    // enter into name group
    _reg->beginGroup(name);

    info.name = name;
    info.host = _reg->value("Host").toString();
    info.username = _reg->value("Username").toString();
    info.password = _reg->value("Password").toString();

    // exit from name group
    _reg->endGroup();

    //sync
    _reg->sync();

    return info;
}

QStringList PanelRepository::getInfoNames()
{
    // get repository grouplist()
    return _reg->childGroups();
}

QList<PanelInfo> PanelRepository::getInfoList()
{
    // list creation
    QList<PanelInfo> list;

    // get info of every child and build PanelInfo list
    for(QString name : getInfoNames())
    {
        list.append(getInfo(name));
    }

    return list;
}

void PanelRepository::setInfo(PanelInfo info)
{
    if(!info.isValid())
    {
        qWarning() << "Repository: Panel not valid!";
        return;
    }

    _reg->setValue(QString("%1/Host").arg(info.name), info.host);
    _reg->setValue(QString("%1/Username").arg(info.name), info.username);
    _reg->setValue(QString("%1/Password").arg(info.name), info.password);
    _reg->sync();
}

void PanelRepository::addInfo(PanelInfo info)
{
    setInfo(info);
}

void PanelRepository::deleteInfo(PanelInfo info)
{
    deleteInfo(info.name);
}

void PanelRepository::deleteInfo(QString name)
{
    _reg->remove(QString("%1").arg(name));
    _reg->sync();
}

PanelStatus* PanelRepository::getLastStatus(QString name)
{
    // find status name into list
    for(PanelStatus *status : _statusList)
    {
        if(status->reqName() == name)
            return status;
    }

    return new PanelStatus(this);
}

QList<PanelStatus*> PanelRepository::getLastStatusList()
{
    return this->_statusList;
}

void PanelRepository::addStatus(PanelStatus *status)
{
    // delete all statuses with new status name
    if(!_statusList.isEmpty())
    {
        //for(PanelStatus *Istatus : _statusList)
        for(int i=0;i<_statusList.size();i++)
        {
            if(_statusList[i]->reqName() == status->reqName())
                _statusList.removeOne(_statusList[i])
        }
    }

    // append
    _statusList.append(status);
}
