#include "gnuplot++.hpp"

#include <vector>



int main() {
    
    
    std::vector<int> x;
    for(int i = 0 ; i < 10 ; ++i)
        x.push_back(i);
    
    gnuplotpp::Session session;
    
    session.plot(x , "f(x) = x" , "with lines");
    
    std::cout << std::endl << "Press ENTER to continue..." << std::endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
    
    return 0;
}