/******************************************************************************
 * File:    progbardlg.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.progbardlg
 *
 * Version: 1.0
 *
 * Date:    2015/07/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement ProgressBarDelegate class
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
 *   See <Changelog>
 *
 * About: License Conditions
 *   See <License>
 *
 ******************************************************************************/

#include "progbardlg.h"

#include <QDebug>
#include <QApplication>

namespace QPF {

ProgressBarDelegate::ProgressBarDelegate(QObject * parent)
 : QItemDelegate(parent)
{
}

ProgressBarDelegate::~ProgressBarDelegate()
{
}

void ProgressBarDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    //QProgressBar progressbar;

    if (index.column() != 6) {
        QItemDelegate::paint(painter, option, index);
        return;
    }

    QStyleOptionProgressBarV2 progressBarOption;

    QRect rect = option.rect;
    rect.setWidth(rect.width()*19/20);

    progressBarOption.state = QStyle::State_Enabled;
    progressBarOption.direction = QApplication::layoutDirection();
    progressBarOption.rect = rect;
    progressBarOption.fontMetrics = QApplication::fontMetrics();
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.textVisible = true;

    //QModelIndex ind = index.parent();
    int progress = index.data(Qt::DisplayRole).toInt();
    // Set the progress and text values of the style option.
    progressBarOption.progress = progress < 0 ? 0 : progress;
    progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

    // Draw the progress bar onto the view.
    QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                       &progressBarOption, painter);
}

}
