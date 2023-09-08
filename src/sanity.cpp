#include <iostream>
#include <boost/version.hpp>

int main() {
    std::cout << "C++ version: " << __cplusplus << std::endl;
    std::cout << "Boost version: " << BOOST_VERSION << std::endl;
    return 0;
}