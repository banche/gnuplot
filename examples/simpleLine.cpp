#include "gnuplot++.hpp"

#include <vector>
#include "tools/tools.hpp"


int main() {
    
    
    std::vector<int> x;
    for(int i = 0 ; i < 10 ; ++i)
        x.push_back(i);
    
    gnuplotpp::Session session(gnuplotpp::ScreenTerminal::wxt);
    
    session.plot(x , std::string("f(x) = x") , "with lines");
    
    tools::press_enter_to_continue();
    
    return 0;
}
