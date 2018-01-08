#include "panelform.h"
#include "ui_panelform.h"

PanelForm::PanelForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PanelForm)
{
    ui->setupUi(this);

    connect(ui->lineURL,&QLineEdit::textChanged,this,&PanelForm::parseURL);
}

PanelForm::~PanelForm()
{
    delete ui;
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
