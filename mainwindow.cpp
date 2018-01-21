#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // install message handler [only on release]
#ifndef QT_DEBUG
    qInstallMessageHandler(SyslogMessageHandler);
#endif

    // pass reg reference at QMyDialog
    pForm->setREGref(reg);

    // UI setup
    ui->setupUi(this);    

    ui->statusBar->addPermanentWidget(labConnection);

    // restore last mainwindow state and geometry
    restoreGeometry(reg->value("MainWindowGeometry").toByteArray());
    restoreState(reg->value("MainWindowState").toByteArray());

    // UI connections
    connect(ui->actionAboutQt,&QAction::triggered,qApp,&QApplication::aboutQt);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
    connect(ui->actionAddPanel,&QAction::triggered,this,&MainWindow::addNewPanelToRepository);

    // logic connections
    connect(pManager,&PanelManager::newPanelStatus,this,&MainWindow::updatePanel);
    connect(pManager,&PanelManager::replyError,this,&MainWindow::errorPanel);

    // read repository
    readRepository();

    // check internet connection
    if(pManager->checkInternetConnection())
    {
        labConnection->setText("Connesso");
        labConnection->setColor(Qt::darkGreen);
    }
    else
    {
        labConnection->setText("Disconnesso");
        labConnection->setColor(Qt::darkRed);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    reg->setValue("MainWindowGeometry", saveGeometry());
    reg->setValue("MainWindowState", saveState());
    event->accept();
}

PanelInfo MainWindow::getSelectedInfo()
{
    if(ui->tablePanels->selectedItems().isEmpty())
        return PanelInfo();

    return pRepo->getInfoByName(ui->tablePanels->item(ui->tablePanels->selectedItems().at(0)->row(), 0)->text());
}

int MainWindow::searchInfoIntoTable(PanelInfo info)
{
    for(int i=0;i<ui->tablePanels->rowCount();i++)
    {
        if(ui->tablePanels->item(i,0)->text() == info.name)
            return i;   // name finded, return index
    }

    // search failed
    return -1;
}

void MainWindow::setTableItem(int row, int column, QString string)
{
    ui->tablePanels->setItem(row, column, new QTableWidgetItem(string));
}

void MainWindow::addPanelToTable(PanelInfo info)
{
    ui->tablePanels->setRowCount(ui->tablePanels->rowCount() + 1);

    int index = ui->tablePanels->rowCount() - 1;

    setTableItem(index,0,info.name);
}

void MainWindow::clearTable()
{
    ui->tablePanels->clear();

    // NAME ACCOUNTSTATUS VISIONSTATUS EXPIREDATE INFOS
    ui->tablePanels->setColumnCount(5);
    ui->tablePanels->setRowCount(0);

    // hide headers
    ui->tablePanels->horizontalHeader()->setVisible(false);
    ui->tablePanels->verticalHeader()->setVisible(false);

    // stretch last section
    ui->tablePanels->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::readRepository()
{
    // stop panel manager
    pManager->stop();

    // clear table
    clearTable();

    // unsubscribe
    pManager->unsubscribeAll();

    // get panels from repo, add to the table and subscribe all
    for(PanelInfo info : pRepo->getList())
    {
        addPanelToTable(info);
        pManager->subscribePanel(info);
    }

    // resize table
    ui->tablePanels->resizeRowsToContents();
    ui->tablePanels->resizeColumnsToContents();

    // start panel manager
    pManager->start();
}

void MainWindow::addNewPanelToRepository()
{
    pRepo->addInfo(pForm->openForm());

    readRepository();
}

void MainWindow::removeSelectedPanelFromRepository()
{

}

void MainWindow::updatePanel(PanelStatus *status)
{
    qDebug() << status;
}

void MainWindow::errorPanel(QString name, QString errorText)
{
    ui->statusBar->showMessage(QString("[%1] Error: %2").arg(name).arg(errorText),3000);
}
