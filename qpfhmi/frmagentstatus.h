#ifndef FRMAGENTSTATUS_H
#define FRMAGENTSTATUS_H

#include <QFrame>

namespace Ui {
class FrmAgentStatus;
}

class FrmAgentStatus : public QFrame
{
    Q_OBJECT

public:
    explicit FrmAgentStatus(QWidget *parent = 0);
    ~FrmAgentStatus();

private:
    Ui::FrmAgentStatus *ui;
};

#endif // FRMAGENTSTATUS_H
