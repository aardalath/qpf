/******************************************************************************
 * File:    logwatcher.cpp
 *          This file is part of QLA Processing Framework
 *
 * Domain:  QPF.libQPF.logwatcher
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
 *   Implement LogWatcher class
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

#include "logwatcher.h"

#include <QDebug>

namespace QPF {

LogWatcher::LogWatcher(QPlainTextEdit * txtVw)
  : textView(txtVw), bytesRead(0)
{
  fsWatcher = new QFileSystemWatcher;
 
  connect(fsWatcher, SIGNAL(fileChanged(const QString &)),
	  this, SLOT(updateLogView(const QString &)));
}

void LogWatcher::setFile(QString s) 
{
  watchedFile = s;
  fsWatcher->addPath(s);
  updateLogView(s);
}

QString LogWatcher::getFile() 
{
  return watchedFile;
}

void LogWatcher::updateLogView(const QString & path)
{
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly)) { return; }

  char * c;
  qint64 newBytes = f.size() - bytesRead;
  c = new char[newBytes + 1];
  f.seek(bytesRead);
  bytesRead += f.read(c, newBytes);
  c[newBytes] = 0;

  QTextCursor cursor = textView->textCursor();
  cursor.insertText(QString(c));
  textView->ensureCursorVisible();
  //textView->appendPlainText(QString(c));
}

}
