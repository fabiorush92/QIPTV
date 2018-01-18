#ifndef PANELREPOSITORY_H
#define PANELREPOSITORY_H

#include <QObject>
#include <QSettings>
#include <QList>

#include <paneltypes.h>

class PanelRepository : public QObject
{
    Q_OBJECT

public:
    explicit PanelRepository(QObject *parent = nullptr);

    PanelInfo getInfoByName(QString name);
    QList<PanelInfo> getList();
    QStringList getNameList();

    void setInfo(PanelInfo info);

    void deleteInfo(PanelInfo info);
    void deleteInfo(QString name);

private:
    QSettings *_reg = new QSettings(this);

};

#endif // PANELREPOSITORY_H
