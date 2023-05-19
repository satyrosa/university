#include "mainwindow.h"

#include "ui_mainwindow.h"


#include <QThread>
#include <QRandomGenerator>
#include <QLabel>
#include <QLayout>
#include <QFile>

const int nPoints = 100;
QVector<QPointF> pnts;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  //настройка графика
//  создаем
  ui->plot->setFixedSize(500, 500);
  ui->plot -> setTitle("PCA");
//
//  Клетки
  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->setMajorPen(Qt::lightGray, 2);
  grid->attach(ui->plot);
//
//  Прокручивание размера
  QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(ui->plot->canvas());
  magnifier->setMouseButton(Qt::MidButton);
//
//  Перемещение по полю
  QwtPlotPanner *panner = new QwtPlotPanner(ui->plot->canvas());
  panner->setMouseButton(Qt::RightButton);
//


//  Отслеживание координт под курсором
  QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                            QwtPlotPicker::CrossRubberBand,
                                            QwtPicker::ActiveOnly,
                                            ui->plot->canvas());

  picker->setRubberBandPen(QColor(Qt::gray));
  picker->setTrackerPen(QColor(Qt::black));
  picker->setStateMachine(new QwtPickerDragPointMachine());
}

MainWindow::~MainWindow()
{
  delete ui;
}

QVector<QPointF> MainWindow::on_pushButton_clicked()
{

  // Генерация точек
//  по факту облако точек-это кривая, но благодаря setStyle(QwtPlotCurve::Dots) они не соединяются линиями
//  здесь подготавливаю поле для отрисовки. Настраиваю цвет, толщину и тд
  QwtPlotCurve *curve = new QwtPlotCurve();
  curve->setStyle(QwtPlotCurve::Dots);
  curve->setPen(Qt::blue, 6, Qt::PenStyle(Qt::RoundJoin));
//

  const auto random = QRandomGenerator::global();
  int x, y;//координаты будущей точки
  QString log;//Строка для отслеживания состояний алгоритма

//Т.к. я не смогла разобраться, как добавлять в QTextBrowser строки по ходу программы
//  я просто дополняю каждый раз одну с ту же строку log и обновляю содержимое QTB
  log = "Генерируем облако...";
  ui->textbr->setText(log);
//
//В цикле добавляю точки в список. Что бы облако выглядело "направленным", добавляю
//зависимость одной коорд от другой
  for (int i = 0;i<nPoints;++i) {
     QThread::msleep(50);
     x = random->bounded(1000);
     y = 3*x+random->bounded(1000);
     pnts.push_back(QPointF(x, y));
    //Уведомляю пользователя о добавлении точки
     log += "\nДобавлена точка: " + QString::number(x) + " " + QString::number(y);
     ui->textbr->setText(log);
     ui->textbr->update();
     QApplication::processEvents();
   }
//И наконец отрисовываю их
  curve->setSamples(pnts);
  curve->attach(ui->plot);
  QApplication::processEvents();

//В перспективе хочу добавить отрисовку точек от руки, поэтому возращаю массив
  return pnts;
}

