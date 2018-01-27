#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>

#include <paneltypes.h>
#include <panelmanager.h>
#include <panelform.h>
#include <panelrepository.h>

#include <common/syslogmessagehandler.h>
#include <common/qmylabel.h>

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
    PanelRepository *pRepo = new PanelRepository(this);

    QMyLabel *labConnection = new QMyLabel(this);

    void closeEvent(QCloseEvent *event);

    int searchInfoIntoTable(PanelInfo info);
    PanelInfo getSelectedInfo();
    void setTableItem(int row, int column, QString string);
    //void setTableItem(int row, int column, QWidget *widget);
    void addPanelToTable(PanelInfo info);
    void insertStatusIntoTable(PanelStatus *status);

private slots:
    void clearTable();
    void updateTable();
    void addNewPanelToRepository();
    void removeSelectedPanelFromRepository();

    void updatePanel(PanelStatus *status);
    void errorPanel(QString name, QString errorText);
};

#endif // MAINWINDOW_H
