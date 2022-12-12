#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addsub.h"
#include "reports.h"

#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QFile>

int cou = 0;
MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  db = QSqlDatabase::addDatabase("QMYSQL");// Подключаем датабазу
  db.setHostName("127.0.0.1");
  db.setDatabaseName("post");
  db.setUserName("root");
  db.setPassword("1234567");

  if(!db.open()){
      qDebug() << db.lastError().text();
      return;
    }
  else {
      qDebug() << "Success!";
    }

  QSqlQueryModel *setquery1 = new QSqlQueryModel;                                                                     //Выводим активный чек
  QString sql0 = QString("SELECT subs.surname, subs.name, subs.address, izd.name_izd, subscribes.sum FROM subscribes"
                         " JOIN subs ON subscribes.idsubs=subs.idsubs "
                         "JOIN izd ON subscribes.idizd=izd.idizd"
                         " WHERE paid = 0");
  setquery1->setQuery(sql0);
  ui->tableView->setModel(setquery1);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()// Переход к добавлению подписчика
{
  AddSub window;
  window.setModal(true);
  window.exec();

}

void MainWindow::on_pushButton_2_clicked()                                      //  Эта кнопка обновляет чек
{
  QSqlQueryModel *setquery1 = new QSqlQueryModel;
  QSqlQuery qTest;
  QString sql0 = QString("SELECT subs.surname, subs.name, subs.address, izd.name_izd, subscribes.sum FROM subscribes"
                         " JOIN subs ON subscribes.idsubs=subs.idsubs "
                         "JOIN izd ON subscribes.idizd=izd.idizd"
                         " WHERE paid = 0");
  qTest.exec(sql0);
  qDebug() << qTest.lastError().text();
  setquery1->setQuery(sql0);
  ui->tableView->setModel(setquery1);
}

void MainWindow::on_reports_clicked()//Переход к отчетам
{
  Reports window;
  window.setModal(true);
  window.exec();
}

void MainWindow::on_pushButton_3_clicked()//Печать чека
{
  int sum = 0;
  QString COUNT="Чек.txt";
  QSqlQuery queryMain, qq;
  if(!queryMain.exec("SELECT subs.surname, subs.name, subs.address FROM subscribes"
                     " JOIN subs ON subscribes.idsubs=subs.idsubs "
                     "JOIN izd ON subscribes.idizd=izd.idizd"
                     " WHERE paid = 0"))
    {qDebug() << queryMain.lastError().text();
     return;
    }

  queryMain.first();
  QFile file( COUNT );
  if ( file.open(QIODevice::ReadWrite) )
  {
      QTextStream stream( &file );
      stream << "             SATYROSAPOST\n"
                "-----------------------------------------\n" <<
                queryMain.value(2).toString() << " " << queryMain.value(0).toString() << endl <<
                queryMain.value(1).toString() << endl;
      if(!queryMain.exec("SELECT izd.name_izd, izd.cost, subscribes.period, subscribes.sum FROM subscribes"
                         " JOIN subs ON subscribes.idsubs=subs.idsubs "
                         "JOIN izd ON subscribes.idizd=izd.idizd"
                         " WHERE subscribes.paid = 0"))
        {qDebug() << queryMain.lastError().text();
         return;
        }
      else{
          while(queryMain.next())
            {
              stream << queryMain.value(0).toString() + "  " + queryMain.value(1).toString() + "  " + queryMain.value(2).toString() + "  " + queryMain.value(3).toString() << endl;
              sum+=queryMain.value(3).toInt();
              if(!qq.exec("UPDATE subscribes SET paid = 1")){qDebug() << qq.lastError().text();};
            }
        }
      stream << "-----------------------------------------\n" <<
                "                               SUM: " << sum << endl;
  }
}
