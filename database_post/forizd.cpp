#include "forizd.h"
#include "ui_forizd.h"

#include <QFile>

ForIzd::ForIzd(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ForIzd)
{
  ui->setupUi(this);

  model = new QSqlTableModel(this, db);//Подключаем combox с доступными подписками
  model ->setTable("izd");
  model -> select();
  ui -> cb_izd->setModel(model);
  ui -> cb_izd->setModelColumn(1);
}

ForIzd::~ForIzd()
{
  delete ui;
}

void ForIzd::on_pushButton_clicked()
{
  int sum=0;
  QVariant izd = ui->cb_izd->currentText(); // забираем данные из cb
  QSqlQueryModel *setquery1 = new QSqlQueryModel;                                                 //Выводим предварительный отчет на экран
  QString sql0 = QString("SELECT izd.name_izd,subs.surname, subs.name, subs.address, subscribes.sum FROM subscribes"
                         " JOIN subs ON subscribes.idsubs=subs.idsubs "
                         "JOIN izd ON subscribes.idizd=izd.idizd"
                         " WHERE izd.name_izd = '%1'").arg(izd.toString());
  setquery1->setQuery(sql0);
  ui->izdView->setModel(setquery1);

  QString izdreport=izd.toString()+"_repot.txt";//Cоздаем файл отчета
  QSqlQuery queryMain;
  QFile file( izdreport );
  if ( file.open(QIODevice::ReadWrite) )
  {
      QTextStream stream( &file );
      stream << "             SATYROSAPOST\n"
                "-----------------------------------------\n" <<
                izd.toString() << endl;
      if(!queryMain.exec("SELECT subs.name, subs.surname, subscribes.period FROM subscribes"
                         " JOIN subs ON subscribes.idsubs=subs.idsubs "
                         "JOIN izd ON subscribes.idizd=izd.idizd"
                         " WHERE izd.name_izd = '"+izd.toString()+"'"))
        {qDebug() << queryMain.lastError().text();
         return;
        }
      else{
          while(queryMain.next())
            {
              stream << queryMain.value(0).toString() + "  " + queryMain.value(1).toString() + "  " + queryMain.value(2).toString() << endl;
              sum+=queryMain.value(2).toInt();
            }
        }
      stream << "-----------------------------------------\n" <<
                "          SUM: " << sum << endl;
  }
}
