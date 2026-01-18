#include "MainWindow.hpp"
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Calculator TDD");

  setMinimumSize(200, 250);
  resize(300, 400);

  setStyleSheet(
      "QMainWindow { background-color: #202020; color: #ffffff; }"
      "QLineEdit { background-color: #303030; color: #000000ff; border: 1px "
      "solid #505050; }"
      "QPushButton { background-color: #404040; color: #ffffff; border: 1px "
      "solid #606060; border-radius: 4px; font-size: 18px; }"
      "QPushButton:hover { background-color: #505050; }"
      "QPushButton:pressed { background-color: #606060; }"
      "QPushButton:disabled { background-color: #2a2a2a; color: #505050; "
      "border: 1px solid #3a3a3a; }"
      "QGroupBox { color: white; font-weight: bold; margin-top: 12px; border: "
      "1px solid #505050; border-radius: 4px; padding-top: 15px; }"
      "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top "
      "left; left: 10px; padding: 0 3px; background-color: #202020; }"
      "QRadioButton { color: #dddddd; font-size: 14px; spacing: 8px; padding: "
      "2px; min-height: 20px; }"
      "QRadioButton::indicator { width: 16px; height: 16px; border: 1px solid "
      "#888; border-radius: 8px; background: #444; }"
      "QRadioButton::indicator:checked { background: #00bcd4; border: 1px "
      "solid #00abc4; }"
      "QRadioButton:hover { color: #ffffff; }");

  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setSpacing(10);
  display = new QLineEdit("0");
  display->setAlignment(Qt::AlignRight);
  display->setReadOnly(true);
  display->setStyleSheet("font-size: 24px; padding: 5px; background-color: "
                         "#e0f7fa; color: black; min-height: 40px;");
  mainLayout->addWidget(display);

  QPushButton *btnMode = new QPushButton("Tryb Programisty: OFF");
  btnMode->setStyleSheet(
      "background-color: #303030; color: #aaa; border: none; padding: 5px;");
  connect(btnMode, &QPushButton::clicked, this, [this, btnMode]() {
    if (programmerButtons.isEmpty())
      return;
    bool visible = !this->programmerButtons[0]->isVisible();
    this->toggleProgrammerMode(visible);
    btnMode->setText(visible ? "Tryb Programisty: ON"
                             : "Tryb Programisty: OFF");

    if (btnSqrt)
      btnSqrt->setEnabled(!visible);
    if (btnPerc)
      btnPerc->setEnabled(!visible);
    if (btnRecip)
      btnRecip->setEnabled(!visible);
  });
  mainLayout->addWidget(btnMode);

  bitwiseDisplay = new BitwiseDisplay();
  bitwiseDisplay->setVisible(false);
  connect(bitwiseDisplay, &BitwiseDisplay::bitToggled, this,
          &MainWindow::onBitToggled);
  mainLayout->insertWidget(1, bitwiseDisplay);
  mainLayout->setAlignment(bitwiseDisplay, Qt::AlignCenter);
  programmerButtons.append(bitwiseDisplay);

  QWidget *compactHeader = new QWidget();
  QHBoxLayout *headerLayout = new QHBoxLayout(compactHeader);
  headerLayout->setContentsMargins(0, 5, 0, 5);

  QGroupBox *rBox = new QGroupBox("Base");
  QVBoxLayout *rLay = new QVBoxLayout(rBox);
  rLay->setContentsMargins(5, 5, 5, 5);
  rLay->setSpacing(6);
  rLay->setAlignment(Qt::AlignVCenter);
  radixGroup = new QButtonGroup(this);

  struct RInfo {
    const char *name;
    int id;
  };
  RInfo rads[] = {{"Hex", HEX}, {"Dec", DEC}, {"Oct", OCT}, {"Bin", BIN}};
  for (const auto &r : rads) {
    QRadioButton *rb = new QRadioButton(r.name);
    radixGroup->addButton(rb, r.id);
    rLay->addWidget(rb);
    if (r.id == DEC)
      rb->setChecked(true);
  }
  connect(radixGroup, &QButtonGroup::idClicked, this,
          &MainWindow::onRadixChanged);
  headerLayout->addWidget(rBox);

  QGroupBox *sBox = new QGroupBox("Size");
  QVBoxLayout *sLay = new QVBoxLayout(sBox);
  sLay->setContentsMargins(5, 5, 5, 5);
  sLay->setSpacing(6);
  sLay->setAlignment(Qt::AlignVCenter);
  sizeGroup = new QButtonGroup(this);
  struct SInfo {
    const char *name;
    int id;
  };
  SInfo szs[] = {
      {"Qword", QWORD}, {"Dword", DWORD}, {"Word", WORD}, {"Byte", BYTE}};
  for (const auto &s : szs) {
    QRadioButton *rb = new QRadioButton(s.name);
    sizeGroup->addButton(rb, s.id);
    sLay->addWidget(rb);
    if (s.id == QWORD)
      rb->setChecked(true);
  }
  connect(sizeGroup, &QButtonGroup::idClicked, this,
          &MainWindow::onSizeChanged);
  headerLayout->addWidget(sBox);

  compactHeader->setVisible(false);


  mainLayout->insertWidget(3, compactHeader);
  programmerButtons.append(compactHeader);

  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->setSpacing(8);
  mainLayout->addLayout(gridLayout);


  QPushButton *btnLsh = createButton("Lsh", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnLsh, 0, 0);
  programmerButtons.append(btnLsh);
  btnLsh->setVisible(false);

  QPushButton *btnRsh = createButton("Rsh", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnRsh, 0, 1);
  programmerButtons.append(btnRsh);
  btnRsh->setVisible(false);

  QPushButton *btnBACKSPACe =
      createButton("<-", &MainWindow::onBackspaceClicked);
  gridLayout->addWidget(btnBACKSPACe, 0, 2);

  QPushButton *btnClear = createButton("C", &MainWindow::onClearInputClicked);
  gridLayout->addWidget(btnClear, 0, 3);

  QPushButton *btnClearEverything =
      createButton("CE", &MainWindow::onClearClicked);
  gridLayout->addWidget(btnClearEverything, 0, 4);


  btnPerc = createButton("%", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnPerc, 0, 6);

  QPushButton *btnRoL = createButton("RoL", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnRoL, 1, 0);
  programmerButtons.append(btnRoL);
  btnRoL->setVisible(false);

  QPushButton *btnRoR = createButton("RoR", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnRoR, 1, 1);
  programmerButtons.append(btnRoR);
  btnRoR->setVisible(false);

  QPushButton *btnAnd = createButton("AND", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnAnd, 1, 2);
  programmerButtons.append(btnAnd);
  btnAnd->setVisible(false);

  QPushButton *btnOr = createButton("OR", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnOr, 1, 3);
  programmerButtons.append(btnOr);
  btnOr->setVisible(false);

  QPushButton *btnXor = createButton("XOR", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnXor, 1, 4);
  programmerButtons.append(btnXor);
  btnXor->setVisible(false);

  QPushButton *btnNot = createButton("NOT", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnNot, 1, 5);
  programmerButtons.append(btnNot);
  btnNot->setVisible(false);

  QPushButton *btnMod = createButton("MOD", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnMod, 1, 6);
  programmerButtons.append(btnMod);
  btnMod->setVisible(false);

  QPushButton *btnOpenP = createButton("(", &MainWindow::onParenthesisClicked);
  gridLayout->addWidget(btnOpenP, 2, 0);
  programmerButtons.append(btnOpenP);
  btnOpenP->setVisible(false);

  QPushButton *btnCloseP = createButton(")", &MainWindow::onParenthesisClicked);
  gridLayout->addWidget(btnCloseP, 2, 1);
  programmerButtons.append(btnCloseP);
  btnCloseP->setVisible(false);

  int row2Digits[] = {7, 8, 9};
  for (int i = 0; i < 3; ++i) {
    QPushButton *btn = createButton(QString::number(row2Digits[i]),
                                    &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 2, 2 + i);
    digitButtons.append(btn);
  }

  QPushButton *btnDiv = createButton("/", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnDiv, 2, 5);

  QPushButton *btnChangeSign =
      createButton("±", &MainWindow::onChangeSignClicked);
  gridLayout->addWidget(btnChangeSign, 2, 6);

  QString hexs[] = {"A", "B", "C", "D", "E", "F"};
  for (int i = 0; i < 2; ++i) {
    QPushButton *btn = createButton(hexs[i], &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 3, i);
    btn->setVisible(false);
    programmerButtons.append(btn);
    digitButtons.append(btn);
  }

  int row3Digits[] = {4, 5, 6};
  for (int i = 0; i < 3; ++i) {
    QPushButton *btn = createButton(QString::number(row3Digits[i]),
                                    &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 3, 2 + i);
    digitButtons.append(btn);
  }

  QPushButton *btnMul = createButton("*", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnMul, 3, 5);

  btnSqrt = createButton("√", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnSqrt, 3, 6);

  for (int i = 2; i < 4; ++i) {
    QPushButton *btn = createButton(hexs[i], &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 4, i - 2);
    btn->setVisible(false);
    programmerButtons.append(btn);
    digitButtons.append(btn);
  }

  int row4Digits[] = {1, 2, 3};
  for (int i = 0; i < 3; ++i) {
    QPushButton *btn = createButton(QString::number(row4Digits[i]),
                                    &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 4, 2 + i);
    digitButtons.append(btn);
  }

  QPushButton *btnMinus = createButton("-", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnMinus, 4, 5);

  btnRecip = createButton("1/x", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnRecip, 4, 6);

  for (int i = 4; i < 6; ++i) {
    QPushButton *btn = createButton(hexs[i], &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 5, i - 4);
    btn->setVisible(false);
    programmerButtons.append(btn);
    digitButtons.append(btn);
  }

  QPushButton *btnZero = createButton("0", &MainWindow::onDigitClicked);
  gridLayout->addWidget(btnZero, 5, 2, 1, 2);
  digitButtons.append(btnZero);

  QPushButton *btnDot = createButton(".", &MainWindow::onDecimalClicked);
  gridLayout->addWidget(btnDot, 5, 4);
  decimalPointBtn = btnDot;

  QPushButton *btnPlus = createButton("+", &MainWindow::onOperatorClicked);
  gridLayout->addWidget(btnPlus, 5, 5);

  QPushButton *btnEq = createButton("=", &MainWindow::onEqualClicked);
  btnEq->setStyleSheet("QPushButton { font-size: 18px; background-color: "
                       "#ff9900; color: black; border: 1px solid #cc7a00; } "
                       "QPushButton:hover { background-color: #ffad33; }");
  gridLayout->addWidget(btnEq, 5, 6);
}