void MainWindow::on_pushButton_2_clicked()
{
//  Сам метод. Здесь с-ковариация, сov-ковариационная матрица. MX MY - матожидания
//  DX DY - Дисперисии. Здесь много лишних переменных, но без них я бы запуталась
  double c = 0;
  double cov[2][2];
  double MX = 0, MY = 0;
  double DX = 0, DY = 0;

  QString log;

  log+="\nСчитаем Матожидание:";
  ui->textbr->setText(log);
  ui->textbr->update();
  QApplication::processEvents();

  for(int i = 0; i < nPoints; ++i)
    {
      MX += pnts[i].x();
      MY += pnts[i].y();
    }
  MX = MX/nPoints;
  MY = MY/nPoints;
//Посчитали матожидания как среднее значение
  log = log+"\nMX="+QString::number(MX)+"\nMY="+QString::number(MY);
  ui->textbr->setText(log);
  ui->textbr->update();
  log+="\nСчитаем Дисперсию::";
  ui->textbr->setText(log);
  ui->textbr->update();
  QApplication::processEvents();

  for(int i = 0; i < nPoints; ++i)
    {
      DX += (pnts[i].x()-MX)*(pnts[i].x()-MX);
      DY += (pnts[i].y()-MY)*(pnts[i].y()-MY);
    }
  DX = DX / (nPoints-1);
  DY = DY / (nPoints-1);
//И так же дисперсию по формулам. Проверяла на онлайн калькуляторах, все верно
  log = log + "\nDX="+QString::number(DX)+"\nDY="+QString::number(DY);
  ui->textbr->setText(log);
  ui->textbr->update();
  QApplication::processEvents();

  for(int i = 0; i< nPoints; ++i)
    {
      c += (pnts[i].x() - MX)*(pnts[i].y()-MY);
    }
  c = c / (nPoints-1);
  cov[0][0] = DX;
  cov[1][1] = DY;
  cov[0][1] = c;
  cov[1][0] = c;
//Здесь двумерный массив нужен только для вывода матрицы и можно было бы обойтись без него

  log = log + "\nКовариационная матрица:\n"+
      QString::number(cov[0][0]) + "\t" + QString::number(cov[0][1])
      + "\n" +QString::number(cov[1][0]) + "\t" + QString::number(cov[1][1]);
  ui->textbr->setText(log);
  ui->textbr->update();
  QApplication::processEvents();

  QVector<QPointF> newAx;
//Приступаем  к собственным значениям. Снова переобозначаем матрицу для удобвства
//вычисления СЗ. В итоге она имеет вид:
//(a-lbd) c
//  c   b-lbd
  double lbd1, lbd2, lbd, k;
  double a = cov[0][0], b = cov[1][1];

//Сложными(нет) вычислениями получаем корни квадратного уравнения
  lbd1 = ((a+b) - sqrt((a+b)*(a+b)-4*(a*b - c*c)))/2;
  lbd2 = ((a+b) + sqrt((a+b)*(a+b)-4*(a*b - c*c)))/2;
//Нам нужна максимальная лямбда для самой вытянутой компоненты. Из нее вычисляем k.
//k- коэф наклона прямой
  lbd = std::max(lbd1, lbd2);
  k = (lbd-a)/c;
//Уведомляем пользователя о наших действиях
  log = log + "\ny = "+
      QString::number(k) + "x + c";
  ui->textbr->setText(log);
  ui->textbr->update();
  QApplication::processEvents();
  log = log + "\nИТОГОВЫЙ МАССИВ:";
//Здесь уже почти финал. Вычисляем значения проекций на компоненту.
//Вручную мы бы строили высоты на новую "ось". Здесь же двигаем ее к нужной точке и вычисляем расстояние до центра вектора
  QPointF sNewAx(-100, k*(-100)+500), eNewAx(1000, k*1000+500);
  newAx.push_back(sNewAx);
  newAx.push_back(eNewAx);

  QPointF midNewAx((sNewAx.x()-eNewAx.x())/2, (sNewAx.y()-eNewAx.y())/2);
  double RESULT[nPoints];
  double shift;
//Загоняем мою идею в цикл. shift-как раз необходимое смещение, что бы iтая точка лежала на векторе
  for(int i = 0; i < nPoints; ++i)
    {
      shift = pnts[i].y() - k*pnts[i].x();
      midNewAx.setY(midNewAx.y()+shift);

      RESULT[i] = sqrt((pnts[i].x()-midNewAx.x())*(pnts[i].x()-midNewAx.x())
                       + (pnts[i].y()-midNewAx.y())*(pnts[i].y()-midNewAx.y()));
      log = log + "\nДля" + QString::number(i+1) + "ой точки:"+
          QString::number(RESULT[i]);
      ui->textbr->setText(log);
      ui->textbr->update();
      QApplication::processEvents();
    }

//Рисуем самую красивую прямую, что бы визуально показать, что мы ее нашли
  QwtPlotCurve *CA = new  QwtPlotCurve();
  CA->setStyle(QwtPlotCurve::Lines);
  CA->setPen(Qt::yellow, 6);
  CA->setSamples(newAx);
  CA->attach(ui->plot);
}
