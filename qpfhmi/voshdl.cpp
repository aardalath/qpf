/******************************************************************************
 * File:    voshdl.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.voshdl
 *
 * Version:  1.2
 *
 * Date:    2015/07/01
 *
 * Author:   J C Gonzalez
 *
 * Copyright (C) 2015-2017 Euclid SOC Team @ ESAC
 *_____________________________________________________________________________
 *
 * Topic: General Information
 *
 * Purpose:
 *   Provides object implementation for some declarations
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

#include "voshdl.h"

#include "config.h"
using Configuration::cfg;

#define C(x) (x).c_str()
#define QS(x) QString::fromStdString(x)

namespace QPF {

VOSpaceHandler::VOSpaceHandler(QObject *parent)
{
}

VOSpaceHandler::~VOSpaceHandler()
{
}

void VOSpaceHandler::uploadToFile(QString folder, QString file, QString content)
{
    QString transfer_url = QString::fromStdString(cfg.connectivity.vospace.url() +
                                                  "/servlet/transfers/async?PHASE=RUN");
    QString end_point("/service/data/");
        
    QString metadataTransfer("transfer_push_to_a.xml"); // Contains the path where to store the output file
    QString & toupload = file; // Name of the file to be uploaded in VOSpace

    QString user = QString::fromStdString(cfg.connectivity.vospace.user());
    QString txData = Tx_XML_File.arg(user).arg(folder).arg("pushToVoSpace");
}

void VOSpaceHandler::uploadFile(QString folder, QString file, QString localFile)
{
}

void VOSpaceHandler::downloadFromFile(QString folder, QString file, QString & content)
{
}

void VOSpaceHandler::downloadFile(QString folder, QString file, QString localFile)
{
}

const QString VOSpaceHandler::Tx_XML_File =
    "<vos:transfer xmlns:vos=\"http://www.ivoa.net/xml/VOSpace/v2.0\">"
    "    <vos:target>vos://esavo!vospace/%1/%2</vos:target>"
    "    <vos:direction>%3</vos:direction>"
    "    <vos:view uri=\"vos://esavo!vospace/core#fits\"/>"
    "    <vos:protocol uri=\"vos://esavo!vospace/core#httpput\"/>"
    "</vos:transfer>";

}
