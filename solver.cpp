#include "solver.h"
#include <QDebug>

Solver::Solver(QString type, u_int64_t a, u_int64_t b, QObject *parent) : QThread(parent)
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
//    counter.start();
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
    evklStruct temp = gcdex(a, b);
    if(temp.d == 1 && temp.y < 0)
        _result = temp.x;
    else
        _result = -1;

    _timeMS = counter.elapsed();
    _type = "evklid";
    emit this->solvingFinished(this);
    return;
}

void Solver::eulerSolve(u_int64_t a, u_int64_t b)
{
    counter.start();
    qDebug() << "PHI " << pow(a, phi(b));
    u_int64_t tempVal = pow(a, (phi(b) - 1));
    if(tempVal == -1)
        _result = -1;
    else
        _result = pow(a, (phi(b) - 1)) % b;
    _timeMS = counter.elapsed();
    _type = "euler";
    emit this->solvingFinished(this);
    return;
}

u_int64_t Solver::phi(u_int64_t n)
{
    u_int64_t result = n;
    for (u_int64_t i=2; i*i<=n; ++i)
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            result -= result / i;
        }
    if (n > 1)
        result -= result / n;
    return result;
}

u_int64_t Solver::pow(u_int64_t a, int b)
{
    u_int64_t temp = a;
    u_int64_t before = temp;
    for(int i = 0; i < b; i++)
    {
        temp*=a;
        qDebug() << "BEF : " << before;
        qDebug() << "TEMP : " << temp;
        if(before > temp)
        {
            qDebug() << "BAAADAS";
            return -1;
        }
        else
            before = temp;
    }
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

evklStruct Solver::gcdex(u_int64_t a, u_int64_t b)
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
