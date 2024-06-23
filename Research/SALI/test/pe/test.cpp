#include "PCRTBP.hpp"

int main()
{
    PCRTBP testclass(1, 2, 3, 4);
    double x = testclass.get_y();
    std::cout << x << std::endl;
    return 0;
}