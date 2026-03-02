#include <gtest/gtest.h>
#include "../include/MainWindow.hpp"
#include <QApplication>

class OverflowTest : public ::testing::Test {
protected:
  static QApplication *app;
  MainWindow *window;

  static void SetUpTestSuite() {
    int argc = 0;
    char **argv = nullptr;
    app = new QApplication(argc, argv);
  }

  static void TearDownTestSuite() {
    delete app;
  }

  void SetUp() override {
    window = new MainWindow();
    window->show();
  }

  void TearDown() override {
    delete window;
  }
};

QApplication *OverflowTest::app = nullptr;

// Test 1: Wpisanie maksymalnej wartości dla BYTE i sprawdzenie czy nie przepełnia się
TEST_F(OverflowTest, ByteModeMaxValue) {
  // Ustaw tryb BYTE
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();

  // Ustaw wartość 255 (maksimum dla BYTE)
  window->setCurrentValInt(255);
  window->setCurrentRadix(MainWindow::DEC);
  window->updateDisplay();

  // Wartość powinna być 255
  EXPECT_EQ(window->getCurrentValInt(), 255);
  EXPECT_EQ(window->getDisplay()->text(), "255");
}

// Test 2: Sprawdzenie czy maska BYTE odrzuca wartość > 255
TEST_F(OverflowTest, ByteModeOverflow256) {
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();

  // Ustaw wartość 256 (powinno być zmaskowane)
  window->setCurrentValInt(256);
  long long masked = window->getCurrentValInt() & window->getCurrentMask();

  // Po masce powinna być 0 (256 & 0xFF = 0)
  EXPECT_EQ(masked, 0);
}

// Test 3: Konwersja HEX na BIN dla WORD
TEST_F(OverflowTest, HexToBinaryWordMode) {
  window->setCurrentSize(MainWindow::WORD);
  window->applyMask();

  window->setCurrentValInt(0xABCD);
  window->setCurrentRadix(MainWindow::HEX);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "ABCD");

  // Zmień na BIN
  window->setCurrentRadix(MainWindow::BIN);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "1010101111001101");
}

// Test 4: Przepełnienie przy zmianie z DWORD na BYTE
TEST_F(OverflowTest, DwordValueOverflowsToByte) {
  window->setCurrentSize(MainWindow::DWORD);
  window->applyMask();

  // Ustaw wartość większą niż BYTE
  window->setCurrentValInt(0x100);
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();

  // Zmaskowana wartość powinna być 0
  long long masked = window->getCurrentValInt() & window->getCurrentMask();
  EXPECT_EQ(masked, 0);
}

// Test 5: Konwersja DEC na HEX dla BYTE
TEST_F(OverflowTest, DecimalToHexByteMode) {
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();

  window->setCurrentValInt(255);
  window->setCurrentRadix(MainWindow::DEC);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "255");

  window->setCurrentRadix(MainWindow::HEX);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "FF");
}

// Test 6: Konwersja OCT na BIN dla WORD
TEST_F(OverflowTest, OctalToBinaryWordMode) {
  window->setCurrentSize(MainWindow::WORD);
  window->applyMask();

  window->setCurrentValInt(0xFFFF);  // 65535 w DEC, 177777 w OCT
  window->setCurrentRadix(MainWindow::OCT);
  window->updateDisplay();

  window->setCurrentRadix(MainWindow::BIN);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "1111111111111111");
}

// Test 7: Maska dla QWORD
TEST_F(OverflowTest, QwordMask) {
  window->setCurrentSize(MainWindow::QWORD);
  window->applyMask();

  EXPECT_EQ(window->getCurrentMask(), 0xFFFFFFFFFFFFFFFFULL);
}

// Test 8: Maska dla DWORD
TEST_F(OverflowTest, DwordMask) {
  window->setCurrentSize(MainWindow::DWORD);
  window->applyMask();

  EXPECT_EQ(window->getCurrentMask(), 0xFFFFFFFFULL);
}

// Test 9: Maska dla WORD
TEST_F(OverflowTest, WordMask) {
  window->setCurrentSize(MainWindow::WORD);
  window->applyMask();

  EXPECT_EQ(window->getCurrentMask(), 0xFFFFULL);
}

// Test 10: Maska dla BYTE
TEST_F(OverflowTest, ByteMask) {
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();

  EXPECT_EQ(window->getCurrentMask(), 0xFFULL);
}

// Test 11: Sprawdzenie czy wartość przepełniona jest prawidłowo maskowana
TEST_F(OverflowTest, LargeNumberMasking) {
  window->setCurrentSize(MainWindow::DWORD);
  window->applyMask();

  window->setCurrentValInt(0x100000000LL);  // Większe niż DWORD
  long long masked = window->getCurrentValInt() & window->getCurrentMask();
  EXPECT_EQ(masked, 0);
}

// Test 12: Konwersja BIN na HEX dla BYTE
TEST_F(OverflowTest, BinaryToHexByteMode) {
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();

  window->setCurrentValInt(0xFF);
  window->setCurrentRadix(MainWindow::BIN);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "11111111");

  window->setCurrentRadix(MainWindow::HEX);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "FF");
}

// Test 13: Zmiana rozmiaru z przepełniającą wartością
TEST_F(OverflowTest, ResizeWithLargeValue) {
  window->setCurrentSize(MainWindow::QWORD);
  window->applyMask();
  window->setCurrentValInt(0xFFFFFFFFFFFFFFFFULL);

  // Zmień na BYTE
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();
  long long masked = window->getCurrentValInt() & window->getCurrentMask();

  // Powinna zostać zmaskowana do 0xFF
  EXPECT_EQ(masked, 0xFF);
}

// Test 14: Sprawdzenie czy updateDisplay() obsługuje pustą wartość
TEST_F(OverflowTest, UpdateDisplayEmpty) {
  window->setCurrentValInt(0);
  window->setCurrentRadix(MainWindow::DEC);
  window->updateDisplay();

  // Nie powinno być puste, powinno być "0"
  EXPECT_FALSE(window->getDisplay()->text().isEmpty());
  EXPECT_EQ(window->getDisplay()->text(), "0");
}

// Test 15: Konwersja między wszystkimi bazami dla tej samej wartości
TEST_F(OverflowTest, ConvertAllRadixes) {
  window->setCurrentSize(MainWindow::BYTE);
  window->applyMask();
  window->setCurrentValInt(255);

  // DEC
  window->setCurrentRadix(MainWindow::DEC);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "255");

  // HEX
  window->setCurrentRadix(MainWindow::HEX);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "FF");

  // OCT
  window->setCurrentRadix(MainWindow::OCT);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "377");

  // BIN
  window->setCurrentRadix(MainWindow::BIN);
  window->updateDisplay();
  EXPECT_EQ(window->getDisplay()->text(), "11111111");
}