void MainWindow::onParenthesisClicked() {
  QPushButton *btn = qobject_cast<QPushButton *>(sender());
  if (!btn)
    return;
  QString text = btn->text();

  if (text == "(") {
    CalculationState state;
    state.value = currentVal;
    state.pendingOp = pendingOperator;
    stateStack.push(state);

    currentVal = 0;
    currentValInt = 0;
    pendingOperator.clear();
    display->setText("0");
    waitingForOperand = true;
  } else if (text == ")") {
    if (stateStack.isEmpty())
      return;

    if (!pendingOperator.isEmpty()) {
      onEqualClicked();
    }

    double insideVal = currentVal;
    CalculationState state = stateStack.pop();
    currentVal = state.value;
    pendingOperator = state.pendingOp;

    if (!pendingOperator.isEmpty()) {
      double result = insideVal;
      if (pendingOperator == "+")
        result = calculator.add(currentVal, insideVal);
      else if (pendingOperator == "-")
        result = calculator.subtract(currentVal, insideVal);
      else if (pendingOperator == "*")
        result = calculator.multiply(currentVal, insideVal);
      else if (pendingOperator == "/")
        result = calculator.divide(currentVal, insideVal);
      else if (pendingOperator == "AND")
        result =
            (double)calculator.And((long long)currentVal, (long long)insideVal);
      else if (pendingOperator == "OR")
        result =
            (double)calculator.Or((long long)currentVal, (long long)insideVal);
      else if (pendingOperator == "XOR")
        result =
            (double)calculator.Xor((long long)currentVal, (long long)insideVal);
      else if (pendingOperator == "MOD")
        result = (double)calculator.modulo((long long)currentVal,
                                           (long long)insideVal);
      else if (pendingOperator == "Lsh")
        result = (double)calculator.shiftLeft((long long)currentVal,
                                              (long long)insideVal);
      else if (pendingOperator == "Rsh")
        result = (double)calculator.shiftRight((long long)currentVal,
                                               (long long)insideVal);
      else if (pendingOperator == "RoL" || pendingOperator == "RoR") {
        int bitWidth = 64;
        switch (currentSize) {
        case QWORD:
          bitWidth = 64;
          break;
        case DWORD:
          bitWidth = 32;
          break;
        case WORD:
          bitWidth = 16;
          break;
        case BYTE:
          bitWidth = 8;
          break;
        }
        if (pendingOperator == "RoL")
          result = (double)calculator.rotateLeft(
              (long long)currentVal, (long long)insideVal, bitWidth);
        else
          result = (double)calculator.rotateRight(
              (long long)currentVal, (long long)insideVal, bitWidth);
      }

      currentVal = result;
      pendingOperator.clear();
    } else {
      currentVal = insideVal;
    }

    currentValInt = (long long)currentVal;

    updateDisplay();
    waitingForOperand = true;
  }
}

