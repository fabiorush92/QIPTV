#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // install message handler only on release
#ifndef QT_DEBUG
    qInstallMessageHandler(SyslogMessageHandler);
#endif

    // setup UI
    ui->setupUi(this);

    // restore last mainwindow state and geometry
    restoreGeometry(reg->value("MainWindowGeometry").toByteArray());
    restoreState(reg->value("MainWindowState").toByteArray());

    connect(pMan,&PanelManager::newPanelStatus,this,&MainWindow::panelStatusUpdate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    qDebug() << pMan->checkInternetConnection();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    reg->setValue("MainWindowGeometry", saveGeometry());
    reg->setValue("MainWindowState", saveState());
    event->accept();
}

void MainWindow::on_pushButton_clicked()
{
//    PanelInfo info;

//    // http://85.114.128.105:2475/panel_api.php?mode=auth&username=thisis85&password=ip85
//    info.host = "85.114.128.105:2475";
//    info.username = "thisis85";
//    info.password = "ip85";
//    pMan->subscribePanel(info);

//    // http://217.79.179.87:4647/panel_api.php?username=ipbrief&password=briefip
//    info.host = "217.79.179.87:4647";
//    info.username = "ipbrief";
//    info.password = "briefip";
//    pMan->subscribePanel(info);

//    pMan->start();
//    qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss.zzz") << "START";


    qDebug() << pForm->newForm();
}

void MainWindow::panelStatusUpdate(PanelStatus *status)
{
    qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss.zzz") << status;
}


