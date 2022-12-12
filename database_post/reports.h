#ifndef REPORTS_H
#define REPORTS_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
  class Reports;
}

class Reports : public QDialog
{
  Q_OBJECT
public:
  explicit Reports(QWidget *parent = nullptr);
  ~Reports();

private slots:
  void on_forsubs_clicked();

  void on_forizd_clicked();

private:
  Ui::Reports *ui;
  QSqlDatabase db;
  QSqlTableModel* model;
  QSqlTableModel* model2;
};


#endif // REPORTS_H
