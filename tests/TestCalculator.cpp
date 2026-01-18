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
