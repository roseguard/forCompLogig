#include "solver.h"
#include <QDebug>

Solver::Solver(QString type, quint64 a, quint64 b, QObject *parent) : QThread(parent)
{
    _type = type;
    _a = a;
    _b = b;
}

void Solver::run()
{
    if(_type == "modElement")
        modElementSolver(_a, _b);
    else if(_type == "NSD")
        NSDSolver(_a, _b);
    else if(_type == "euler")
        eulerSolve(_a, _b);
    else if(_type == "evklid")
        evklidSolver(_a, _b);
}

void Solver::modElementSolver(quint64 value, quint64 range)
{
    counter.start();
    bool loopFin = false;
    for(int i = 0; i < loopNum; i++)
    {
        for(quint64 i = 1; i < range ; i++)
        {
            if ((((value * i) - 1) % range) == 0)
            {
                _result = i;
                _type = "modElement";
                loopFin = true;
                break;
            }
        }
        if(loopFin)
            break;
        _result = 0;
        _type = "modElement";
    }
    _timeMS = counter.elapsed();
    emit this->solvingFinished(this);
    return;
}

void Solver::NSDSolver(quint64 _a, quint64 _b)
{
    quint64 a = _a;
    quint64 b = _b;
    counter.start();
    for(int i = 0; i < loopNum; i++)
    {
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
        _type = "NSD";
    }
    _timeMS = counter.elapsed();
    emit this->solvingFinished(this);
    return;
}

void Solver::evklidSolver(quint64 a, quint64 b)
{
    counter.start();
//    while(a!=0 && b!=0)
//    {
//       if(a>=b) a=a%b;
//       else b=b%a;
//    }
//    _result = a + b;
//    _timeMS = counter.elapsed();
//    _type = "evklid";
//    emit this->solvingFinished(this);
//    return;
    for(int i = 0; i < loopNum; i++)
    {
        evklStruct temp = gcdex(a, b);
        if(temp.d == 1 && temp.y < 0)
            _result = temp.x;
        else
            _result = -1;

        _type = "evklid";
    }
    _timeMS = counter.elapsed();
    emit this->solvingFinished(this);
    return;
}

void Solver::eulerSolve(quint64 a, quint64 b)
{
    counter.start();
    for(int i = 0; i < loopNum; i++)
    {
        quint64 tempVal = pow(a, (phi(b) - 1));
        if(tempVal == -1)
            _result = -1;
        else
            _result = tempVal%b;        
        _type = "euler";
    }
    _timeMS = counter.elapsed();
    emit this->solvingFinished(this);
    return;
}

quint64 Solver::phi(quint64 n)
{
    quint64 result = n;
    for (quint64 i=2; i*i<=n; ++i)
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            result -= result / i;
        }
    if (n > 1)
    {
        result -= result / n;
    }
    return result;
}

quint64 Solver::pow(quint64 a, int b)
{
    quint64 temp = a;
    quint64 before = temp;
    for(int i = 0; i < b-1; i++)
    {
        temp*=a;
        if(before > temp)
        {
            return -1;
        }
        else
            before = temp;
    }
    return temp;
}

quint64 Solver::getLastResult()
{
    return _result;
}

quint64 Solver::getLastTimeMS()
{
    return _timeMS;
}

QString Solver::solvingType()
{
    return _type;
}

evklStruct Solver::gcdex(quint64 a, quint64 b)
{
    struct evklStruct temp;
    if (b == 0)
    {
        temp.d = a;
        temp.x = 1;
        temp.y = 0;
        return temp;
    }
    else
    {
        struct evklStruct costTemp = gcdex(b, a%b);
        temp.d = costTemp.d;
        temp.x = costTemp.y;
        temp.y = costTemp.x - (costTemp.y * (a / b));
        return temp;
    }
}