void MainWindow::toggleProgrammerMode(bool enable) {
  isHexMode = enable;
  isProMode = enable;

  if (enable) {
    currentVal = display->text().toDouble();
    currentValInt = static_cast<long long>(currentVal);
    updateDisplay();
  } else {
    currentVal = static_cast<double>(currentValInt);
    display->setText(QString::number(currentVal));
  }

  for (QWidget *widget : programmerButtons) {
    widget->setVisible(enable);
  }

  if (enable) {
    display->setStyleSheet("font-size: 24px; padding: 5px; background-color: "
                           "#e0f7fa; color: black; min-height: 40px;");
  } else {
    display->setStyleSheet("font-size: 24px; padding: 5px; background-color: "
                           "#e0f7fa; color: black; min-height: 40px;");
  }

  QTimer::singleShot(0, this, [this, enable]() {
    if (enable) {
      this->setMinimumSize(400, 300);
      this->adjustSize();
    } else {
      setMinimumSize(200, 250);
      this->resize(300, 400);
    }
  });
  updateDigitAvailability();
}

QPushButton *MainWindow::createButton(const QString &text,
                                      void (MainWindow::*slot)()) {
  QPushButton *btn = new QPushButton(text);
  btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn->setStyleSheet("QPushButton { font-size: 18px; }"
                     "QPushButton:disabled { color: #888888; background-color: "
                     "#333333; border: 1px solid #444444; }");
  connect(btn, &QPushButton::clicked, this, slot);
  return btn;
}

