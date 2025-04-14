#include "Rational.hpp"
#include <stdexcept>

Rational::Rational(const Integer& n, const Integer& d) {
    if (d.getLength() == 0) {
        throw std::invalid_argument("Denominator cannot be zero");
    }
    if (d.getSign()) {
        numerator = -n;
        denominator = -d;
    }
    else {
        numerator = n;
        denominator = d;
    }
    normalize();
}

void Rational::normalize() {
    if (denominator.getSign()) {
        numerator = -numerator;
        denominator = -denominator;
    }

    // Compute GCD of numerator and denominator
    Integer a = numerator.abs();
    Integer b = denominator.abs();
    while (b.getLength() != 0) {
        Integer temp = b;
        b = a % b;
        a = temp;
    }
    Integer gcd = a;

    if (gcd.getLength() != 0) {
        numerator = numerator / gcd;
        denominator = denominator / gcd;
    }
}

Rational& Rational::operator=(const Rational& other) {
    if (this != &other) {
        numerator = other.numerator;
        denominator = other.denominator;
    }
    return *this;
}

Rational Rational::operator-() const {
    return Rational(-numerator, denominator);
}

Rational Rational::operator+(const Rational& other) const {
    Integer newNum = numerator * other.denominator + other.numerator * denominator;
    Integer newDen = denominator * other.denominator;
    return Rational(newNum, newDen);
}

Rational Rational::operator-(const Rational& other) const {
    Integer newNum = numerator * other.denominator - other.numerator * denominator;
    Integer newDen = denominator * other.denominator;
    return Rational(newNum, newDen);
}

Rational Rational::operator*(const Rational& other) const {
    Integer newNum = numerator * other.numerator;
    Integer newDen = denominator * other.denominator;
    return Rational(newNum, newDen);
}

Rational Rational::operator/(const Rational& other) const {
    if (other.numerator.getLength() == 0) {
        throw std::invalid_argument("Division by zero");
    }
    Integer newNum = numerator * other.denominator;
    Integer newDen = denominator * other.numerator;
    return Rational(newNum, newDen);
}

bool Rational::operator==(const Rational& other) const {
    return (numerator * other.denominator) == (denominator * other.numerator);
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    os << r.numerator << "/" << r.denominator;
    return os;
}