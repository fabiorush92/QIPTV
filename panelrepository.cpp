#include "panelrepository.h"

PanelRepository::PanelRepository(QObject *parent) : QObject(parent)
{
    // enter into "Repository" group, needed to retrieve childKeys()
    _reg->beginGroup("Repository");
}

PanelInfo PanelRepository::getInfoByName(QString name)
{
    PanelInfo info;

    if(!getNameList().contains(name))
        return info;

    // enter into name group
    _reg->beginGroup(name);

    info.name = name;
    info.host = _reg->value("Host").toString();
    info.username = _reg->value("Username").toString();
    info.password = _reg->value("Password").toString();

    // exit from name group
    _reg->endGroup();

    return info;
}

QStringList PanelRepository::getNameList()
{
    // get repository childKeys()
    return _reg->childKeys();
}

QList<PanelInfo> PanelRepository::getList()
{
    // list creation
    QList<PanelInfo> list;

    // get info of every child and build PanelInfo list
    for(QString name : getNameList())
    {
        list.append(getInfoByName(name));
    }

    return list;
}

void PanelRepository::setInfo(PanelInfo info)
{
    _reg->setValue(QString("%1/Host").arg(info.name), info.host);
    _reg->setValue(QString("%1/Username").arg(info.name), info.username);
    _reg->setValue(QString("%1/Password").arg(info.name), info.password);
}

void PanelRepository::deleteInfo(PanelInfo info)
{
    deleteInfo(info.name);
}

void PanelRepository::deleteInfo(QString name)
{
    _reg->remove(QString("%1").arg(name));
}
