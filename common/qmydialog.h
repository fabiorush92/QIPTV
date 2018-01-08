#ifndef QMYDIALOG_H
#define QMYDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QAction>

class QMyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QMyDialog(QWidget *parent, QString dialogName, QString dialogTitle);

    QString dialogName() { return _dialogName; }
    QSettings *ini = nullptr;
    QSettings *reg = nullptr;

    void setINIref(QSettings *iniReference)
    {
        ini = iniReference;
        while(!ini->group().isEmpty()) ini->endGroup();
    }

    void setREGref(QSettings *regReference)
    {
        reg = regReference;
        while(!reg->group().isEmpty()) reg->endGroup();
    }

    void setTitle() { this->setWindowTitle(_dialogTitle); }
    void setDeleteOnClose(bool on) { this->setAttribute(Qt::WA_DeleteOnClose, on); }

public slots:
    void show();
    int exec();

private:
    QString _dialogName;
    QString _dialogTitle;

    void saveWindowState();
    void restoreWindowState();

    void closeEvent(QCloseEvent *event)
    {
        saveWindowState();
        event->accept();
    }
};

#endif // QMYDIALOG_H
