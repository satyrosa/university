#ifndef ADDSUB_H
#define ADDSUB_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
  class AddSub;
}

class AddSub : public QDialog
{
  Q_OBJECT

public:
  explicit AddSub(QWidget *parent = nullptr);
  ~AddSub();

private slots:
  void on_accept_but_clicked();

private:
  Ui::AddSub *ui;
  QSqlDatabase db;
  QSqlTableModel* model;
};

#endif // ADDSUB_H
