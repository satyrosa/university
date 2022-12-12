#ifndef FORSUB_H
#define FORSUB_H

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
  class ForSub;
}

class ForSub : public QDialog
{
  Q_OBJECT

public:
  explicit ForSub(QWidget *parent = nullptr);
  ~ForSub();

private slots:
  void on_pushButton_clicked();

private:
  Ui::ForSub *ui;
};

#endif // FORSUB_H
