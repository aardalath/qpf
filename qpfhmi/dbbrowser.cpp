#include "dbbrowser.h"
#include "ui_dbbrowser.h"
#include "config.h"

DBBrowser::DBBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBBrowser)
{
    ui->setupUi(this);

    if (QSqlDatabase::connectionNames().isEmpty()) {
        QString databaseName ( QPF::Configuration::DBName.c_str() );
        QString userName     ( QPF::Configuration::DBUser.c_str() );
        QString password     ( QPF::Configuration::DBPwd.c_str() );
        QString hostName     ( QPF::Configuration::DBHost.c_str() );
        QString port         ( QPF::Configuration::DBPort.c_str() );

        Browser::DBConnection connection = { "QPSQL",        // .driverName
                                             databaseName,
                                             userName,
                                             password,
                                             hostName,
                                             port.toInt() };
        ui->browser->addConnection(connection);
    }
}

DBBrowser::~DBBrowser()
{
    delete ui;
}
