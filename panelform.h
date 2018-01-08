#ifndef PANELFORM_H
#define PANELFORM_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class PanelForm;
}

class PanelForm : public QDialog
{
    Q_OBJECT

public:
    explicit PanelForm(QWidget *parent = 0);
    ~PanelForm();

private:
    Ui::PanelForm *ui;

private slots:
    void parseURL();

};

#endif // PANELFORM_H
