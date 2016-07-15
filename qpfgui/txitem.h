#ifndef TXITEM_H
#define TXITEM_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class TxItem;
}

namespace QPF {

class TxItem
{
public:
    TxItem();

    enum Mode { Empty, Line, Source, Target };
    enum Orientation { FromLeftToRight, FromRightToLeft };

    void setMode(Mode m);
    void setOrientation(Orientation o);
    void setMsgName(QString n);
    void setContent(QJsonObject o);

    QSize sizeHint() const;
    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette) const;

private:
    Ui::TxItem *ui;
    Mode mode;
    Orientation orientation;
    QString name;
    QJsonObject content;
};

}

#endif // TXITEM_H