void MainWindow::onDigitClicked() {
  QPushButton *btn = qobject_cast<QPushButton *>(sender());
  if (!btn)
    return;
  if (waitingForOperand) {
    display->clear();
    waitingForOperand = false;
  }
  if (isProMode) {
    bool ok;
    int val = btn->text().toInt(&ok, 16);
    if (ok) {
      bool allowed = false;
      switch (currentRadix) {
      case HEX:
        allowed = true;
        break;
      case DEC:
        allowed = (val < 10);
        break;
      case OCT:
        allowed = (val < 8);
        break;
      case BIN:
        allowed = (val < 2);
        break;
      }
      if (!allowed)
        return;
    }
  }
  if (display->text() == "0") {
    display->clear();
  }
  display->setText(display->text() + btn->text());

  if (isProMode) {
    bool ok;
    long long newVal = 0;
    QString txt = display->text();
    switch (currentRadix) {
    case HEX:
      newVal = txt.toLongLong(&ok, 16);
      break;
    case DEC:
      newVal = txt.toLongLong(&ok, 10);
      break;
    case OCT:
      newVal = txt.toLongLong(&ok, 8);
      break;
    case BIN:
      newVal = txt.toLongLong(&ok, 2);
      break;
    }
    if (ok) {
      currentValInt = newVal;
      bitwiseDisplay->setValue(currentValInt);
    }
  }
}

