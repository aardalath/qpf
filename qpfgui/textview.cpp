#include <QtWidgets>

#include "textview.h"

TextView::TextView()
{
    setAttribute(Qt::WA_DeleteOnClose);
}

bool TextView::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool TextView::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QPF"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << getTextEditor()->toPlainText();
    QApplication::restoreOverrideCursor();

    return true;
}

void TextView::setLogName(QString n)
{
    name = n;
    setWindowTitle(n);

}

QString TextView::logName()
{
    return name;
}

void TextView::closeEvent(QCloseEvent *event)
{
    event->accept();
}

