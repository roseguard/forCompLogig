#include "solver.h"
#include <QDebug>

Solver::Solver(QObject *parent) : QThread(parent)
{
    connect(this, SIGNAL(solveEuler(u_int64_t,u_int64_t)), this, SLOT(eulerSolve(u_int64_t,u_int64_t)));
    connect(this, SIGNAL(solveEvklid(u_int64_t,u_int64_t)), this, SLOT(evklidSolver(u_int64_t,u_int64_t)));
    connect(this, SIGNAL(solveModElement(u_int64_t,u_int64_t)), this, SLOT(modElementSolver(u_int64_t,u_int64_t)));
    connect(this, SIGNAL(solveNSD(u_int64_t,u_int64_t)), this, SLOT(NSDSolver(u_int64_t,u_int64_t)));
}

void Solver::modElementSolver(u_int64_t value, u_int64_t range)
{
    counter.start();
    for(u_int64_t i = 1; i < range ; i++)
    {
        if ((((value * i) - 1) % range) == 0)
        {
            _timeMS = counter.elapsed();
            _result = i;
            _type = "modElement";
            emit this->solvingFinished(this);
            return;
        }
    }
    _timeMS = counter.elapsed();
    _result = 0;
    _type = "modElement";
    emit this->solvingFinished(this);
    return;
}

void Solver::NSDSolver(u_int64_t a, u_int64_t b)
{
    counter.start();
    while(a*b!=0)
    {
        if(a>=b)
        {
            a = a % b;
        }
        else
        {
            b = b % a;
        }
    }
    _result = a + b;
    _timeMS = counter.elapsed();
    _type = "NSD";
    emit this->solvingFinished(this);
    return;
}

void Solver::evklidSolver(u_int64_t a, u_int64_t b)
{
    qDebug() << counter.currentTime().toString("hh:mm:ss:zzz");
    counter.start();
    while(a!=0 && b!=0)
    {
       if(a>=b) a=a%b;
       else b=b%a;
    }
    _result = a + b;
    _timeMS = counter.elapsed();
    _type = "evklid";
    emit this->solvingFinished(this);
    return;
}

void Solver::eulerSolve(u_int64_t a, u_int64_t b)
{
    counter.start();
    _result = pow(a, (phi(b) - 1)) % b;
    _timeMS = counter.elapsed();
    _type = "euler";
    emit this->solvingFinished(this);
    return;
}

u_int64_t Solver::phi(u_int64_t a)
{
    int result = a;
    for (int i=2; i*i<=a; ++i)
        if (a % i == 0) {
            while (a % i == 0)
                a /= i;
            result -= result / i;
        }
    if (a > 1)
        result -= result / a;
    return result;
}

u_int64_t Solver::pow(u_int64_t a, int b)
{
    int temp = a;
    for(int i = 0; i < b; i++)
        temp*=a;
    return temp;
}

u_int64_t Solver::getLastResult()
{
    return _result;
}

u_int64_t Solver::getLastTimeMS()
{
    return _timeMS;
}

QString Solver::solvingType()
{
    return _type;
}
