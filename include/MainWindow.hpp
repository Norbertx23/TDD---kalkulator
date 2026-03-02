#pragma once
#include "BitwiseDisplay.hpp"
#include "Calculator.hpp"
#include <QButtonGroup>
#include <QGridLayout>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton> // Added for QStackedWidget
#include <QStack>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);

  // Gettery dla testów
  QLineEdit *getDisplay() { return display; }
  BitwiseDisplay *getBitwiseDisplay() { return bitwiseDisplay; }

  // Enums publiczne dla testów
  enum Radix { HEX, DEC, OCT, BIN };
  enum DataSize { QWORD, DWORD, WORD, BYTE };

  // Settery dla testów
  void setCurrentValInt(long long val) { currentValInt = val; }
  long long getCurrentValInt() { return currentValInt; }
  void setCurrentRadix(Radix r) { currentRadix = r; }
  void setCurrentSize(DataSize s) { currentSize = s; }
  unsigned long long getCurrentMask() { return currentMask; }
  void updateDisplay();
  void applyMask();

private slots:
  void onDigitClicked();
  void onOperatorClicked();
  void onEqualClicked();
  void onClearClicked();
  void onClearInputClicked();
  void onBackspaceClicked();
  void onParenthesisClicked();
  void onModeChanged();
  void onRadixChanged(int id);
  void onBitToggled(int bit);

  void onChangeSignClicked();
  void onDecimalClicked();

private:
  QLineEdit *display;
  QList<QWidget *> programmerButtons;
  BitwiseDisplay *bitwiseDisplay;
  Calculator calculator;

  double currentVal = 0.0;
  long long currentValInt = 0;
  QString pendingOperator;
  bool waitingForOperand = true;
  bool isProMode = false;
  bool isHexMode = false;

  bool isSqrtOperand = false;
  bool isRecipOperand = false;
  bool isPercentageOperand = false;

  QPushButton *btnSqrt;
  QPushButton *btnPerc;
  QPushButton *btnRecip;
  QList<QPushButton *> digitButtons;
  QPushButton *decimalPointBtn = nullptr;

  void updateDigitAvailability();

  Radix currentRadix = DEC;
  QButtonGroup *radixGroup;

  DataSize currentSize = QWORD;
  QButtonGroup *sizeGroup;
  unsigned long long currentMask = 0xFFFFFFFFFFFFFFFF;

  void toggleProgrammerMode(bool enable);
  QPushButton *createButton(const QString &text, void (MainWindow::*slot)());

  void onSizeChanged(int id);

  struct CalculationState {
    double value;
    QString pendingOp;
  };
  QStack<CalculationState> stateStack;
};
