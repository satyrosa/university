#include "reports.h"
#include "forsub.h"
#include "forizd.h"
#include "ui_reports.h"

#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QString>
#include <QSqlQuery>

Reports::Reports(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Reports)
{
  ui->setupUi(this);
  QSqlQueryModel *setquery1 = new QSqlQueryModel;
  QString sql0 = QString("SELECT subs.surname, subs.name, subs.address, izd.name_izd, subscribes.sum FROM subscribes"
                         " JOIN subs ON subscribes.idsubs=subs.idsubs "
                         "JOIN izd ON subscribes.idizd=izd.idizd");
  setquery1->setQuery(sql0);
  ui->repView->setModel(setquery1);
}

Reports::~Reports()
{
  delete ui;
}


void Reports::on_forsubs_clicked()
{
  ForSub window;
  window.setModal(true);
  window.exec();
}

void Reports::on_forizd_clicked()
{
  ForIzd window;
  window.setModal(true);
  window.exec();
}
