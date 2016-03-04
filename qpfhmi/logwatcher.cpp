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
#include <memory>

namespace QPF {

LogWatcher::LogWatcher(TextView * txtVw, int lines)
  : textView(txtVw), bytesRead(0), maxNumLines(lines)
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

  qint64 newBytes = f.size() - bytesRead;
  f.seek(bytesRead);
  QByteArray c;
  c = f.read(newBytes);
  bytesRead += c.size();

  QTextCursor cursor = textView->textCursor();

  qint64 linesToRemove = textView->blockCount() > maxNumLines;
  if (linesToRemove > 0) {
      cursor.movePosition(QTextCursor::Start);
      cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, linesToRemove);
      cursor.select(QTextCursor::LineUnderCursor);
      cursor.removeSelectedText();
  }

  cursor.movePosition(QTextCursor::End);
  cursor.insertText(QString(c));
  textView->setTextCursor(cursor);
  textView->ensureCursorVisible();

  emit logUpdated();
}

}
