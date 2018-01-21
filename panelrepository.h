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

    PanelInfo getInfo(QString name);
    QList<PanelInfo> getInfoList();
    QStringList getInfoNames();

    void setInfo(PanelInfo info);
    void addInfo(PanelInfo info);
    void deleteInfo(PanelInfo info);
    void deleteInfo(QString name);

    PanelStatus* getLastStatus(QString name);
    QList<PanelStatus *> getLastStatusList();
    void addStatus(PanelStatus *status);

public slots:
    void clearStatusList() { _statusList.clear(); }

private:
    QSettings *_reg = new QSettings(this);
    QList<PanelStatus*> _statusList;
};

#endif // PANELREPOSITORY_H
