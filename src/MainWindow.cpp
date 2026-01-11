#include "MainWindow.hpp"
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Calculator TDD");

  setMinimumSize(200, 250);
  resize(300, 400);


  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  display = new QLineEdit("0");
  display->setAlignment(Qt::AlignRight);
  display->setReadOnly(true);
  display->setStyleSheet("font-size: 24px; padding: 5px;");
  mainLayout->addWidget(display);

  QPushButton *btnMode = new QPushButton("Tryb Programisty: OFF");
  connect(btnMode, &QPushButton::clicked, this, [this, btnMode]() {
    if (programmerButtons.isEmpty())
      return;
    bool isVisible = this->programmerButtons[0]->isVisible();
    this->toggleProgrammerMode(!isVisible);
    btnMode->setText(isVisible ? "Tryb Programisty: OFF"
                               : "Tryb Programisty: ON");
  });
  mainLayout->addWidget(btnMode);

  QGridLayout *gridLayout = new QGridLayout();
  mainLayout->addLayout(gridLayout);
  int pos[4][3] = {{7, 8, 9}, {4, 5, 6}, {1, 2, 3}, {0, -1, -1}};
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 3; ++c) {
      int val = pos[r][c];
      if (val == -1)
        continue;
      QPushButton *btn =
          createButton(QString::number(val), &MainWindow::onDigitClicked);
      gridLayout->addWidget(btn, r + 2, c + 2);
    }
  }

  QString ops[] = {"+", "-", "*", "/"};
  for (int i = 0; i < 4; ++i) {
    QPushButton *btn = createButton(ops[i], &MainWindow::onOperatorClicked);
    gridLayout->addWidget(btn, i + 2, 5);
  }

  QPushButton *btnClear = createButton("C", &MainWindow::onClearClicked);
  gridLayout->addWidget(btnClear, 5, 3);

  QPushButton *btnEq = createButton("=", &MainWindow::onEqualClicked);
  gridLayout->addWidget(btnEq, 5, 4);

  QString hexDigits[] = {"A", "B", "C", "D", "E", "F"};
  for (int i = 0; i < 6; ++i) {
    QPushButton *btn = createButton(hexDigits[i], &MainWindow::onDigitClicked);
    gridLayout->addWidget(btn, 2 + (i / 2), i % 2);
    btn->setVisible(false);
    programmerButtons.append(btn);
  }

  QString logicOps[] = {"AND", "OR", "XOR", "NOT"};
  for (int i = 0; i < 4; ++i) {
    QPushButton *btn =
        createButton(logicOps[i], &MainWindow::onOperatorClicked);
    gridLayout->addWidget(btn, 6, i + 1);
    btn->setVisible(false);
    programmerButtons.append(btn);
  }
}

void MainWindow::toggleProgrammerMode(bool enable) {
  isHexMode = enable;
  onClearClicked();
  for (QWidget *widget : programmerButtons) {
    widget->setVisible(enable);
  }
  if (enable) {
    display->setStyleSheet(
        "font-size: 24px; padding: 5px; background-color: #e0f7fa;");
  } else {
    display->setStyleSheet(
        "font-size: 24px; padding: 5px; background-color: white;");
  }
  QTimer::singleShot(0, this, [this, enable]() {
    if (enable) {
      this->setMinimumSize(400,300);
      this->adjustSize();
    } else {
      setMinimumSize(200, 250);
      this->resize(300, 400);
    }
  });
}

QPushButton *MainWindow::createButton(const QString &text,
                                      void (MainWindow::*slot)()) {
  QPushButton *btn = new QPushButton(text);
  btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn->setStyleSheet("font-size: 18px;");
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
  display->setText(display->text() + btn->text());
}

void MainWindow::onOperatorClicked() {
  QPushButton *btn = qobject_cast<QPushButton *>(sender());
  if (!btn)
    return;
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
  if (waitingForOperand)
    return;
  double rightVal = 0.0;
  if (isHexMode) {
    bool ok;
    rightVal = (double)display->text().toLongLong(&ok, 16);
  } else {
    rightVal = display->text().toDouble();
  }
  double result = 0.0;
  try {
    if (pendingOperator == "+")
      result = calculator.add(currentVal, rightVal);
    else if (pendingOperator == "-")
      result = calculator.subtract(currentVal, rightVal);
    else if (pendingOperator == "*")
      result = calculator.multiply(currentVal, rightVal);
    else if (pendingOperator == "/")
      result = calculator.divide(currentVal, rightVal);
    if (isHexMode) {
      long long intResult = (long long)result;
      display->setText(QString::number(intResult, 16).toUpper());
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
  pendingOperator.clear();
  waitingForOperand = true;
}