#include "panelform.h"
#include "ui_panelform.h"

PanelForm::PanelForm(QWidget *parent) :
    QMyDialog(parent,"PanelForm","Panel informations"),
    ui(new Ui::PanelForm)
{
    ui->setupUi(this);

    // disabling deleteonclose, to permits ui values retention after closing
    this->setDeleteOnClose(false);

    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&PanelForm::checkInfo);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&PanelForm::reject);
    connect(ui->lineURL,&QLineEdit::textChanged,this,&PanelForm::parseURL);
}

PanelForm::~PanelForm()
{
    delete ui;
}

PanelInfo PanelForm::openForm()
{
    return openForm(PanelInfo());
}

PanelInfo PanelForm::openForm(PanelInfo info)
{  
    this->clear();
    this->insertInfo(info);

    int ret = this->exec();

    PanelInfo newinfo;
    if(ret == QDialog::Accepted)
    {
        newinfo.name = ui->lineName->text();
        newinfo.host = ui->lineHost->text();
        newinfo.username = ui->lineUsername->text();
        newinfo.password = ui->linePassword->text();
    }

    return newinfo;
}

void PanelForm::clear()
{
    ui->lineName->clear();
    ui->lineHost->clear();
    ui->linePassword->clear();
    ui->lineUsername->clear();
    ui->lineURL->clear();
}

void PanelForm::insertInfo(PanelInfo info)
{
    ui->lineURL->clear();

    ui->lineName->setText(info.name);
    ui->lineHost->setText(info.host);
    ui->lineUsername->setText(info.username);
    ui->linePassword->setText(info.password);
}

void PanelForm::parseURL()
{
    // clear ui
    ui->lineHost->clear();
    ui->lineUsername->clear();
    ui->linePassword->clear();

    QString username, password;
    QString url = ui->lineURL->text();
    url.remove("http://");
    url.remove(" ");

    // host, with checks
    QString host = url.section("/",0,0);
    if(host.contains(".") && !host.contains(QRegExp("[?&]")))
        ui->lineHost->setText(host);

    // username and password from php request
    QStringList list = url.split(QRegExp("[?&]"));
    for(QString str : list)
    {
        if(str.startsWith("username="))
            username = str.remove("username=");
        else if(str.startsWith("password="))
            password = str.remove("password=");
    }
    ui->lineUsername->setText(username);
    ui->linePassword->setText(password);
}

void PanelForm::checkInfo()
{
    // info checks

    if(ui->lineName->text().isEmpty())
    {
        qWarning() << "A valid name must be specified!";
        return;
    }

    if(ui->lineHost->text().isEmpty())
    {
        qWarning() << "A valid host must be specified!";
        return;
    }

    if(ui->lineUsername->text().isEmpty())
    {
        qWarning() << "A valid username must be specified!";
        return;
    }

    if(ui->linePassword->text().isEmpty())
    {
        qWarning() << "A valid password must be specified!";
        return;
    }

    // accept dialog only if checks are passed
    this->accept();
}
