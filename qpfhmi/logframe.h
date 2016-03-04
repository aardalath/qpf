#ifndef LOGFRAME_H
#define LOGFRAME_H

#include <QFrame>
#include <QPlainTextEdit>

namespace Ui {
class LogFrame;
}

class LogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LogFrame(QWidget *parent = 0);
    ~LogFrame();

    QPlainTextEdit * getTextEditor();

public slots:
    void setMsg(QString msg);
    void clearMsg();

    void displayLed(QString msgType);
    void clearLed();

private:
    Ui::LogFrame *ui;
};

#endif // LOGFRAME_H
