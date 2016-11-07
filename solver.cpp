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
        eulerSolver(_a, _b);
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
    for(int i = 0; i < loopNum; i++)
    {
        evklStruct temp = gcdex(a, b);
        if(temp.d == 1 && temp.y < 0)
            _result = temp.x;
        else
        {
            _result = b + temp.x;
        }

        _type = "evklid";
    }
    _timeMS = counter.elapsed();
    emit this->solvingFinished(this);
    return;
}

void Solver::eulerSolver(quint64 _a, quint64 _b)
{
    counter.start();
    for(int i = 0; i < loopNum; i++)
    {
        quint64 a = _a;
        quint64 b = _b;
        quint64 powa = phi(b) - 1;
        QVector<quint64> zalysh;
        if(a>b)
        {
            a-=(a/b)*b;
        }
        while(a > b)
        {
            a -= b;
        }
        while (powa >=2)
        {
            if (powa % 2 != 0)
            {
                powa -= 1;
                zalysh.append(a);
                powa /= 2;
                a = (a*a) % b;
            }
            else
            {
                powa /= 2;
                a = (a*a) % b;
            }
        }
        quint64 beforeA = a;
        for(int i = 0; i < zalysh.length(); i++)
        {
            a *= zalysh.at(i);
            if (a < beforeA)
            {
                emit pushAlarm(_type + " : " + "при обчисленні виникли надто великі значення(a*n). a = " +
                               QString::number(beforeA) + ", n = " + QString::number(zalysh.at(i)) + ", max = " +
                               QString::number((quint64)-1) + ", i/lenght = " + QString::number(i) + "/" + QString::number(zalysh.length()));
                return;
            }
            else
                beforeA = a;
        }

        if(a>b)
        {
            a-=(a/b)*b;
        }
        while(a > b)
        {
            a -= b;
        }

        qint64 z = pow(a,powa) % b;
        _result = z;
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
    if(b > 20)
    {
        QString mess = _type + " : ";
        mess.append("великий степінь, відсутня можливість підрахувати.");
        emit pushAlarm(mess);
    }
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
