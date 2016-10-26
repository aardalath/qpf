#ifndef HMITYPES_H
#define HMITYPES_H

#include <QMap>
#include <QString>

namespace QPF {

struct QUserDefTool {
    QString name;
    QString desc;
    QString exe;
    QString args;
    QStringList prod_types;
};

typedef QMap<QString, QUserDefTool> MapOfUserDefTools;

}

#endif // HMITYPES_H
