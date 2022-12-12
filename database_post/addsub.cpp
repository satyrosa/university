#include "addsub.h"
#include "ui_addsub.h"

#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QString>
#include <QSqlQuery>

AddSub::AddSub(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddSub)
{
  ui->setupUi(this);

  model = new QSqlTableModel(this, db);//Подключаем combox с доступными подписками
  model ->setTable("izd");
  model -> select();
  ui -> subscribe->setModel(model);
  ui -> subscribe->setModelColumn(1);
}

AddSub::~AddSub()
{
  delete ui;
}

int checkSubs(QString n, QString sn, QString ads){
  QSqlQuery q;
  QString sql = QString("select * from subs where  surname='%1' and name='%2' and address='%3'").arg(sn, n, ads);
  qDebug() << q.exec(sql) << '\n' << q.lastError().text();
  qDebug() << q.size();
  return q.size();
}
void AddSub::on_accept_but_clicked()
{
    QString name = ui->name->text();            //Забираем данные из полей
    QString surn = ui->surname->text();
    QString address = ui->address->text();
    QString subs = ui->subscribe->currentText();
    QVariant time = ui->period->currentText();
    int sum=0, ids=0, idiz=0;

    QSqlQuery querySum = QSqlQuery(db);                                         //Вычисляем сумму подписки
    QString sql1 = QString("select cost from izd where name_izd='%1'").arg(subs);
    if (!querySum.exec(sql1)){qDebug() << querySum.lastError().text(); return;}
    else{
      querySum.first();
      sum = querySum.value(0).toInt();
      sum *=time.toInt();
      }

    if(checkSubs(name, surn, address)==0){                        //Проверяем наличие человека в базе
      QSqlQuery query;                                            //Если нет, добавляем в базу
      query.prepare("INSERT INTO subs(surname, name, address)"
                     "VALUES (:surname, :name, :address)");
      query.bindValue(":surname", surn);
      query.bindValue(":name", name);
      query.bindValue(":address", address);
      query.exec();
      }

    QSqlQuery qTemp;
    sql1 = QString("select idsubs from subs where name='%1'").arg(name);//Получаем  id подписчика
    if (!qTemp.exec(sql1)){qDebug() << qTemp.lastError().text();}
    else{
      qTemp.first();
      ids = qTemp.value(0).toInt();
      }
    sql1 = QString("select idizd from izd where name_izd='%1'").arg(subs);//Получаем id издания
    if (!qTemp.exec(sql1)){qDebug() << qTemp.lastError().text();}
    else{
      qTemp.first();
      idiz = qTemp.value(0).toInt();
      }

    QSqlQuery qS;                                                        //Заполняем таблицу подписок
    qS.prepare("INSERT INTO subscribes(idsubs, idizd, period, sum)"
                   "VALUES (:idsubs, :idizd, :period, :sum)");
    qS.bindValue(":idsubs", ids);
    qS.bindValue(":idizd", idiz);
    qS.bindValue(":period", time.toInt());
    qS.bindValue(":sum", sum);
    qS.exec();
}
