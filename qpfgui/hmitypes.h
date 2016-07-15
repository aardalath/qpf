#ifndef HMITYPES_H
#define HMITYPES_H

#include <QMap>
#include <QString>

namespace QPF {

struct UserDefTool {
    QString name;
    QString desc;
    QString exe;
    QString args;
    QStringList prod_types;
};

typedef QMap<QString, UserDefTool> MapOfUserDefTools;

}

#endif // HMITYPES_H
