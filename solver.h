#ifndef SOLVER_H
#define SOLVER_H

#include <QThread>
#include <QTime>

#include "defines.h"

struct evklStruct
{
    quint64 d;
    quint64 x;
    qint64 y;
};

class Solver : public QThread
{
    Q_OBJECT
public:
    Solver(QString type, quint64 a, quint64 b, QObject *parent = 0);
    void run();
private:
    quint64 _a;
    quint64 _b;
    quint64 _result;
    quint64 _timeMS;
    QTime counter;
    QString _type;
public:
    quint64 getLastResult();
    quint64 getLastTimeMS();
    quint64 pow(quint64 a, int b);
    quint64 phi(quint64 n);
    evklStruct gcdex(quint64 a, quint64 b);
    QString solvingType();

signals:
    void solvingFinished(Solver* object);
    void pushAlarm(QString message);
//    void solveModElement(quint64 value, quint64 range);
//    void solveNSD(quint64 a, quint64 b);
//    void solveEvklid(quint64 a, quint64 b);
//    void solveEuler(quint64 a, quint64 b);
private:
    void modElementSolver(quint64 value, quint64 range);
    void NSDSolver(quint64 _a, quint64 _b);
    void evklidSolver(quint64 a, quint64 b);
    void eulerSolver(quint64 _a, quint64 _b);
};

#endif // SOLVER_H
