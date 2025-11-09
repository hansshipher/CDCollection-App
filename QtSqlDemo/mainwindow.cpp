#include "mainwindow.h"
#include "addArtistWndw.h"
#include "addCDWndw.h"
#include "addTrackWndw.h"
#include "./ui_mainwindow.h"

#include <QtSql>
#include <QDebug>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    this->setWindowTitle("Database app");
    QVBoxLayout *vl = new QVBoxLayout;
    QPushButton *add_a = new QPushButton("Add artist");
    QPushButton *add_c = new QPushButton("Add CD(album)");
    QPushButton *add_t = new QPushButton("Add track");

    vl->addWidget(add_a);
    vl->addWidget(add_c);
    vl->addWidget(add_t);

    central->setLayout(vl);
    this->adjustSize();

    connect(add_a, &QPushButton::clicked, this, [this]() {
        AddArtistWndw *win = new AddArtistWndw;
        win->show();
    });
    connect(add_c, &QPushButton::clicked, this, [this]() {
        addCDWndw *win = new addCDWndw;
        win->show();
    });
    connect(add_t, &QPushButton::clicked, this, [this]() {
        addTrackWndw *win = new addTrackWndw;
        win->show();
    });


    QSqlQuery query_a("CREATE TABLE IF NOT EXISTS artist("
                      "artist_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT, "
                      "country TEXT"
                      ");");
    if(query_a.lastError().isValid())
        qDebug() << "Error creation of table_a: " << query_a.lastError().text();
    QSqlQuery query_c("CREATE TABLE IF NOT EXISTS cd("
                      "cd_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "artist_id INTEGER, "
                      "cd_title TEXT, "
                      "year INTEGER, "
                      "FOREIGN KEY(artist_id) REFERENCES artist(artist_id)"
                      ");");
    if(query_c.lastError().isValid())
        qDebug() << "Error creation of table_c: " << query_c.lastError().text();
    QSqlQuery query_t("CREATE TABLE IF NOT EXISTS track("
                      "track_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "cd_id INTEGER, "
                      "number INTEGER, "
                      "t_title TEXT, "
                      "duration INTEGER, "
                      "FOREIGN KEY(cd_id) REFERENCES cd(cd_id)"
                      ");");
    if(query_t.lastError().isValid())
        qDebug() << "Error creation of table_t: " << query_t.lastError().text();
    QSqlQuery query("SELECT * FROM artist");
    while (query.next()) {
        int id = query.value("artist_id").toInt();
        QString name = query.value("name").toString();
        QString country = query.value("country").toString();
        qDebug() << id << name << country;
    }

// debug DB info
    QPushButton *showTablesBtn = new QPushButton("Show Tables");
    vl->addWidget(showTablesBtn);

    connect(showTablesBtn, &QPushButton::clicked, this, [=]() {
        qDebug() << "---- ARTIST TABLE ----";
        QSqlQuery queryA("SELECT * FROM artist");
        while (queryA.next()) {
            int id = queryA.value("artist_id").toInt();
            QString name = queryA.value("name").toString();
            QString country = queryA.value("country").toString();
            qDebug() << id << name << country;
        }

        qDebug() << "---- CD TABLE ----";
        QSqlQuery queryC("SELECT * FROM cd");
        while (queryC.next()) {
            int id = queryC.value("cd_id").toInt();
            int artistId = queryC.value("artist_id").toInt();
            QString title = queryC.value("cd_title").toString();
            int year = queryC.value("year").toInt();
            qDebug() << id << artistId << title << year;
        }

        qDebug() << "---- TRACK TABLE ----";
        QSqlQuery queryT("SELECT * FROM track");
        while (queryT.next()) {
            int id = queryT.value("track_id").toInt();
            int cdId = queryT.value("cd_id").toInt();
            int number = queryT.value("number").toInt();
            QString title = queryT.value("t_title").toString();
            int duration = queryT.value("duration").toInt();
            qDebug() << id << cdId << number << title << duration;
        }
    });

    QPushButton *btnStats = new QPushButton("Show CD stats");
    vl->addWidget(btnStats);

    QTextEdit *statsOutput = new QTextEdit;
    statsOutput->setReadOnly(true);
    vl->addWidget(statsOutput);

    connect(btnStats, &QPushButton::clicked, this, [=]() {
        statsOutput->clear();

        QSqlQuery query1("SELECT artist_id, COUNT(*) AS cd_count FROM cd GROUP BY artist_id;");
        statsOutput->append("CD count by artist:");
        while(query1.next()) {
            int artistId = query1.value("artist_id").toInt();
            int cdCount = query1.value("cd_count").toInt();
            statsOutput->append(QString("Artist ID %1: %2 CDs").arg(artistId).arg(cdCount));
        }

        QSqlQuery query2("SELECT cd_title, COUNT(*) AS title_count FROM cd GROUP BY cd_title;");
        statsOutput->append("\nCD count by title:");
        while(query2.next()) {
            QString title = query2.value("cd_title").toString();
            int count = query2.value("title_count").toInt();
            statsOutput->append(QString("%1: %2 times").arg(title).arg(count));
        }

        QSqlQuery query3("SELECT year, COUNT(*) AS year_count FROM cd GROUP BY year;");
        statsOutput->append("\nCD count by year:");
        while(query3.next()) {
            int year = query3.value("year").toInt();
            int count = query3.value("year_count").toInt();
            statsOutput->append(QString("%1: %2 CDs").arg(year).arg(count));
        }
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
