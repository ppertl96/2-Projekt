#include "Integer.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <iostream>

void Integer::intToDigits(int value, bool& sign, int& length, char*& digits) {
    if (value == 0) {
        sign = false;
        length = 0;
        digits = nullptr;
        return;
    }

    sign = value < 0;
    value = std::abs(value);

    int temp = value;
    length = 0;
    while (temp > 0) {
        temp /= 100;
        length++;
    }

    digits = new char[length];
    temp = value;
    for (int i = 0; i < length; i++) {
        digits[i] = temp % 100;
        temp /= 100;
    }
}

Integer::Integer(int i) {
    intToDigits(i, sign, length, digits);
}

Integer::Integer(bool s, int n, char* d) : sign(s && (n > 0)), length(n) {
    if (n < 0) throw std::invalid_argument("Number of digits cannot be negative");

    if (n == 0) {
        digits = nullptr;
        sign = false;
        return;
    }

    int actualLength = n;
    while (actualLength > 0 && d[actualLength - 1] == 0) {
        actualLength--;
    }

    if (actualLength == 0) {
        length = 0;
        sign = false;
        digits = nullptr;
        return;
    }

    length = actualLength;
    digits = new char[length];
    std::memcpy(digits, d, length * sizeof(char));
}

Integer::Integer(const Integer& other) : sign(other.sign), length(other.length) {
    if (length > 0) {
        digits = new char[length];
        std::memcpy(digits, other.digits, length * sizeof(char));
    }
    else {
        digits = nullptr;
    }
}

Integer::~Integer() {
    delete[] digits;
}

Integer& Integer::operator=(const Integer& other) {
    if (this != &other) {
        delete[] digits;
        sign = other.sign;
        length = other.length;

        if (length > 0) {
            digits = new char[length];
            std::memcpy(digits, other.digits, length * sizeof(char));
        }
        else {
            digits = nullptr;
        }
    }
    return *this;
}

void Integer::normalize() {
    if (length == 0) {
        sign = false;
        return;
    }

    int newLength = length;
    while (newLength > 0 && digits[newLength - 1] == 0) {
        newLength--;
    }

    if (newLength == 0) {
        delete[] digits;
        length = 0;
        sign = false;
        digits = nullptr;
        return;
    }

    if (newLength < length) {
        char* newDigits = new char[newLength];
        std::memcpy(newDigits, digits, newLength * sizeof(char));
        delete[] digits;
        digits = newDigits;
        length = newLength;
    }
}

Integer Integer::operator-() const {
    Integer result(*this);
    if (length > 0) {
        result.sign = !sign;
    }
    return result;
}

void Integer::addDigits(const Integer& a, const Integer& b, char* result) const {
    int carry = 0;
    int maxLen = std::max(a.length, b.length);
    for (int i = 0; i < maxLen; ++i) {
        char aDigit = (i < a.length) ? a.digits[i] : 0;
        char bDigit = (i < b.length) ? b.digits[i] : 0;
        int sum = static_cast<int>(aDigit) + static_cast<int>(bDigit) + carry;
        result[i] = sum % 100;
        carry = sum / 100;
    }
    if (carry > 0) {
        result[maxLen] = carry;
    }
}

Integer Integer::operator+(const Integer& other) const {
    if (sign == other.sign) {
        int maxLen = std::max(length, other.length);
        char* resultDigits = new char[maxLen + 1]();
        addDigits(*this, other, resultDigits);
        Integer result(sign, maxLen + 1, resultDigits);
        delete[] resultDigits;
        result.normalize();
        return result;
    }
    else {
        if (sign) {
            return other - (-(*this));
        }
        else {
            return *this - (-other);
        }
    }
}

void Integer::subtractDigits(const Integer& a, const Integer& b, char* result) const {
    int borrow = 0;
    for (int i = 0; i < a.length; ++i) {
        int aDigit = a.digits[i];
        int bDigit = (i < b.length) ? b.digits[i] : 0;
        int diff = aDigit - bDigit - borrow;
        if (diff < 0) {
            diff += 100;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result[i] = diff;
    }
}

Integer Integer::operator-(const Integer& other) const {
    if (sign != other.sign) {
        return *this + (-other);
    }

    bool thisIsLarger = false;
    if (length > other.length) {
        thisIsLarger = true;
    }
    else if (length == other.length) {
        for (int i = length - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                thisIsLarger = digits[i] > other.digits[i];
                break;
            }
        }
    }

    int maxLen = std::max(length, other.length);
    char* resultDigits = new char[maxLen]();

    if (thisIsLarger) {
        subtractDigits(*this, other, resultDigits);
    }
    else {
        subtractDigits(other, *this, resultDigits);
    }

    bool resultSign = sign ? thisIsLarger : !thisIsLarger;
    Integer result(resultSign, maxLen, resultDigits);
    delete[] resultDigits;
    result.normalize();
    return result;
}

Integer Integer::operator*(const Integer& other) const {
    if (length == 0 || other.length == 0) {
        return Integer(0);
    }

    const int result_size = length + other.length;
    char* result_digits = new char[result_size]();

    for (int i = 0; i < other.length; ++i) {
        int carry = 0;
        for (int j = 0; j < length; ++j) {
            int product = static_cast<int>(digits[j]) * static_cast<int>(other.digits[i])
                + static_cast<int>(result_digits[i + j]) + carry;
            result_digits[i + j] = product % 100;
            carry = product / 100;
        }
        if (carry > 0) {
            result_digits[i + length] += carry;
        }
    }

    Integer result(sign != other.sign, result_size, result_digits);
    delete[] result_digits;
    result.normalize();
    return result;
}

Integer Integer::operator/(const Integer& other) const {
    if (other.length == 0) {
        throw std::invalid_argument("Division by zero");
    }

    Integer dividend = this->abs();
    Integer divisor = other.abs();

    if (dividend < divisor) {
        return Integer(0);
    }

    Integer quotient(0);
    Integer remainder(dividend);

    while (remainder >= divisor) {
        remainder = remainder - divisor;
        quotient = quotient + Integer(1);
    }

    quotient.sign = (sign != other.sign);
    return quotient;
}

Integer Integer::operator%(const Integer& other) const {
    if (other.length == 0) {
        throw std::invalid_argument("Modulo by zero");
    }
    Integer quotient = *this / other;
    Integer remainder = *this - (quotient * other);
    return remainder;
}

bool Integer::operator==(const Integer& other) const {
    if (sign != other.sign || length != other.length) {
        return false;
    }

    for (int i = 0; i < length; i++) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }

    return true;
}

bool Integer::operator<(const Integer& other) const {
    if (sign != other.sign) {
        return sign;
    }
    if (length != other.length) {
        return (length < other.length) ^ sign;
    }
    for (int i = length - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return (digits[i] < other.digits[i]) ^ sign;
        }
    }
    return false;
}

bool Integer::operator>=(const Integer& other) const {
    return !(*this < other);
}

Integer Integer::abs() const {
    Integer result(*this);
    result.sign = false;
    return result;
}

std::ostream& operator<<(std::ostream& os, const Integer& i) {
    if (i.length == 0) {
        os << "0";
        return os;
    }

    if (i.sign) {
        os << "-";
    }

    for (int pos = i.length - 1; pos >= 0; pos--) {
        if (pos == i.length - 1) {
            os << static_cast<int>(i.digits[pos]);
        }
        else {
            os.width(2);
            os.fill('0');
            os << static_cast<int>(i.digits[pos]);
        }
    }

    return os;
}

int Integer::getLength() const { return length; }
bool Integer::getSign() const { return sign; }