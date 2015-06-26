#include "tools.hpp"

#include <iostream>

namespace tools {

void press_enter_to_continue()
{
    std::cerr << "Press enter to continue" << std::endl;
    
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
}

    
}
