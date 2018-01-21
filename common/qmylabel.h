#ifndef QMYLABEL_H
#define QMYLABEL_H

#include <QObject>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QPalette>

class QMyLabel : public QLabel
{
    Q_OBJECT

    public:
    QMyLabel(QWidget * parent = 0) : QLabel(parent) {}
    QMyLabel(QString text, QWidget * parent = 0) : QLabel(text, parent) {}
    ~QMyLabel(){}

    void setColor(QColor color)
    {
        QPalette p = this->palette();
        p.setColor(QPalette::WindowText,color);
        this->setPalette(p);
    }

    signals:
        void clicked();
        void doubleClicked();
        void rightClicked();

    protected:
        void mouseReleaseEvent(QMouseEvent *event)
        {
            if(event->button() == Qt::RightButton)
                emit rightClicked();
            else
                emit clicked();
        }

        void mouseDoubleClickEvent(QMouseEvent *event)
        {
            Q_UNUSED(event);
            emit doubleClicked();
        }
};

#endif // QMYLABEL_H
