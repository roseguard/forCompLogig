#ifndef SOLVER_H
#define SOLVER_H

#include <QThread>
#include <QTime>

struct evklStruct
{
    u_int64_t a;
    u_int64_t b;
};

class Solver : public QThread
{
    Q_OBJECT
public:
    Solver(QObject *parent = 0);
private:
    u_int64_t _result;
    u_int64_t _timeMS;
    QTime counter;
    QString _type;
public:
    u_int64_t getLastResult();
    u_int64_t getLastTimeMS();
    u_int64_t pow(u_int64_t a, int b);
    u_int64_t phi(u_int64_t a);
    QString solvingType();

signals:
    void solvingFinished(Solver* object);
    void solveModElement(u_int64_t value, u_int64_t range);
    void solveNSD(u_int64_t a, u_int64_t b);
    void solveEvklid(u_int64_t a, u_int64_t b);
    void solveEuler(u_int64_t a, u_int64_t b);
public slots:
    void modElementSolver(u_int64_t value, u_int64_t range);
    void NSDSolver(u_int64_t a, u_int64_t b);
    void evklidSolver(u_int64_t a, u_int64_t b);
    void eulerSolve(u_int64_t a, u_int64_t b);
};

#endif // SOLVER_H
