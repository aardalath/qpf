#ifndef FRMAGENTSTATUS_H
#define FRMAGENTSTATUS_H

#include <QFrame>
#include "datatypes.h"

namespace Ui {
class FrmAgentStatus;
}

namespace QPF {

class FrmAgentStatus : public QFrame
{
    Q_OBJECT

public:
    explicit FrmAgentStatus(QWidget *parent = 0);
    ~FrmAgentStatus();

    void updateInfo(TaskAgentInfo &d);

private:
    Ui::FrmAgentStatus *ui;
};

}

#endif // FRMAGENTSTATUS_H
