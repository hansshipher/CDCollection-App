#ifndef ADDARTISTWNDW_H
#define ADDARTISTWNDW_H

#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QtSql>
#include <QMessageBox>

class AddArtistWndw : public QWidget
{
    Q_OBJECT
public:
    explicit AddArtistWndw(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("Add Artist");
        setFixedSize(300, 200); // фиксированный размер окна

        QVBoxLayout *vl = new QVBoxLayout(this);

        QLabel *nameLabel = new QLabel("Artist's name:");
        QLineEdit *name = new QLineEdit;

        QLabel *countryLabel = new QLabel("Artist's country:");
        QLineEdit *country = new QLineEdit;

        QPushButton *saveBtn = new QPushButton("Save");

        vl->addWidget(nameLabel);
        vl->addWidget(name);
        vl->addWidget(countryLabel);
        vl->addWidget(country);
        vl->addWidget(saveBtn);

        setLayout(vl);

        connect(saveBtn, &QPushButton::clicked, this, [=]() {
            QString artistName = name->text().trimmed();
            QString artistCountry = country->text().trimmed();

            if (artistName.isEmpty() || artistCountry.isEmpty()) {
                QMessageBox::warning(this, "Input Error", "Please enter both name and country.");
                return;
            }

            QSqlQuery query;
            query.prepare("INSERT INTO artist (name, country) VALUES (:name, :country)");
            query.bindValue(":name", artistName);
            query.bindValue(":country", artistCountry);

            if (!query.exec()) {
                QMessageBox::critical(this, "Database Error", query.lastError().text());
                return;
            }

            QMessageBox::information(this, "Success", "Artist saved successfully!");
            close(); // закрываем окно после успешного сохранения
        });
    }
};

#endif // ADDARTISTWNDW_H
