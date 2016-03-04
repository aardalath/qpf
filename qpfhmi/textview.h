#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QtWidgets>

class TextView : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextView();

    bool saveAs();
    bool saveFile(const QString &fileName);
    void setLogName(QString n);
    QString logName();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    QString name;
};

#endif // TEXTVIEW_H

