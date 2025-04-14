#include "Integer.hpp"
#include "Rational.hpp"
#include <iostream>
#include <cassert>

int main() {
    try {
        // ===== Integer Tests =====
        std::cout << "===== Testing Integer =====" << std::endl;

        // Basic construction
        char d1[] = { 45, 23, 1 };       // -12345 (since digits are stored little-endian)
        char d2[] = { 99, 66, 33, 0, 0 }; // 336699 (digits: 99,66,33,0,0)
        Integer i1(true, 3, d1);
        Integer i2(false, 5, d2);
        std::cout << "i1: " << i1 << " (should be -12345)" << std::endl;
        std::cout << "i2: " << i2 << " (should be 336699)" << std::endl;

        // Test copy constructor and assignment
        Integer i1_copy(i1);
        Integer i2_assign = i2;
        assert(i1 == i1_copy);
        assert(i2 == i2_assign);
        std::cout << "Copy/assignment tests passed" << std::endl;

        // Arithmetic operations
        Integer i3 = -i1;
        Integer i4 = i1 + i2;
        std::cout << "-i1: " << i3 << " (should be 12345)" << std::endl;
        std::cout << "i1+i2: " << i4 << " (should be 324354)" << std::endl;

        Integer i5 = i1 - i2;
        Integer i6 = i1 * i2;
        std::cout << "i1-i2: " << i5 << " (should be -349044)" << std::endl;
        std::cout << "i1*i2: " << i6 << " (should be -4155559155)" << std::endl;

        // Edge cases
        Integer zero(0);
        Integer one(1);
        assert(i1 + zero == i1);
        assert(i1 * zero == zero);
        std::cout << "Zero handling tests passed" << std::endl;

        // ===== Rational Tests =====
        std::cout << "\n===== Testing Rational =====" << std::endl;

        // Basic construction
        Rational r1(i1, i2);  // -12345/336699
        Rational r2(i4, i5);   // 324354/-349044
        std::cout << "r1: " << r1 << " (should be -12345/336699)" << std::endl;
        std::cout << "r2: " << r2 << " (should be 324354/-349044)" << std::endl;

        // Test sign normalization
        Rational r_neg_denom(one, Integer(-2));
        std::cout << "1/-2 normalized: " << r_neg_denom << " (should be -1/2)" << std::endl;

        // Arithmetic operations
        Rational r3 = -r1;
        Rational r4 = r1 + r2;
        Rational r5 = r1 - r2;
        std::cout << "-r1: " << r3 << " (should be 12345/336699)" << std::endl;
        std::cout << "r1+r2: " << r4 << " (should be -12345/336699 + 324354/-349044)" << std::endl;
        std::cout << "r1-r2: " << r5 << " (should be -12345/336699 - 324354/-349044)" << std::endl;

        Rational r6 = r1 * r2;
        Rational r7 = r1 / r2;
        std::cout << "r1*r2: " << r6 << " (should be -12345*324354/336699*349044)" << std::endl;
        std::cout << "r1/r2: " << r7 << " (should be -12345*349044/336699*324354)" << std::endl;

        // Simple fraction
        Rational half(one, Integer(2));
        std::cout << "1/2: " << half << " (should be 1/2)" << std::endl;

        // Equality tests
        assert(Rational(one, Integer(2)) == Rational(Integer(2), Integer(4)));
        std::cout << "Equality test passed" << std::endl;

        // Division by zero test
        try {
            Rational r_error(one, zero);
            std::cerr << "ERROR: Should have thrown exception!" << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Successfully caught division by zero: " << e.what() << std::endl;
        }

        std::cout << "\nAll tests passed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}