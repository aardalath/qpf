/******************************************************************************
 * File:    frmagentstatus.h
 *          Declaration of class FrmAgentStatus
 *
 * Domain:  QPF.qpfgui.frmagentstatus
 *
 * Version:  2.0
 *
 * Date:    2016-11-03
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2018 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Declaration of class FrmAgentStatus
 *
 * Created by:
 *   J C Gonzalez
 *
 * Status:
 *   Prototype
 *
 * Dependencies:
 *   none
 *
 * Files read / modified:
 *   none
 *
 * History:
 *   See <ChangeLog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#ifndef FRMAGENTSTATUS_H
#define FRMAGENTSTATUS_H

#include <QFrame>

#include "procinfo.h"

namespace Ui {
class FrmAgentStatus;
}

namespace QPF {

//struct TaskAgentInfo {};

class FrmAgentStatus : public QFrame
{
    Q_OBJECT

    typedef float percent;
    
public:
    explicit FrmAgentStatus(QWidget *parent = 0);
    ~FrmAgentStatus();

    void updateInfo(QString name, QString mode, TaskStatusSpectra & d);

private:
    void paintArcSection(QRectF & rect, QPainter & painter,
                         const QColor & color,
                         percent start, percent end);

    Ui::FrmAgentStatus *ui;

    static const int HalfCircle;
    static const float HalfCircleWidth;
    static QPen pen;
    static QVector<QColor> colors;
};

}

#endif // FRMAGENTSTATUS_H
