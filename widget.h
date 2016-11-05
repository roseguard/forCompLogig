#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QSpacerItem>
#include <QKeyEvent>

#include "solver.h"
#include "defines.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QVBoxLayout         *mainLay;
    QTextBrowser        *alarms;
    QSpacerItem         *spacer;

    QHBoxLayout         *inputALay;
    QLabel              *inputALab;
    QLineEdit           *inputALine;

    QHBoxLayout         *inputBLay;
    QLabel              *inputBLab;
    QLineEdit           *inputBLine;

    QPushButton         *solveButt;

    QHBoxLayout         *NSDResultLay;
    QLabel              *NSDResultLab;
    QLineEdit           *NSDResultLine;
    QHBoxLayout         *NSDTimeLay;
    QLabel              *NSDTimeLab;
    QLineEdit           *NSDTimeLine;
    Solver              *NSDSolver;

    QHBoxLayout         *evklidResultLay;
    QLabel              *evklidResultLab;
    QLineEdit           *evklidResultLine;
    QHBoxLayout         *evklidTimeLay;
    QLabel              *evklidTimeLab;
    QLineEdit           *evklidTimeLine;
    Solver              *evklidSolver;

    QHBoxLayout         *eulerResultLay;
    QLabel              *eulerResultLab;
    QLineEdit           *eulerResultLine;
    QHBoxLayout         *eulerTimeLay;
    QLabel              *eulerTimeLab;
    QLineEdit           *eulerTimeLine;
    Solver              *eulerSolver;

    QHBoxLayout         *modElementResultLay;
    QLabel              *modElementResultLab;
    QLineEdit           *modElementResultLine;
    QHBoxLayout         *modElementTimeLay;
    QLabel              *modElementTimeLab;
    QLineEdit           *modElementTimeLine;
    Solver              *modElementSolver;

    bool needDel = false;
    bool focusA = true;

    void keyPressEvent(QKeyEvent *);
public slots:
    void someOneSolved(Solver * obj);
    void startSolving();
    void newAlarm(QString message);
};

#endif // WIDGET_H
