#include "MainWindow.hpp"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Calculator TDD");
  resize(300, 400);

  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  display = new QLineEdit("0");
  display->setAlignment(Qt::AlignRight);
  display->setReadOnly(true);
  display->setStyleSheet("font-size: 24px; padding: 5px;");
  mainLayout->addWidget(display);

  QGridLayout *gridLayout = new QGridLayout();
  mainLayout->addLayout(gridLayout);

  // Digit Buttons
  int pos[4][3] = {
      {7, 8, 9}, {4, 5, 6}, {1, 2, 3}, {0, -1, -1} // -1 for empty/special
  };

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 3; ++c) {
      int val = pos[r][c];
      if (val == -1)
        continue;
      QPushButton *btn =
          createButton(QString::number(val), SLOT(onDigitClicked()));
      gridLayout->addWidget(btn, r, c);
    }
  }

  // Operator Buttons
  QString ops[] = {"+", "-", "*", "/"};
  for (int i = 0; i < 4; ++i) {
    QPushButton *btn = createButton(ops[i], SLOT(onOperatorClicked()));
    gridLayout->addWidget(btn, i, 3);
  }

  // Other Buttons
  QPushButton *btnClear = createButton("C", SLOT(onClearClicked()));
  gridLayout->addWidget(btnClear, 3, 1);

  QPushButton *btnEq = createButton("=", SLOT(onEqualClicked()));
  gridLayout->addWidget(btnEq, 3, 2);
}

QPushButton *MainWindow::createButton(const QString &text, const char *member) {
  QPushButton *btn = new QPushButton(text);
  btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn->setStyleSheet("font-size: 18px;");
  connect(btn, SIGNAL(clicked()), this, member);
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

  currentVal = display->text().toDouble();
  pendingOperator = btn->text();
  waitingForOperand = true;
}

void MainWindow::onEqualClicked() {
  if (waitingForOperand)
    return;

  double rightVal = display->text().toDouble();
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

    display->setText(QString::number(result));
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
