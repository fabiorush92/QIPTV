#include "qmydialog.h"

QMyDialog::QMyDialog(QWidget *parent, QString dialogName, QString dialogTitle) :
    QDialog(parent)
{
    _dialogName = dialogName;
    _dialogTitle = dialogTitle;

    this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void QMyDialog::show()
{
    restoreWindowState();
    setTitle();
    qApp->processEvents();

    QDialog::show();
}

int QMyDialog::exec()
{
    restoreWindowState();
    setTitle();
    qApp->processEvents();

    return QDialog::exec();
}

void QMyDialog::saveWindowState()
{
    if(reg != nullptr && !_dialogName.isEmpty())
    {
        reg->setValue(_dialogName + "/Geometry", this->saveGeometry());
    }
}

void QMyDialog::restoreWindowState()
{
    if(reg != nullptr && !_dialogName.isEmpty())
    {
        this->restoreGeometry(reg->value(_dialogName + "/Geometry").toByteArray());
    }
}
