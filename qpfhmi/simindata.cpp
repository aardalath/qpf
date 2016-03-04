/******************************************************************************
 * File:    simindata.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.simindata
 *
 * Version: 1.0
 *
 * Date:    2015/09/01
 *
 * Copyright (C) 2015 J C Gonzalez
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Implement SimInData class
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

#include "simindata.h"
#include "hmipxy.h"

#include <QDebug>
#include <QFile>

namespace QPF {

SimInData::SimInData(QObject *parent) :
    QObject(parent), hmiNode(0), timerMultInData(0)
{
    multInDataContentValues.clear();
}

//----------------------------------------------------------------------
// Method: setHmiNode
// Sets HMI node object pointer
//----------------------------------------------------------------------
void SimInData::setHmiNode(HMIProxy * hmiObj)
{
    hmiNode = hmiObj;
}

//----------------------------------------------------------------------
// Method: sendInData
// Send sample InData message to Event Manager
//----------------------------------------------------------------------
void SimInData::sendInData(QString start, QString end,
                           QString prodId, QString prodType, QString prodVer,
                           QString status,
                           QString url)
{
    QString instrument = prodId.left(prodId.indexOf("_"));

    ProductMetadata m;
    m.startTime      = start.toStdString();
    m.endTime        = end.toStdString();
    m.instrument     = instrument.toStdString();
    m.productId      = prodId.toStdString();
    m.productType    = prodType.toStdString();
    m.productVersion = prodVer.toStdString();
    m.productStatus  = status.toStdString();
    m.productSize    = 12345678;
    m.url            = url.toStdString();

    qDebug() << "Trying to send InData";
    hmiNode->sendInData(m);
    qDebug() << " >> back from hmiNode->sendInData";
}

//----------------------------------------------------------------------
// Method: sendMultInData
// Prepare a set of sample InData messages to be sent to Event Manager
//----------------------------------------------------------------------
bool SimInData::sendMultInData(QDateTime start, QDateTime end,
                               QStringList prodTypes, QStringList prodStat,
                               int step, int freq)
{
    // Avoid execution when a previous set of messages is
    // still being sent
    if (timerMultInData != 0) { return false; }

    QDateTime startDateTime = start;
    QDateTime lastDateTime = end;
    QDateTime endDateTime = startDateTime;

    multInDataContentValues.clear();
    bool stillInPeriod = true;

    while (startDateTime < lastDateTime) {

        // Loop on Product Types
        for (int j = 0; (j < prodTypes.count() && stillInPeriod); ++j) {

            QString prodType = prodTypes.at(j);

            // Loop on Product Status
            for (int k = 0; (k < prodStat.count() && stillInPeriod); ++k) {

                QString prodStatus = prodStat.at(k);

                if (startDateTime < lastDateTime) {
                    // Compute endTime
                    endDateTime = startDateTime.addSecs(step * 3600 - 1);

                    // Build product id.
                    QString sDateTime(startDateTime.toString("yyyyMMddTHHmmss"));
                    QString eDateTime(endDateTime.toString("yyyyMMddTHHmmss"));
                    QString prodId = QString("%1_%2_%3-%4_%5")
                            .arg("EUCL").arg(prodType)
                            .arg(sDateTime).arg(eDateTime).arg("10");
                    QString instrument = prodType.left(prodType.indexOf("_"));

                    // Set new content for InData Message
                    ProductMetadata m;

                    m.startTime      = sDateTime.toStdString();
                    m.endTime        = eDateTime.toStdString();
                    m.instrument     = instrument.toStdString();
                    m.productId      = prodId.toStdString();
                    m.productType    = prodType.toStdString();
                    m.productVersion = "1";
                    m.productStatus  = prodStatus.toStdString();
                    m.productSize    = 12345678;
                    m.url            = "http://euclid.esa.int/data/" + prodId.toStdString() + ".zip";

                    // Append to InData message list
                    multInDataContentValues.append(m);

                    // Step startTime
                    startDateTime = endDateTime.addSecs(1);
                } else {
                    stillInPeriod = false;
                    break;
                }

            }

        }

        if (startDateTime == endDateTime) { return false; }

    }

    bool retVal = false;

    if (multInDataContentValues.size() > 0) {
        // Prepare sending timer
        timerMultInData = new QTimer(this);
        connect(timerMultInData, SIGNAL(timeout()),
                this, SLOT(actualSendMultInData()));
        timerMultInData->start(freq * 1000);
        retVal = true;
    }

    return retVal;
}

//----------------------------------------------------------------------
// Method: sendInDataFromFile
// Send a set of sample InData messages to Event Manager, according to
// a user-defined file
//----------------------------------------------------------------------
bool SimInData::sendInDataFromFile(QString fileInDataParams)
{
    if (fileInDataParams.isEmpty()) { return false; }

    QFile file(fileInDataParams);
    if(!file.open(QIODevice::ReadOnly)) { return false; }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");

        // Build product id.
        int     offset   = fields.at(0).toInt();
        QString sDateTime = fields.at(1);
        QString eDateTime = fields.at(1);
        QString prodType  = fields.at(3);
        QString productStatus = fields.at(4);;
        QString prodId    = QString("%1_%2_%3-%4_%5")
                            .arg("EUCL").arg(prodType)
                            .arg(sDateTime).arg(eDateTime).arg("10");
        QString instrument = prodType.left(prodType.indexOf("_"));

        // Set new content for InData Message
        ProductMetadata m;

        m.startTime      = sDateTime.toStdString();
        m.endTime        = eDateTime.toStdString();
        m.instrument     = instrument.toStdString();
        m.productId      = prodId.toStdString();
        m.productType    = prodType.toStdString();
        m.productVersion = "1.0";
        m.productStatus  = productStatus.toStdString();
        m.productSize    = 12345678;
        m.url            = ("http://euclid-data.esa.int/data/" + prodId + ".bin").toStdString();

        // Append to InData message list
        multInDataContentValues.append(m);

        // Append to InData message list
        QTimer::singleShot(offset * 1000, Qt::PreciseTimer,
                           this, SLOT(actualSendInDataFromFile()));
    }

    file.close();

    bool retVal = false;

    if (multInDataContentValues.size() > 0) { retVal = true; }

    return retVal;
}

//----------------------------------------------------------------------
// Method: actualSendMultInData
// Send a new InData messages to Event Manager
//----------------------------------------------------------------------
void SimInData::actualSendMultInData()
{
    // Build and send message from the list
    hmiNode->sendInData(multInDataContentValues.takeFirst());

    emit simInDataSent(multInDataContentValues.count());

    // If the list is empty, stop the timer and delete it
    if (multInDataContentValues.isEmpty()) {
        timerMultInData->stop();
        delete timerMultInData;
        timerMultInData = 0;
        emit endOfInData();
    }
}

//----------------------------------------------------------------------
// Method: actualSendInDataFromFile
// Send a set of sample InData messages to Event Manager, as specified
// in a used-created file
//----------------------------------------------------------------------
void SimInData::actualSendInDataFromFile()
{
    // Build and send message from the list
    hmiNode->sendInData(multInDataContentValues.takeFirst());

    emit simInDataSent(multInDataContentValues.count());

    // If the list is empty, stop the timer and delete it
    if (multInDataContentValues.isEmpty()) {
        emit endOfInData();
    }
}

void SimInData::stopSendingInData()
{
    multInDataContentValues.clear();
    if (timerMultInData != 0) {
        timerMultInData->stop();
        delete timerMultInData;
        timerMultInData = 0;
    }
}

}