void MainWindow::onOperatorClicked() {
  QPushButton *btn = qobject_cast<QPushButton *>(sender());
  if (!btn)
    return;

  QString op = btn->text();

  if (op == "√") {
    isSqrtOperand = true;
    return;
  }
  if (op == "1/x") {
    isRecipOperand = true;
    return;
  }
  if (op == "%") {
    isPercentageOperand = true;
    return;
  }

  if (!pendingOperator.isEmpty() && !waitingForOperand) {
    onEqualClicked();
  }
  if (isHexMode) {
    bool ok;
    currentVal = (double)display->text().toLongLong(&ok, 16);
  } else {
    currentVal = display->text().toDouble();
  }
  pendingOperator = btn->text();
  waitingForOperand = true;
}

void MainWindow::onEqualClicked() {
  bool hasModifier = isSqrtOperand || isRecipOperand || isPercentageOperand;

  if (waitingForOperand && !hasModifier)
    return;

  double rightVal = 0.0;
  if (isHexMode) {
    bool ok;
    rightVal = (double)display->text().toLongLong(&ok, 16);
  } else {
    rightVal = display->text().toDouble();
  }

  if (isSqrtOperand) {
    rightVal = calculator.squareRoot(rightVal);
    isSqrtOperand = false;
  }
  if (isRecipOperand) {
    rightVal = calculator.reciprocal(rightVal);
    isRecipOperand = false;
  }
  if (isPercentageOperand) {
    if (pendingOperator == "+" || pendingOperator == "-") {
      rightVal = (rightVal / 100.0) * currentVal;
    } else {
      rightVal = rightVal / 100.0;
    }
    isPercentageOperand = false;
  }

  double result = rightVal;
  try {
    if (pendingOperator == "+")
      result = calculator.add(currentVal, rightVal);
    else if (pendingOperator == "-")
      result = calculator.subtract(currentVal, rightVal);
    else if (pendingOperator == "*")
      result = calculator.multiply(currentVal, rightVal);
    else if (pendingOperator == "/")
      result = calculator.divide(currentVal, rightVal);
    else if (pendingOperator == "AND")
      result =
          (double)calculator.And((long long)currentVal, (long long)rightVal);
    else if (pendingOperator == "OR")
      result =
          (double)calculator.Or((long long)currentVal, (long long)rightVal);
    else if (pendingOperator == "XOR")
      result =
          (double)calculator.Xor((long long)currentVal, (long long)rightVal);
    else if (pendingOperator == "NOT")
      result = (double)calculator.Not((long long)currentVal);
    else if (pendingOperator == "MOD")
      result =
          (double)calculator.modulo((long long)currentVal, (long long)rightVal);
    else if (pendingOperator == "Lsh")
      result = (double)calculator.shiftLeft((long long)currentVal,
                                            (long long)rightVal);
    else if (pendingOperator == "Rsh")
      result = (double)calculator.shiftRight((long long)currentVal,
                                             (long long)rightVal);
    else if (pendingOperator == "RoL" || pendingOperator == "RoR") {
      int bitWidth = 64;
      switch (currentSize) {
      case QWORD:
        bitWidth = 64;
        break;
      case DWORD:
        bitWidth = 32;
        break;
      case WORD:
        bitWidth = 16;
        break;
      case BYTE:
        bitWidth = 8;
        break;
      }
      if (pendingOperator == "RoL")
        result = (double)calculator.rotateLeft((long long)currentVal,
                                               (long long)rightVal, bitWidth);
      else
        result = (double)calculator.rotateRight((long long)currentVal,
                                                (long long)rightVal, bitWidth);
    }

    if (isHexMode) {
      long long intResult = (long long)result;
      display->setText(QString::number(intResult, 16).toUpper());
      currentValInt = intResult;
      bitwiseDisplay->setValue(currentValInt);
    } else {
      display->setText(QString::number(result));
    }
    currentVal = result;
    pendingOperator.clear();
    waitingForOperand = true;
  } catch (const std::exception &e) {
    QMessageBox::critical(this, "Error", e.what());
    onClearClicked();
  }
}

void MainWindow::onClearClicked() {
  display->setText("0");
  currentVal = 0.0;
  currentValInt = 0;
  if (bitwiseDisplay)
    bitwiseDisplay->setValue(0);
  pendingOperator.clear();
  waitingForOperand = true;
}

