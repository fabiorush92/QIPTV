#ifndef PANELFORM_H
#define PANELFORM_H

#include <QDialog>
#include <QDebug>

#include <panelmanager.h>

namespace Ui {
class PanelForm;
}

class PanelForm : public QDialog
{
    Q_OBJECT

public:
    explicit PanelForm(QWidget *parent = 0);
    ~PanelForm();

    PanelInfo newForm();
    PanelInfo newForm(PanelInfo info);

public slots:
    void clear();

private:
    Ui::PanelForm *ui;
    void insertInfo(PanelInfo info);

private slots:
    void parseURL();

};

#endif // PANELFORM_H
