#ifndef FORIZD_H
#define FORIZD_H

#include <QDialog>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QString>
#include <QSqlQuery>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>


namespace Ui {
  class ForIzd;
}

class ForIzd : public QDialog
{
  Q_OBJECT

public:
  explicit ForIzd(QWidget *parent = nullptr);
  ~ForIzd();

private slots:
  void on_pushButton_clicked();

private:
  Ui::ForIzd *ui;
  QSqlDatabase db;
  QSqlTableModel* model;
};

#endif // FORIZD_H
