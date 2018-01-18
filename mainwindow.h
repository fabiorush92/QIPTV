#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>

#include <paneltypes.h>
#include <panelmanager.h>
#include <panelform.h>
#include <common/syslogmessagehandler.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSettings *reg = new QSettings(this);
    PanelManager *pManager = new PanelManager(this);
    PanelForm *pForm = new PanelForm(this);

    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();
    void panelStatusUpdate(PanelStatus *status);
};

#endif // MAINWINDOW_H
