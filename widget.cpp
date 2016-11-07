#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mainLay = new QVBoxLayout(this);
    setLayout(mainLay);
    alarms = new QTextBrowser;
    alarms->setReadOnly(true);

    showErrors = new QPushButton("Показати помилки", this);
    connect(showErrors, SIGNAL(clicked(bool)), alarms, SLOT(show()));
    mainLay->addWidget(showErrors);

    inputALay = new QHBoxLayout;
    inputALab = new QLabel("Значення а", this);
    inputALine = new QLineEdit(this);
    inputALay->addWidget(inputALab);
    inputALay->addWidget(inputALine);
    mainLay->addLayout(inputALay);

    inputBLay = new QHBoxLayout;
    inputBLab = new QLabel("Значення б", this);
    inputBLine = new QLineEdit(this);
    inputBLay->addWidget(inputBLab);
    inputBLay->addWidget(inputBLine);
    mainLay->addLayout(inputBLay);

    solveButt = new QPushButton("Обрахувати", this);
    connect(solveButt, SIGNAL(clicked(bool)), this, SLOT(startSolving()));
    mainLay->addWidget(solveButt);

    NSDLab = new QLabel("НСД", this);
    NSDLab->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(NSDLab);
    NSDResultLay = new QHBoxLayout;
    NSDResultLab = new QLabel("Результат", this);
    NSDResultLine = new QLineEdit(this);
    NSDResultLine->setReadOnly(true);
    NSDResultLay->addWidget(NSDResultLab);
    NSDResultLay->addWidget(NSDResultLine);
    NSDTimeLay = new QHBoxLayout;
    NSDTimeLab = new QLabel("Час", this);
    NSDTimeLine = new QLineEdit(this);
    NSDTimeLine->setReadOnly(true);
    NSDTimeLay->addWidget(NSDTimeLab);
    NSDTimeLay->addWidget(NSDTimeLine);
    mainLay->addLayout(NSDResultLay);
    mainLay->addLayout(NSDTimeLay);

    evklidLab = new QLabel("Евклід", this);
    evklidLab->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(evklidLab);
    evklidResultLay = new QHBoxLayout;
    evklidResultLab = new QLabel("Результат", this);
    evklidResultLine = new QLineEdit(this);
    evklidResultLine->setReadOnly(true);
    evklidResultLay->addWidget(evklidResultLab);
    evklidResultLay->addWidget(evklidResultLine);
    evklidTimeLay = new QHBoxLayout;
    evklidTimeLab = new QLabel("Час", this);
    evklidTimeLine = new QLineEdit(this);
    evklidTimeLine->setReadOnly(true);
    evklidTimeLay->addWidget(evklidTimeLab);
    evklidTimeLay->addWidget(evklidTimeLine);
    mainLay->addLayout(evklidResultLay);
    mainLay->addLayout(evklidTimeLay);

    eulerLab = new QLabel("Ейлер", this);
    eulerLab->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(eulerLab);
    eulerResultLay = new QHBoxLayout;
    eulerResultLab = new QLabel("Результат", this);
    eulerResultLine = new QLineEdit(this);
    eulerResultLine->setReadOnly(true);
    eulerResultLay->addWidget(eulerResultLab);
    eulerResultLay->addWidget(eulerResultLine);
    eulerTimeLay = new QHBoxLayout;
    eulerTimeLab = new QLabel("Час", this);
    eulerTimeLine = new QLineEdit(this);
    eulerTimeLine->setReadOnly(true);
    eulerTimeLay->addWidget(eulerTimeLab);
    eulerTimeLay->addWidget(eulerTimeLine);
    mainLay->addLayout(eulerResultLay);
    mainLay->addLayout(eulerTimeLay);

    modElementLab = new QLabel("Перебір", this);
    modElementLab->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(modElementLab);
    modElementResultLay = new QHBoxLayout;
    modElementResultLab = new QLabel("Результат", this);
    modElementResultLine = new QLineEdit(this);
    modElementResultLine->setReadOnly(true);
    modElementResultLay->addWidget(modElementResultLab);
    modElementResultLay->addWidget(modElementResultLine);
    modElementTimeLay = new QHBoxLayout;
    modElementTimeLab = new QLabel("Час", this);
    modElementTimeLine = new QLineEdit(this);
    modElementTimeLine->setReadOnly(true);
    modElementTimeLay->addWidget(modElementTimeLab);
    modElementTimeLay->addWidget(modElementTimeLine);
    mainLay->addLayout(modElementResultLay);
    mainLay->addLayout(modElementTimeLay);
}

