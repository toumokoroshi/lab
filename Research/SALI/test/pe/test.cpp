#include <iostream>
#include <cmath>

int main() {
    double c1 = 1 / (2 * (2 - std::pow(2, 1.0 / 3.0)));
    double c2 = 1 / (2 * (2 - std::pow(2, 1.0 / 3.0)));  // Changed 1 / 3 to 1.0 / 3.0

    std::cout << c1 << std::endl;
    std::cout << c2 << std::endl;

    return 0;
}
