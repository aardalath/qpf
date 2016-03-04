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
#include "config.h"
#include "filenamespec.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

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
    m.signature      = "XXXXXX";
    m.url            = url.toStdString();
    m.urlSpace       = ExternalSpace;

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

    FileNameSpec fs;
    std::string prodIdStd;

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
                    QString instrument = prodType.left(prodType.indexOf("_"));

                    prodIdStd = fs.buildProductId("EUC",
                                                  sDateTime.toStdString(),
                                                  eDateTime.toStdString(),
                                                  prodType.toStdString(),
                                                  "",
                                                  "01.00");
                    QString prodId = QString::fromStdString(prodIdStd);

                    // Set new content for InData Message
                    ProductMetadata m;

                    m.startTime      = sDateTime.toStdString();
                    m.endTime        = eDateTime.toStdString();
                    m.instrument     = instrument.toStdString();
                    m.productId      = prodIdStd;
                    m.productType    = prodType.toStdString();
                    m.productVersion = "1";
                    m.productStatus  = prodStatus.toStdString();
                    m.productSize    = 12345678;
                    m.signature      = "XXXXXXXXXXX";
                    m.url            = "http://euclid.esa.int/data/" + prodIdStd + ".zip";
                    m.urlSpace       = ExternalSpace;

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

    FileNameSpec fs;
    std::string prodIdStd;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");

        // Build product id.
        int     offset   = fields.at(0).toInt();
        QString sDateTime = fields.at(1);
        QString eDateTime = fields.at(1);
        QString prodType  = fields.at(3);
        QString instrument = prodType.left(prodType.indexOf("_"));
        QString productStatus = fields.at(4);;
        prodIdStd = fs.buildProductId("EUC",
                                      sDateTime.toStdString(),
                                      eDateTime.toStdString(),
                                      prodType.toStdString(),
                                      "",
                                      "01.00");
        QString prodId = QString::fromStdString(prodIdStd);

        // Set new content for InData Message
        ProductMetadata m;

        m.startTime      = sDateTime.toStdString();
        m.endTime        = eDateTime.toStdString();
        m.instrument     = instrument.toStdString();
        m.productId      = prodIdStd;
        m.productType    = prodType.toStdString();
        m.productVersion = "1.0";
        m.productStatus  = productStatus.toStdString();
        m.productSize    = 12345678;
        m.signature      = "XXXXXXXXXXXXX";
        m.url            = "http://euclid-data.esa.int/data/" + prodIdStd + ".bin";
        m.urlSpace       = ExternalSpace;

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
// Method: processInbox
// Send INDATA messages into the system, to process all the
// products in the Inbox directory selected
//----------------------------------------------------------------------
bool SimInData::processInbox(QString folder, QString & metadata)
{
    const QString MetaFile("METADATA.SDC");

    // Get metadata for images in inbox, or generate them
    QString fullMetaFile = folder + "/" + MetaFile;
    QFileInfo fsMeta(fullMetaFile);

    multInDataContentValues.clear();
    if (!fsMeta.exists()) { generateMetaFile(folder, fullMetaFile); }

    metadata = "";

    // Process entries in metafile
    if (multInDataContentValues.size() < 1) {
        metadata = readMetaFile(fullMetaFile);
    }

    // Send messages, every 10 s
    if (multInDataContentValues.size() > 0) {
        // Prepare sending timer
        timerMultInData = new QTimer(this);
        connect(timerMultInData, SIGNAL(timeout()),
                this, SLOT(actualSendMultInData()));
        timerMultInData->start(freq * 1000);
        return true;
    }

    return false;
}

//----------------------------------------------------------------------
// Method: readMetaFile
// Read metadata file from incoming products folder
//----------------------------------------------------------------------
QString SimInData::readMetaFile(const QString& metaFile)
{
    // Parse metafile
    std::ifstream metaJsonFile(metaFile.toStdString());
    if (!metaJsonFile.good()) {
        metaJsonFile.close();
        return QString();
    }
    Json::Reader reader;
    Json::Value v;
    reader.parse(metaJsonFile, v);
    metaJsonFile.close();

    Json::StyledWriter w;
    std::string mdStdStr = w.write(v);
    qDebug() << mdStdStr.c_str() << v.size();
    // Map into ProductList structure
    ProductList metadata;
    metadata.setData(v["metadata"]);

    // Add entries to list for sending messages
    for (ProductMetadata & m : metadata.productList) {
        multInDataContentValues.append(m);
    }

    return QString::fromStdString(mdStdStr);
}

//----------------------------------------------------------------------
// Method: generateMetaFile
// Generate metafile with metadata from product file names
//----------------------------------------------------------------------
void SimInData::generateMetaFile(const QString& folder, const QString& metaFile)
{
    // Geenrate list of all files in folder and child folders
    QStringList productList;
    traverseDirectory(folder, productList);

    // Now, parse filenames and generate metadata
    FileNameSpec fs;
    ProductList metadata;
    for (QString & file : productList) {

        FileNameSpec::FileNameComponents c = fs.parseFileName(file.toStdString());
        QFileInfo fi(file);

        // Set new content for InData Message
        ProductMetadata m;
        m.startTime      = c.dateStart;
        m.endTime        = c.dateEnd;
        m.creator        = "UNKNOWN";
        m.instrument     = c.instrument;
        m.productId      = c.productId;
        m.productType    = c.productType;
        m.productVersion = c.version;
        m.productStatus  = "OK";
        m.productSize    = fi.size();
        m.signature      = c.signature;
        m.url            = "file://" + file.toStdString();
        m.urlSpace       = UserSpace;

        metadata.productList.push_back(m);

        // Append to InData message list (so we do not need to
        // read generated metafile again)
        multInDataContentValues.append(m);
    }


    // Generate JSON metafile
    Json::StyledWriter writer;
    Json::Value v;
    v["metadata"] = metadata.getData();
    std::ofstream metaOut(metaFile.toStdString());
    metaOut << writer.write(v);
    metaOut.close();
}

//----------------------------------------------------------------------
// Method: traverseDirectory
// Generate (recursive) list of product files (actually .fits files) in
// folder
//----------------------------------------------------------------------
void SimInData::traverseDirectory(const QString& sDir, QStringList & files)
{
    QDir dir(sDir);
    FileNameSpec fs;
    for (QFileInfo & info : dir.entryInfoList()) {
        QString filePath = info.filePath();
        if (info.isDir()) {
            // recursive
            if ((info.fileName() != "..") && (info.fileName() != ".")) {
                traverseDirectory(filePath, files);
            }
        } else {
            // do something with the file here
            if (isValidExtension(info.suffix().toLower().toStdString())) {
                files << filePath;
            }
        }
    }

/*
    QFileInfoList list = dir.entryInfoList();
    for (int iList=0;iList<list.count();iList++)
    {
        QFileInfo info = list[iList];

        QString sFilePath = info.filePath();
        if (info.isDir())
        {
            // recursive
            if (info.fileName()!=".." && info.fileName()!=".")
            {
                RecurseDirectory(sFilePath);
            }
        }
        else
        {
            // Do something with the file here
        }
    }*/
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

//----------------------------------------------------------------------
// Method: setInjectionFrequency
// Set the frequency of injection of INDATA messages
//----------------------------------------------------------------------
void SimInData::setInjectionFrequency(int f)
{
    freq = f;
}

}
