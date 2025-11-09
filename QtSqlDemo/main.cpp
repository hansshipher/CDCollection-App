#include "mainwindow.h"

#include <QApplication>

#include <QtSql>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("CDCollection.db");
    if(!db.open()){
        qDebug() << "Error connection to data base" << db.lastError().text();
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
