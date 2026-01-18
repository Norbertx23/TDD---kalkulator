#pragma once

class Calculator {
public:
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    long long And(long long a, long long b);
    long long Or(long long a, long long b);
    long long Xor(long long a, long long b);
    long long Not(long long a);
    long long modulo(long long a, long long b);
    long long shiftLeft(long long a, long long b);
    long long shiftRight(long long a, long long b);
    long long rotateLeft(long long a, long long b, int bitWidth);
    long long rotateRight(long long a, long long b, int bitWidth);
    double squareRoot(double a);
    double reciprocal(double a);
    double percentage(double a);

};
