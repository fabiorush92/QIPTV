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

    // check internet connection
    if(pManager->checkInternetConnection())
    {
        labConnection->setText("Connected");
        labConnection->setColor(Qt::darkGreen);
    }
    else
    {
        labConnection->setText("Disconnected");
        labConnection->setColor(Qt::darkRed);
    }

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

    // update table
    updateTable();

    PanelInfo info;
    info.name = ui->label_9->text();
    qDebug() << info;
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

    return pRepo->getInfo(ui->tablePanels->item(ui->tablePanels->selectedItems().at(0)->row(), 0)->text());
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
    ui->tablePanels->setRowCount(ui->tablePanels->rowCount() + 1);      // increase row count
    int index = ui->tablePanels->rowCount() - 1;                        // save index
    setTableItem(index,0,info.name);                                    // always set name of panel into table
    insertStatusIntoTable(pRepo->getLastStatus(info.name));             // search status
}

void MainWindow::insertStatusIntoTable(PanelStatus *status)
{
    // if the account status is Unknown the status is not inizialized
    if(status->accountStatus() == PanelStatus::Unknown)
        return;

    // find correct panel row

    fare una funzione veloce per ottenere la riga della table in base al nome


}

void MainWindow::clearTable()
{
    // NAME ACCOUNTSTATUS VISIONSTATUS EXPIREDATE INFOS
    ui->tablePanels->clear();
    ui->tablePanels->setColumnCount(5);
    ui->tablePanels->setRowCount(0);

    // hide headers
    ui->tablePanels->horizontalHeader()->setVisible(false);
    ui->tablePanels->verticalHeader()->setVisible(false);

    // stretch last section
    ui->tablePanels->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::updateTable()
{
    // stop panel manager
    pManager->stop();

    // clear table
    clearTable();

    // unsubscribe
    pManager->unsubscribeAll();

    // get panels from repo, add to the table and subscribe all
    for(PanelInfo info : pRepo->getInfoList())
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

    updateTable();
}

void MainWindow::removeSelectedPanelFromRepository()
{
    if(ui->tablePanels->selectedItems().size() > 0)
    {
        int row = ui->tablePanels->selectedItems().at(0)->row();
        pRepo->deleteInfo(pRepo->getInfo(ui->tablePanels->item(row,0)->text()));
    }

    updateTable();
}

void MainWindow::updatePanel(PanelStatus *status)
{
    pRepo->addStatus(status);
}

void MainWindow::errorPanel(QString name, QString errorText)
{
    ui->statusBar->showMessage(QString("[%1] Error: %2").arg(name).arg(errorText),3000);
}
