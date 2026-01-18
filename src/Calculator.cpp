#include "Calculator.hpp"
#include <stdexcept>
#include "cmath"

double Calculator::add(double a, double b) { return a + b; }

double Calculator::subtract(double a, double b) { return a - b; }

double Calculator::multiply(double a, double b) { return a * b; }

double Calculator::divide(double a, double b) {
  if (b == 0)
    throw std::invalid_argument("Division by zero");
  return a / b;
}
long long Calculator::And(long long a, long long b) { return a & b; }
long long Calculator::Or(long long a, long long b) { return a | b; }
long long Calculator::Xor(long long a, long long b) { return a ^ b; }
long long Calculator::Not(long long a) {return ~a;}
long long Calculator::modulo(long long a, long long b) {
  if (b == 0)
    throw std::invalid_argument("Division by zero");
  return a % b;
}

long long Calculator::shiftLeft(long long a, long long b) { return a << b; }

long long Calculator::shiftRight(long long a, long long b) { return a >> b; }

long long Calculator::rotateLeft(long long a, long long b, int bitWidth) {
  if (bitWidth <= 0)
    return a;
  unsigned long long val = (unsigned long long)a;
  unsigned long long mask =
      (bitWidth == 64) ? 0xFFFFFFFFFFFFFFFFULL : ((1ULL << bitWidth) - 1);
  val &= mask;

  b %= bitWidth;
  if (b < 0)
    b += bitWidth;
  if (b == 0)
    return (long long)val;

  unsigned long long left = (val << b) & mask;
  unsigned long long right = (val >> (bitWidth - b));
  return (long long)(left | right);
}

long long Calculator::rotateRight(long long a, long long b, int bitWidth) {
  if (bitWidth <= 0)
    return a;
  unsigned long long val = (unsigned long long)a;
  unsigned long long mask =
      (bitWidth == 64) ? 0xFFFFFFFFFFFFFFFFULL : ((1ULL << bitWidth) - 1);
  val &= mask;

  b %= bitWidth;
  if (b < 0)
    b += bitWidth;
  if (b == 0)
    return (long long)val;

  unsigned long long right = val >> b;
  unsigned long long left = (val << (bitWidth - b)) & mask;
  return (long long)(left | right);
}
double Calculator::squareRoot(double a) {
  if (a < 0)
    throw std::invalid_argument("Negative input for square root");
  return std::sqrt(a);
}
double Calculator::reciprocal(double a) {
  if (a == 0)
    throw std::invalid_argument("Division by zero");
  return 1.0 / a;
}

double Calculator::percentage(double a) { return a / 100.0; }

