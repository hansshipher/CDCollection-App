#ifndef ADDCDWNDW_H
#define ADDCDWNDW_H

#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QtSql>
#include <QComboBox>
#include <QMessageBox>

class addCDWndw : public QWidget
{
    Q_OBJECT
public:
    explicit addCDWndw(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("Add CD");
        setFixedSize(300, 250);

        QVBoxLayout *vl = new QVBoxLayout(this);

        QLabel *artistLabel = new QLabel("Select artist:");
        QComboBox *artistCombo = new QComboBox;

        // Загружаем всех артистов из базы
        QSqlQuery query("SELECT artist_id, name FROM artist");
        while(query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            artistCombo->addItem(name, id);
        }

        QLabel *titleLabel = new QLabel("CD Title:");
        QLineEdit *titleEdit = new QLineEdit;

        QLabel *yearLabel = new QLabel("Year:");
        QLineEdit *yearEdit = new QLineEdit;

        QPushButton *saveBtn = new QPushButton("Save");

        vl->addWidget(artistLabel);
        vl->addWidget(artistCombo);
        vl->addWidget(titleLabel);
        vl->addWidget(titleEdit);
        vl->addWidget(yearLabel);
        vl->addWidget(yearEdit);
        vl->addWidget(saveBtn);

        setLayout(vl);

        connect(saveBtn, &QPushButton::clicked, this, [=]() {
            QString title = titleEdit->text().trimmed();
            int year = yearEdit->text().toInt();
            int artistId = artistCombo->currentData().toInt();

            if(title.isEmpty() || year == 0) {
                QMessageBox::warning(this, "Input Error", "Please enter valid CD title and year.");
                return;
            }

            QSqlQuery query;
            query.prepare("INSERT INTO cd (artist_id, cd_title, year) VALUES (:artist_id, :title, :year)");
            query.bindValue(":artist_id", artistId);
            query.bindValue(":title", title);
            query.bindValue(":year", year);

            if(!query.exec()) {
                QMessageBox::critical(this, "Database Error", query.lastError().text());
                return;
            }

            QMessageBox::information(this, "Success", "CD saved successfully!");
            close();
        });
    }
};

#endif // ADDCDWNDW_H
