#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mainLay = new QVBoxLayout(this);
    spacer = new QSpacerItem(1, 10);
    mainLay->addItem(spacer);

    inputALay = new QHBoxLayout(this);
    inputALab = new QLabel("Input A", this);
    inputALine = new QLineEdit(this);
    inputALay->addWidget(inputALab);
    inputALay->addWidget(inputALine);
    mainLay->addLayout(inputALay);

    inputBLay = new QHBoxLayout(this);
    inputBLab = new QLabel("Input B", this);
    inputBLine = new QLineEdit(this);
    inputBLay->addWidget(inputBLab);
    inputBLay->addWidget(inputBLine);
    mainLay->addLayout(inputBLay);

    solveButt = new QPushButton("SOLVE", this);
    connect(solveButt, SIGNAL(clicked(bool)), this, SLOT(startSolving()));
    mainLay->addWidget(solveButt);

    mainLay->addItem(spacer);

    NSDResultLay = new QHBoxLayout(this);
    NSDResultLab = new QLabel("NSD result", this);
    NSDResultLine = new QLineEdit(this);
    NSDResultLay->addWidget(NSDResultLab);
    NSDResultLay->addWidget(NSDResultLine);
    NSDTimeLay = new QHBoxLayout(this);
    NSDTimeLab = new QLabel("NSD time : ", this);
    NSDTimeLine = new QLineEdit(this);
    NSDTimeLay->addWidget(NSDTimeLab);
    NSDTimeLay->addWidget(NSDTimeLine);
    mainLay->addLayout(NSDResultLay);
    mainLay->addLayout(NSDTimeLay);
    NSDSolver = new Solver(this);
    connect(NSDSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));

    mainLay->addItem(spacer);

    evklidResultLay = new QHBoxLayout(this);
    evklidResultLab = new QLabel("Evklid result", this);
    evklidResultLine = new QLineEdit(this);
    evklidResultLay->addWidget(evklidResultLab);
    evklidResultLay->addWidget(evklidResultLine);
    evklidTimeLay = new QHBoxLayout(this);
    evklidTimeLab = new QLabel("Evklid time", this);
    evklidTimeLine = new QLineEdit(this);
    evklidTimeLay->addWidget(evklidTimeLab);
    evklidTimeLay->addWidget(evklidTimeLine);
    mainLay->addLayout(evklidResultLay);
    mainLay->addLayout(evklidTimeLay);
    evklidSolver = new Solver(this);
    connect(evklidSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));

    mainLay->addItem(spacer);

    eulerResultLay = new QHBoxLayout(this);
    eulerResultLab = new QLabel("Euler result", this);
    eulerResultLine = new QLineEdit(this);
    eulerResultLay->addWidget(eulerResultLab);
    eulerResultLay->addWidget(eulerResultLine);
    eulerTimeLay = new QHBoxLayout(this);
    eulerTimeLab = new QLabel("Euler time", this);
    eulerTimeLine = new QLineEdit(this);
    eulerTimeLay->addWidget(eulerTimeLab);
    eulerTimeLay->addWidget(eulerTimeLine);
    mainLay->addLayout(eulerResultLay);
    mainLay->addLayout(eulerTimeLay);
    eulerSolver = new Solver(this);
    connect(eulerSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));

    mainLay->addItem(spacer);

    modElementResultLay = new QHBoxLayout(this);
    modElementResultLab = new QLabel("Mod Element result", this);
    modElementResultLine = new QLineEdit(this);
    modElementResultLay->addWidget(modElementResultLab);
    modElementResultLay->addWidget(modElementResultLine);
    modElementTimeLay = new QHBoxLayout(this);
    modElementTimeLab = new QLabel("Mod Element time", this);
    modElementTimeLine = new QLineEdit(this);
    modElementTimeLay->addWidget(modElementTimeLab);
    modElementTimeLay->addWidget(modElementTimeLine);
    mainLay->addLayout(modElementResultLay);
    mainLay->addLayout(modElementTimeLay);
    modElementSolver = new Solver(this);
    connect(modElementSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
}

Widget::~Widget()
{

}

void Widget::someOneSolved(Solver *obj)
{
    QString typeOf = obj->solvingType();
    if(typeOf == "NSD")
    {
        NSDResultLine->setText(QString::number(obj->getLastResult()));
        NSDTimeLine->setText(QString::number(obj->getLastTimeMS()));
    }
    else if(typeOf == "modElement")
    {
        modElementResultLine->setText(QString::number(obj->getLastResult()));
        modElementTimeLine->setText(QString::number(obj->getLastTimeMS()));
    }
    else if(typeOf == "evklid")
    {
        evklidResultLine->setText(QString::number(obj->getLastResult()));
        evklidTimeLine->setText(QString::number(obj->getLastTimeMS()));
    }
    else if(typeOf == "euler")
    {
        eulerResultLine->setText(QString::number(obj->getLastResult()));
        eulerTimeLine->setText(QString::number(obj->getLastTimeMS()));
    }
}

void Widget::startSolving()
{
    if(inputALine->text().isEmpty() && inputBLine->text().isEmpty())
        return;
    NSDSolver->terminate();
    modElementSolver->terminate();
    evklidSolver->terminate();
    eulerSolver->terminate();
    NSDSolver = new Solver(this);
    modElementSolver = new Solver(this);
    evklidSolver = new Solver(this);
    eulerSolver = new Solver(this);
    connect(eulerSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
    connect(evklidSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
    connect(NSDSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));
    connect(modElementSolver, SIGNAL(solvingFinished(Solver*)), this, SLOT(someOneSolved(Solver*)));

    u_int64_t tempa = inputALine->text().toLongLong();
    u_int64_t tempb = inputBLine->text().toLongLong();
    emit NSDSolver->solveNSD(tempa, tempb);
    emit modElementSolver->solveModElement(tempa, tempb);
    emit evklidSolver->solveEvklid(tempa, tempb);
    emit eulerSolver->solveEuler(tempa, tempb);
}








