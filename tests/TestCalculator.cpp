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