Widget::~Widget()
{
    alarms->close();
    if(needDel)
    {
        NSDSolver->terminate();
        modElementSolver->terminate();
        evklidSolver->terminate();
        eulerSolver->terminate();

        delete NSDSolver;
        delete modElementSolver;
        delete evklidSolver;
        delete eulerSolver;
    }

    delete inputALay;
    delete inputBLay;
    delete NSDResultLay;
    delete NSDTimeLay;
    delete evklidResultLay;
    delete evklidTimeLay;
    delete eulerResultLay;
    delete eulerTimeLay;
    delete modElementResultLay;
    delete modElementTimeLay;
}

void Widget::someOneSolved(Solver *obj)
{
    QString typeOf = obj->solvingType();
    if(typeOf == "NSD")
    {
        NSDResultLine->setText(QString::number(obj->getLastResult()));
        NSDTimeLine->setText(QString::number(obj->getLastTimeMS()) + "/" + QString::number(loopNum));
    }
    else if(typeOf == "modElement")
    {
        modElementResultLine->setText(QString::number(obj->getLastResult()));
        modElementTimeLine->setText(QString::number(obj->getLastTimeMS()) + "/" + QString::number(loopNum));
    }
    else if(typeOf == "evklid")
    {
        evklidResultLine->setText(QString::number(obj->getLastResult()));
        evklidTimeLine->setText(QString::number(obj->getLastTimeMS()) + "/" + QString::number(loopNum));
    }
    else if(typeOf == "euler")
    {
        eulerResultLine->setText(QString::number(obj->getLastResult()));
        eulerTimeLine->setText(QString::number(obj->getLastTimeMS()) + "/" + QString::number(loopNum));
    }
}

void Widget::startSolving()
{
    if(inputALine->text().isEmpty() && inputBLine->text().isEmpty())
        return;

    if(needDel)
    {
        NSDSolver->terminate();
        modElementSolver->terminate();
        evklidSolver->terminate();
        eulerSolver->terminate();

        delete NSDSolver;
        delete modElementSolver;
        delete evklidSolver;
        delete eulerSolver;
    }

    quint64 tempa = inputALine->text().toLongLong();
    quint64 tempb = inputBLine->text().toLongLong();

    NSDSolver = new Solver("NSD", tempa, tempb, this);
    modElementSolver = new Solver("modElement", tempa, tempb, this);
    evklidSolver = new Solver("evklid", tempa, tempb, this);
    eulerSolver = new Solver("euler", tempa, tempb, this);
    needDel = true;

    connect(eulerSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
    connect(evklidSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
    connect(NSDSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
    connect(modElementSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));

    connect(eulerSolver, SIGNAL(pushAlarm(QString)), this, SLOT(newAlarm(QString)));
    connect(evklidSolver, SIGNAL(pushAlarm(QString)), this, SLOT(newAlarm(QString)));
    connect(NSDSolver, SIGNAL(pushAlarm(QString)), this, SLOT(newAlarm(QString)));
    connect(modElementSolver, SIGNAL(pushAlarm(QString)), this, SLOT(newAlarm(QString)));

    NSDSolver->start();
    modElementSolver->start();
    evklidSolver->start();
    eulerSolver->start();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->text();
    if(event->text()=="\r")
    {
        startSolving();
        return;
    }
    else if(event->text()=="right")
    {
        if(focusA)
        {
            inputBLine->setFocus();
            focusA = false;
            return;
        }
        else
        {
            inputALine->setFocus();
            focusA = true;
            return;
        }
    }

}

void Widget::newAlarm(QString message)
{
    alarms->append(message);
    alarms->append("");
}





