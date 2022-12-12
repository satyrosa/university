#include "forsub.h"
#include "ui_forsub.h"

#include <QFile>

ForSub::ForSub(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ForSub)
{
  ui->setupUi(this);
}

ForSub::~ForSub()
{
  delete ui;
}

void ForSub::on_pushButton_clicked()
{
    QString sname = ui->sub_name->text(); //Забираем данные из ledit
    QSqlQueryModel *setquery1 = new QSqlQueryModel;                             // Выводим предварительный отчет на экран
    QString sql0 = QString("SELECT subs.surname, subs.name, subs.address, izd.name_izd, subscribes.sum FROM subscribes"
                           " JOIN subs ON subscribes.idsubs=subs.idsubs "
                           "JOIN izd ON subscribes.idizd=izd.idizd"
                           " WHERE subs.surname = '%1'").arg(sname);
    setquery1->setQuery(sql0);
    ui->subReport->setModel(setquery1);

    int sum = 0;
    QString subreport=sname+"_repot.txt"; // Сщздаем файл отчета
    QSqlQuery queryMain;
    if(!queryMain.exec("SELECT subs.name, subs.address FROM subs"
                       " WHERE subs.surname = '"+sname+"'"))
      {qDebug() << queryMain.lastError().text();
       return;
      }

    queryMain.first();
    QFile file( subreport );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << "             SATYROSAPOST\n"
                  "-----------------------------------------\n" <<
                  sname << " " << queryMain.value(0).toString() << endl <<
                  queryMain.value(1).toString() << endl;
        if(!queryMain.exec("SELECT izd.name_izd, izd.cost, subscribes.period, subscribes.sum FROM subscribes"
                           " JOIN subs ON subscribes.idsubs=subs.idsubs "
                           "JOIN izd ON subscribes.idizd=izd.idizd"
                           " WHERE subs.surname = '"+sname+"'"))
          {qDebug() << queryMain.lastError().text();
           return;
          }
        else{
            while(queryMain.next())
              {
                stream << queryMain.value(0).toString() + "  " + queryMain.value(1).toString() + "  " + queryMain.value(2).toString() + "  " + queryMain.value(3).toString() << endl;
                sum+=queryMain.value(2).toInt();
              }
          }
        stream << "-----------------------------------------\n" <<
                  "                               SUM: " << sum << endl;
    }
}
