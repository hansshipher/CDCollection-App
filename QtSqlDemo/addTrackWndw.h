#ifndef ADDTRACKWNDW_H
#define ADDTRACKWNDW_H

#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QtSql>
#include <QComboBox>
#include <QMessageBox>

class addTrackWndw : public QWidget
{
    Q_OBJECT
public:
    explicit addTrackWndw(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("Add Track");
        setFixedSize(300, 300);

        QVBoxLayout *vl = new QVBoxLayout(this);

        QLabel *cdLabel = new QLabel("Select CD:");
        QComboBox *cdCombo = new QComboBox;

        // Загружаем все CD из базы
        QSqlQuery query("SELECT cd_id, cd_title FROM cd");
        while(query.next()) {
            int id = query.value(0).toInt();
            QString title = query.value(1).toString();
            cdCombo->addItem(title, id);
        }

        QLabel *trackTitleLabel = new QLabel("Track title:");
        QLineEdit *trackTitleEdit = new QLineEdit;

        QLabel *durationLabel = new QLabel("Duration (seconds):");
        QLineEdit *durationEdit = new QLineEdit;

        QPushButton *saveBtn = new QPushButton("Save");

        vl->addWidget(cdLabel);
        vl->addWidget(cdCombo);
        vl->addWidget(trackTitleLabel);
        vl->addWidget(trackTitleEdit);
        vl->addWidget(durationLabel);
        vl->addWidget(durationEdit);
        vl->addWidget(saveBtn);

        setLayout(vl);

        connect(saveBtn, &QPushButton::clicked, this, [=]() {
            int cdId = cdCombo->currentData().toInt();
            QString trackTitle = trackTitleEdit->text().trimmed();
            int duration = durationEdit->text().toInt();

            if(trackTitle.isEmpty() || duration == 0) {
                QMessageBox::warning(this, "Input Error", "Please enter valid track info.");
                return;
            }

            QSqlQuery queryNum;
            queryNum.prepare("SELECT MAX(number) FROM track WHERE cd_id = :cd_id");
            queryNum.bindValue(":cd_id", cdId);
            queryNum.exec();
            int nextNumber = 1;
            if(queryNum.next() && !queryNum.value(0).isNull()) {
                nextNumber = queryNum.value(0).toInt() + 1;
            }

            QSqlQuery query;
            query.prepare("INSERT INTO track (cd_id, number, t_title, duration) "
                          "VALUES (:cd_id, :number, :title, :duration)");
            query.bindValue(":cd_id", cdId);
            query.bindValue(":number", nextNumber);
            query.bindValue(":title", trackTitle);
            query.bindValue(":duration", duration);

            if(!query.exec()) {
                QMessageBox::critical(this, "Database Error", query.lastError().text());
                return;
            }

            QMessageBox::information(this, "Success", "Track saved successfully!");
            close();
        });

    }
};

#endif // ADDTRACKWNDW_H