void MainWindow::onBackspaceClicked() {
  if (waitingForOperand)
    return;

  QString text = display->text();
  text.chop(1);

  if (text.isEmpty() || text == "-") {
    text = "0";
  }

  display->setText(text);

  if (isProMode) {
    bool ok;
    long long newVal = 0;
    switch (currentRadix) {
    case HEX:
      newVal = text.toLongLong(&ok, 16);
      break;
    case DEC:
      newVal = text.toLongLong(&ok, 10);
      break;
    case OCT:
      newVal = text.toLongLong(&ok, 8);
      break;
    case BIN:
      newVal = text.toLongLong(&ok, 2);
      break;
    }
    if (ok) {
      currentValInt = newVal;
      bitwiseDisplay->setValue(currentValInt);
    }
  }
}
void MainWindow::onClearInputClicked() {
  display->setText("0");

  waitingForOperand = false;
}

void MainWindow::onRadixChanged(int id) {
  currentRadix = static_cast<Radix>(id);
  updateDigitAvailability();
  updateDisplay();
}

void MainWindow::onSizeChanged(int id) {
  currentSize = static_cast<DataSize>(id);
  applyMask();
  updateDisplay();
}

void MainWindow::applyMask() {
  switch (currentSize) {
  case QWORD:
    currentMask = 0xFFFFFFFFFFFFFFFF;
    break;
  case DWORD:
    currentMask = 0xFFFFFFFF;
    break;
  case WORD:
    currentMask = 0xFFFF;
    break;
  case BYTE:
    currentMask = 0xFF;
    break;
  }
}

void MainWindow::onBitToggled(int bit) {
  long long val = currentValInt;
  long long mask = 1LL << bit;
  val ^= mask;
  val &= currentMask;

  currentValInt = val;
  bitwiseDisplay->setValue(val);

  if (pendingOperator.isEmpty()) {
    currentVal = (double)val;
  }

  updateDisplay();
}

void MainWindow::updateDisplay() {
  long long val = currentValInt;
  val &= currentMask;

  QString text;
  switch (currentRadix) {
  case HEX:
    text = QString::number(val, 16).toUpper();
    break;
  case DEC:
    text = QString::number(val);
    break;
  case OCT:
    text = QString::number(val, 8);
    break;
  case BIN:
    text = QString::number(val, 2);
    break;
  }
  display->setText(text);

  int bitCount = 64;
  switch (currentSize) {
  case QWORD:
    bitCount = 64;
    break;
  case DWORD:
    bitCount = 32;
    break;
  case WORD:
    bitCount = 16;
    break;
  case BYTE:
    bitCount = 8;
    break;
  }
  bitwiseDisplay->setDataSize(bitCount);
  bitwiseDisplay->setValue(val);
}

void MainWindow::onModeChanged() {
}

void MainWindow::updateDigitAvailability() {
  if (!isProMode) {
    for (auto btn : digitButtons) {
      bool isHex = false;
      QString t = btn->text();
      int v = t.toInt(nullptr, 16);
      if (v >= 10 || (t >= "A" && t <= "F"))
        isHex = true;

      if (!isHex)
        btn->setEnabled(true);
    }
    if (decimalPointBtn)
      decimalPointBtn->setEnabled(true);
    return;
  }

  if (decimalPointBtn)
    decimalPointBtn->setEnabled(false);

  for (auto btn : digitButtons) {
    QString t = btn->text();
    bool ok;
    int val = t.toInt(&ok, 16);
    if (!ok)
      continue;

    bool enabled = false;
    switch (currentRadix) {
    case HEX:
      enabled = true;
      break;
    case DEC:
      enabled = (val < 10);
      break;
    case OCT:
      enabled = (val < 8);
      break;
    case BIN:
      enabled = (val < 2);
      break;
    }
    btn->setEnabled(enabled);
  }
}

void MainWindow::onChangeSignClicked() {
  QString text = display->text();

  if (text.startsWith("-")) {
    text.remove(0, 1);
  } else {
    text.prepend("-");
  }
  display->setText(text);
}

void MainWindow::onDecimalClicked() {
  if (waitingForOperand) {
    display->setText("0.");
    waitingForOperand = false;
  } else {
    if (!display->text().contains(".")) {
      display->setText(display->text() + ".");
    }
  }
}