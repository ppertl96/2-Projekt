#pragma once

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <algorithm>

class Integer {
public:
    Integer(int i = 0);
    Integer(bool s, int n, char* d);
    Integer(const Integer& other);
    ~Integer();

    Integer& operator=(const Integer& other);
    Integer operator-() const;
    Integer operator+(const Integer& other) const;
    Integer operator-(const Integer& other) const;
    Integer operator*(const Integer& other) const;
    Integer operator/(const Integer& other) const;
    Integer operator%(const Integer& other) const;
    bool operator==(const Integer& other) const;
    bool operator<(const Integer& other) const;
    bool operator>=(const Integer& other) const;

    Integer abs() const;
    void normalize();

    friend std::ostream& operator<<(std::ostream& os, const Integer& i);

    int getLength() const;
    bool getSign() const;

private:
    bool sign;
    int length;
    char* digits;

    void intToDigits(int value, bool& sign, int& length, char*& digits);
    void addDigits(const Integer& a, const Integer& b, char* result) const;
    void subtractDigits(const Integer& a, const Integer& b, char* result) const;
    void multiplyByDigit(const Integer& a, char digit, char* result) const;
    void shiftLeftDigits(char* d, int n, int shift) const;
};