#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

void usage()
{
    QString execName = QCoreApplication::arguments().at(0);
    std::cerr << "Usage: "
              << execName.toStdString()
              << " -c db://user:passwd@host:port/dbname"
              << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    const QStringList & args = QCoreApplication::arguments();
    const int & numOfArgs = args.count();

    if (numOfArgs > 4) {
        QString configStr("");
        QString sessionStr("");

        for (int i = 1; i < numOfArgs; ++i) {
            const QString & tok = args.at(i);
            if (tok == "-c") {
                configStr = args.at(i + 1);
                ++i;
            } else if (tok == "-s") {
                sessionStr = args.at(i + 1);
                ++i;
            }
        }

        if (configStr.isEmpty() || sessionStr.isEmpty()) {
            usage();
        }

        QPF::MainWindow w(configStr, sessionStr);
        w.show();
        return a.exec();

    } else {
        usage();
    }

    return EXIT_SUCCESS;
}
