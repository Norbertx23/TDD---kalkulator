#pragma once
#include "Calculator.hpp"
#include <QGridLayout>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    MainWindow(QWidget *parent = nullptr);

private slots:
  void onDigitClicked();
    void onOperatorClicked();
    void onEqualClicked();
    void onClearClicked();

private:
    QLineEdit *display;
    Calculator calculator;
    double currentVal = 0.0;
    QString pendingOperator;
    bool waitingForOperand = true;
    bool isHexMode = false;

    QPushButton *createButton(const QString &text, void (MainWindow::*slot)());
    QList<QWidget *> programmerButtons;
    void toggleProgrammerMode(bool enable);
};
