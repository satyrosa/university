#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_reports_clicked();

  void on_pushButton_3_clicked();

private:
  Ui::MainWindow *ui;
  QSqlDatabase db;
  QSqlTableModel* model;
};
#endif // MAINWINDOW_H
