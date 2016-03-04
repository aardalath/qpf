/******************************************************************************
 * File:    simindata.h
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
 *   Declare SimInData class
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
#ifndef SIMINDATA_H
#define SIMINDATA_H

#include "msgtypes.h"

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QStringList>


namespace QPF {

class HMIProxy;

class SimInData : public QObject
{
    Q_OBJECT
public:
    explicit SimInData(QObject *parent = 0);

signals:
    void simInDataSent(int);
    void endOfInData();

public slots:
    //----------------------------------------------------------------------
    // Method: sendInData
    // Send sample InData message to Event Manager
    //----------------------------------------------------------------------
    void setHmiNode(HMIProxy * hmiObj = 0);

    //----------------------------------------------------------------------
    // Method: sendInData
    // Send sample InData message to Event Manager
    //----------------------------------------------------------------------
    void sendInData(QString start, QString end,
                    QString prodId, QString prodType, QString prodVer,
                    QString status,
                    QString url);

    //----------------------------------------------------------------------
    // Method: sendMultInData
    // Prepare a set of sample InData messages to be sent to Event Manager
    //----------------------------------------------------------------------
    bool sendMultInData(QDateTime start, QDateTime end,
                        QStringList prodTypes, QStringList prodStat,
                        int step = 12, int freq = 60);

    //----------------------------------------------------------------------
    // Method: sendInDataFromFile
    // Send a set of sample InData messages to Event Manager, according to
    // a user-defined file
    //----------------------------------------------------------------------
    bool sendInDataFromFile(QString fileInDataParams);

    //----------------------------------------------------------------------
    // Method: processInbox
    // Send INDATA messages into the system, to process all the
    // products in the Inbox directory selected
    //----------------------------------------------------------------------
    bool processInbox(QString folder);

    //----------------------------------------------------------------------
    // Method: actualSendMultInData
    // Send a new InData messages to Event Manager
    //----------------------------------------------------------------------
    void actualSendMultInData();

    //----------------------------------------------------------------------
    // Method: actualSendInDataFromFile
    // Send a set of sample InData messages to Event Manager, as specified
    // in a used-created file
    //----------------------------------------------------------------------
    void actualSendInDataFromFile();

    //----------------------------------------------------------------------
    // Method: stopSendingInData
    // Stop sending messages and clears list of pending ones
    //----------------------------------------------------------------------
    void stopSendingInData();

private:
    //----------------------------------------------------------------------
    // Method: readMetaFile
    // Read metadata file from incoming products folder
    //----------------------------------------------------------------------
    void readMetaFile(const QString& metaFile);

    //----------------------------------------------------------------------
    // Method: generateMetaFile
    // Generate metafile with metadata from product file names
    //----------------------------------------------------------------------
    void generateMetaFile(const QString& folder, const QString& metaFile);

    //----------------------------------------------------------------------
    // Method: traverseDirectory
    // Generate (recursive) list of product files (actually .fits files) in
    // folder
    //----------------------------------------------------------------------
    void traverseDirectory(const QString& sDir, QStringList & files);

private:
    HMIProxy * hmiNode;

    QList<ProductMetadata> multInDataContentValues;
    QTimer * timerMultInData;

};

}

#endif // SIMINDATA_H
