/******************************************************************************
 * File:    frmagentstatus.cpp
 *          Implementation of class FrmAgentStatus
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
 *   Implementation of class FrmAgentStatus
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

#include "frmagentstatus.h"
#include "ui_frmagentstatus.h"

#include <QPainter>
#include <QPixmap>

namespace QPF {

FrmAgentStatus::FrmAgentStatus(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrmAgentStatus)
{
    qsrand(static_cast<quint64>(time(NULL)));

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(24);
    pen.setCapStyle(Qt::FlatCap);
                
    ui->setupUi(this);
}

FrmAgentStatus::~FrmAgentStatus()
{
    delete ui;
}

void FrmAgentStatus::updateInfo(QString name, QString mode, TaskStatusSpectra & d)
{
    ui->lblTaskAgentName->setText(name);
    ui->lblTaskAgentMode->setText(mode);

    ui->lblRun->setText(QString("%1").arg(d.running));
    ui->lblWait->setText(QString("%1").arg(d.scheduled));
    ui->lblPau->setText(QString("%1").arg(d.paused));
    ui->lblSto->setText(QString("%1").arg(d.stopped));
    ui->lblFail->setText(QString("%1").arg(d.failed));
    ui->lblFin->setText(QString("%1").arg(d.finished));

    ui->lblTotalTasks->setText(QString("%1").arg(d.total));

    if (d.total > 0) {
        int w = ui->lblArcSpectrum->width();
        int h = ui->lblArcSpectrum->height();
        QPixmap pixmap(w, 2 * h);
        pixmap.fill(QColor("transparent"));

        QPainter painter(&pixmap);
        
        QRectF rect(HalfCircleWidth, HalfCircleWidth,
                    w - 2. * HalfCircleWidth - 2.,
                    2. * h - 2. * HalfCircleWidth - 2.);
        int start = 0;
        int end;
        percent toPerc = 1.0 / (float)(d.running + d.scheduled + d.paused +
                                       d.stopped + d.failed + d.finished);
        int i = 0;
        for (auto delta : {d.running, d.scheduled, d.paused,
                    d.stopped, d.failed, d.finished}) {
            end = start + delta;
            paintArcSection(rect, painter, colors.at(i),
                            start * toPerc, end * toPerc);
            start = end;
            ++i;
        }

        ui->lblArcSpectrum->setPixmap(pixmap.copy(0, 0, w, h));
    }
}

void FrmAgentStatus::paintArcSection(QRectF & rect, QPainter & painter,
                                     const QColor & color,
                                     percent start, percent end)
{
    int startAngle = (int)(HalfCircle * (1.0 - start));
    int spanAngle = (int)(HalfCircle * (end - start));
    startAngle -= spanAngle;

    pen.setColor(color);
    painter.setPen(pen);
    painter.setBrush(color);
    painter.drawArc(rect, startAngle, spanAngle);
}

const int FrmAgentStatus::HalfCircle = 180 * 16;
const float FrmAgentStatus::HalfCircleWidth = 16.;
QPen FrmAgentStatus::pen;
QVector<QColor> FrmAgentStatus::colors = QVector<QColor> {QColor(0, 0, 255),
                                                          QColor(14, 170, 204),
                                                          QColor(255, 165, 0),
                                                          QColor(121, 121, 121),
                                                          QColor(255, 0, 0),
                                                          QColor(33, 202, 33)};
    
}
