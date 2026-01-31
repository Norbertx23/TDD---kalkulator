#include "Calculator.hpp"
#include <gtest/gtest.h>

TEST(CalculatorTest, AddsTwoNumbers) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.add(2.0, 3.0), 5.0);
}

TEST(CalculatorTest, SubtractsTwoNumbers) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.subtract(5.0, 3.0), 2.0);
}

TEST(CalculatorTest, MultipliesTwoNumbers) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.multiply(4.0, 2.5), 10.0);
}

TEST(CalculatorTest, DividesTwoNumbers) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.divide(10.0, 2.0), 5.0);
}

TEST(CalculatorTest, ThrowsOnDivisionByZero) {
  Calculator calc;
  EXPECT_THROW(calc.divide(10.0, 0.0), std::invalid_argument);
}

TEST(CalculatorTest, BitwiseAnd) {
  Calculator calc;
  EXPECT_EQ(calc.And(5, 3), 1);
}

TEST(CalculatorTest, BitwiseOr) {
  Calculator calc;
  EXPECT_EQ(calc.Or(5, 3), 7);
}

TEST(CalculatorTest, BitwiseXor) {
  Calculator calc;
  EXPECT_EQ(calc.Xor(5, 3), 6);
}

TEST(CalculatorTest, BitwiseNot) {
  Calculator calc;
  EXPECT_EQ(calc.Not(0), -1);
}

TEST(CalculatorTest, Modulo) {
  Calculator calc;
  EXPECT_EQ(calc.modulo(10, 3), 1);
}

TEST(CalculatorTest, ThrowsOnModuloByZero) {
  Calculator calc;
  EXPECT_THROW(calc.modulo(10, 0), std::invalid_argument);
}

TEST(CalculatorTest, ShiftLeft) {
  Calculator calc;
  EXPECT_EQ(calc.shiftLeft(1, 2), 4);
}

TEST(CalculatorTest, ShiftRight) {
  Calculator calc;
  EXPECT_EQ(calc.shiftRight(4, 2), 1);
}

TEST(CalculatorTest, SquareRoot) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.squareRoot(9.0), 3.0);
}

TEST(CalculatorTest, ThrowsOnNegativeSquareRoot) {
  Calculator calc;
  EXPECT_THROW(calc.squareRoot(-1.0), std::invalid_argument);
}

TEST(CalculatorTest, Reciprocal) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.reciprocal(4.0), 0.25);
}

TEST(CalculatorTest, ThrowsOnReciprocalZero) {
  Calculator calc;
  EXPECT_THROW(calc.reciprocal(0.0), std::invalid_argument);
}

TEST(CalculatorTest, Percentage) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.percentage(50.0), 0.5);
}

TEST(CalculatorTest, RotateLeft) {
  Calculator calc;
  EXPECT_EQ(calc.rotateLeft(1, 1, 4), 2);
  EXPECT_EQ(calc.rotateLeft(8, 1, 4), 1);
  EXPECT_EQ(calc.rotateLeft(0xF, 4, 4), 0xF);
}

TEST(CalculatorTest, RotateRight) {
  Calculator calc;
  EXPECT_EQ(calc.rotateRight(2, 1, 4), 1);
  EXPECT_EQ(calc.rotateRight(1, 1, 4), 8);
  EXPECT_EQ(calc.rotateRight(0xF, 4, 4), 0xF);
}

TEST(CalculatorTest, RotateEdgeCases) {
  Calculator calc;

  EXPECT_EQ(calc.rotateLeft(123, 1, 0), 123);
  EXPECT_EQ(calc.rotateRight(456, 2, -1), 456);

  EXPECT_EQ(calc.rotateLeft(0xAB, 0, 8), 0xAB);
  EXPECT_EQ(calc.rotateRight(0xCD, 0, 8), 0xCD);

  EXPECT_EQ(calc.rotateLeft(1, 5, 4), 2);

  EXPECT_EQ(calc.rotateLeft(2, -1, 4), 1);

  EXPECT_EQ(calc.rotateRight(1, -1, 4), 2);
}
TEST(CalculatorTest, RotatesLeft) {
  Calculator calc;
  EXPECT_EQ(calc.rotateLeft(128, 1, 8), 1);
}

TEST(CalculatorTest, RotatesRight) {
  Calculator calc;
  EXPECT_EQ(calc.rotateRight(1, 1, 8), 128);
}

TEST(CalculatorTest, SquareRootZero) {
  Calculator calc;
  EXPECT_DOUBLE_EQ(calc.squareRoot(0.0), 0.0);
}

TEST(CalculatorTest, LargeShift) {
  Calculator calc;
  EXPECT_EQ(calc.rotateLeft(1, 10, 8), 4);
}