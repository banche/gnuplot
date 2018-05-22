#include "gnuplot++.hpp"

#include <vector>
#include "tools/tools.hpp"
#include <cmath>


int main() {
    
    
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    
    x.reserve(400);
    y.reserve(400);
    float start = 0.f;
    float end = 2*M_PI;
    
    float step = (end - start)/400.f;
    for(float i = start; i < end; i+=step)
    {
        x.push_back(i);
        y.push_back(std::sin(i));
        z.push_back(std::cos(i));
    }
    
    gnuplotpp::Session session(gnuplotpp::ScreenTerminal::wxt);
    
    session.cmd("set key outside");
    
    session.plot(x,y , std::string("f(x) = sin(x)"), "with lines");
    
    tools::press_enter_to_continue();
    
    session.plot(x,z,std::string("g(x) = cos(x)"), "with lines");
    
    tools::press_enter_to_continue();
    return 0;
}
