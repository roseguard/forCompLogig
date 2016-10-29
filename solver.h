#ifndef SOLVER_H
#define SOLVER_H

#include <QThread>
#include <QTime>

struct evklStruct
{
    u_int64_t d;
    u_int64_t x;
    int y;
};

class Solver : public QThread
{
    Q_OBJECT
public:
    Solver(QString type, u_int64_t a, u_int64_t b, QObject *parent = 0);
    void run();
private:
    u_int64_t _a;
    u_int64_t _b;
    u_int64_t _result;
    u_int64_t _timeMS;
    QTime counter;
    QString _type;
public:
    u_int64_t getLastResult();
    u_int64_t getLastTimeMS();
    u_int64_t pow(u_int64_t a, int b);
    u_int64_t phi(u_int64_t n);
    evklStruct gcdex(u_int64_t a, u_int64_t b);
    QString solvingType();

signals:
    void solvingFinished(Solver* object);
//    void solveModElement(u_int64_t value, u_int64_t range);
//    void solveNSD(u_int64_t a, u_int64_t b);
//    void solveEvklid(u_int64_t a, u_int64_t b);
//    void solveEuler(u_int64_t a, u_int64_t b);
private:
    void modElementSolver(u_int64_t value, u_int64_t range);
    void NSDSolver(u_int64_t a, u_int64_t b);
    void evklidSolver(u_int64_t a, u_int64_t b);
    void eulerSolve(u_int64_t a, u_int64_t b);
};

#endif // SOLVER_H
