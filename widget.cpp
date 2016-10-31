#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mainLay = new QVBoxLayout(this);
    this->setLayout(mainLay);
    spacer = new QSpacerItem(1, 10);
    mainLay->addItem(spacer);

    inputALay = new QHBoxLayout;
    inputALab = new QLabel("Input A", this);
    inputALine = new QLineEdit(this);
    inputALay->addWidget(inputALab);
    inputALay->addWidget(inputALine);
    mainLay->addLayout(inputALay);

    inputBLay = new QHBoxLayout;
    inputBLab = new QLabel("Input B", this);
    inputBLine = new QLineEdit(this);
    inputBLay->addWidget(inputBLab);
    inputBLay->addWidget(inputBLine);
    mainLay->addLayout(inputBLay);

    solveButt = new QPushButton("SOLVE", this);
    connect(solveButt, SIGNAL(clicked(bool)), this, SLOT(startSolving()));
    mainLay->addWidget(solveButt);

    mainLay->addItem(spacer);

    NSDResultLay = new QHBoxLayout;
    NSDResultLab = new QLabel("NSD result", this);
    NSDResultLine = new QLineEdit(this);
    NSDResultLay->addWidget(NSDResultLab);
    NSDResultLay->addWidget(NSDResultLine);
    NSDTimeLay = new QHBoxLayout;
    NSDTimeLab = new QLabel("NSD time : ", this);
    NSDTimeLine = new QLineEdit(this);
    NSDTimeLay->addWidget(NSDTimeLab);
    NSDTimeLay->addWidget(NSDTimeLine);
    mainLay->addLayout(NSDResultLay);
    mainLay->addLayout(NSDTimeLay);

    mainLay->addItem(spacer);

    evklidResultLay = new QHBoxLayout;
    evklidResultLab = new QLabel("Evklid result", this);
    evklidResultLine = new QLineEdit(this);
    evklidResultLay->addWidget(evklidResultLab);
    evklidResultLay->addWidget(evklidResultLine);
    evklidTimeLay = new QHBoxLayout;
    evklidTimeLab = new QLabel("Evklid time", this);
    evklidTimeLine = new QLineEdit(this);
    evklidTimeLay->addWidget(evklidTimeLab);
    evklidTimeLay->addWidget(evklidTimeLine);
    mainLay->addLayout(evklidResultLay);
    mainLay->addLayout(evklidTimeLay);

    mainLay->addItem(spacer);

    eulerResultLay = new QHBoxLayout;
    eulerResultLab = new QLabel("Euler result", this);
    eulerResultLine = new QLineEdit(this);
    eulerResultLay->addWidget(eulerResultLab);
    eulerResultLay->addWidget(eulerResultLine);
    eulerTimeLay = new QHBoxLayout;
    eulerTimeLab = new QLabel("Euler time", this);
    eulerTimeLine = new QLineEdit(this);
    eulerTimeLay->addWidget(eulerTimeLab);
    eulerTimeLay->addWidget(eulerTimeLine);
    mainLay->addLayout(eulerResultLay);
    mainLay->addLayout(eulerTimeLay);

    mainLay->addItem(spacer);

    modElementResultLay = new QHBoxLayout;
    modElementResultLab = new QLabel("Mod Element result", this);
    modElementResultLine = new QLineEdit(this);
    modElementResultLay->addWidget(modElementResultLab);
    modElementResultLay->addWidget(modElementResultLine);
    modElementTimeLay = new QHBoxLayout;
    modElementTimeLab = new QLabel("Mod Element time", this);
    modElementTimeLine = new QLineEdit(this);
    modElementTimeLay->addWidget(modElementTimeLab);
    modElementTimeLay->addWidget(modElementTimeLine);
    mainLay->addLayout(modElementResultLay);
    mainLay->addLayout(modElementTimeLay);
}

Widget::~Widget()
{
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

    NSDSolver->start();
    modElementSolver->start();
    evklidSolver->start();
    eulerSolver->start();
}








