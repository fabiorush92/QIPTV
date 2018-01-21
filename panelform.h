#ifndef PANELFORM_H
#define PANELFORM_H

#include <QDialog>
#include <QDebug>

#include <common/qmydialog.h>
#include <paneltypes.h>

namespace Ui {
class PanelForm;
}

class PanelForm : public QMyDialog
{
    Q_OBJECT

public:
    explicit PanelForm(QWidget *parent = 0);
    ~PanelForm();

    PanelInfo openForm();
    PanelInfo openForm(PanelInfo info);

public slots:
    void clear();

private:
    Ui::PanelForm *ui;
    void insertInfo(PanelInfo info);

private slots:
    void parseURL();
    void checkInfo();

};

#endif // PANELFORM_H
