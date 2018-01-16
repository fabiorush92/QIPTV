#include "panelform.h"
#include "ui_panelform.h"

PanelForm::PanelForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PanelForm)
{
    ui->setupUi(this);

    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&PanelForm::accept);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&PanelForm::reject);
    connect(ui->lineURL,&QLineEdit::textChanged,this,&PanelForm::parseURL);
}

PanelForm::~PanelForm()
{
    delete ui;
}

PanelInfo PanelForm::newForm()
{
    return newForm(PanelInfo());
}

PanelInfo PanelForm::newForm(PanelInfo info)
{
    this->clear();
    this->insertInfo(info);

    PanelInfo newinfo;
    int ret = this->exec();

    if(ret == QDialog::Accepted)
    {
        newinfo.host = ui->lineHost->text();
        newinfo.username = ui->lineUsername->text();
        newinfo.password = ui->linePassword->text();
    }

    return newinfo;
}

void PanelForm::clear()
{
    ui->lineHost->clear();
    ui->linePassword->clear();
    ui->lineURL->clear();
    ui->lineUsername->clear();
}

void PanelForm::insertInfo(PanelInfo info)
{
    ui->lineURL->clear();

    ui->lineHost->setText(info.host);
    ui->lineUsername->setText(info.username);
    ui->linePassword->setText(info.password);
}

void PanelForm::parseURL()
{
    // http://85.114.128.105:2475/panel_api.php?mode=auth&username=thisis85&password=ip85

    ui->lineHost->clear();
    ui->lineUsername->clear();
    ui->linePassword->clear();

    QString username, password;
    QString url = ui->lineURL->text();
    url.remove("http://");
    url.remove(" ");

    QString host = url.section("/",0,0);
    if(host.contains(".") && !host.contains(QRegExp("[?&]")))
        ui->lineHost->setText(host);

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
