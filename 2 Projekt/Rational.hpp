#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include "Integer.hpp"
#include <iostream>
#include <stdexcept>

class Rational {
    Integer numerator;
    Integer denominator;

    void normalize();

public:
    Rational(const Integer& n, const Integer& d);
    Rational(const Rational& other) = default;
    ~Rational() = default;

    Rational& operator=(const Rational& other);

    Rational operator-() const;
    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;

    bool operator==(const Rational& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Rational& r);
};

#endif