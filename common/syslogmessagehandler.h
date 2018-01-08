#ifndef SYSLOGMESSAGEHANDLER_H
#define SYSLOGMESSAGEHANDLER_H

#include <QMessageBox>
#include <QErrorMessage>
#include <QFile>
#include <QTextStream>
#include <QMessageLogContext>

static inline void SyslogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString strType;
    QMessageBox::Icon msgIcon;
    switch (type)
    {
    case QtDebugMsg:
        strType = "Debug";
        msgIcon = QMessageBox::Information;
        break;
    case QtInfoMsg:
        strType = "Info";
        msgIcon = QMessageBox::Information;
        break;
    case QtWarningMsg:
        strType = "Warning";
        msgIcon = QMessageBox::Warning;
        break;
    case QtCriticalMsg:
        strType = "Critical";
        msgIcon = QMessageBox::Critical;
        break;
    case QtFatalMsg:
        strType = "Fatal";
        msgIcon = QMessageBox::Critical;
        break;
    }

    QFile log_file("log.txt");
    log_file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    QTextStream stream(&log_file);

    stream << QString("[%1] %2: %3 (%4/%5, %6)\n")
              .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss.zzz"))
              .arg(strType)
              .arg(msg)
              .arg(context.file)
              .arg(context.line)
              .arg(context.function);

    log_file.close();

    QMessageBox msgBox;
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
    msgBox.setModal(true);
    msgBox.setWindowTitle(QString("QIPTV [%1]").arg(strType));
    msgBox.setText(msg);
    msgBox.setIcon(msgIcon);
    msgBox.exec();
}

#endif // SYSLOGMESSAGEHANDLER_H
