#include <session.hpp>
#include "tools/tools.hpp"

#include <cmath>
#include <vector>
#include <unistd.h>

int main()
{
    float start = 0.f;
    float end = 2*M_PI;
    
    std::vector<float> x;
    std::vector<float> y;
    
    x.reserve(200);
    
    float step = (end - start) /200.f;
    
    float cur = 0.f;
    do
    {
        x.push_back(cur);
        cur += step;
    } while(cur < end);
    
    
    gnuplotpp::Session session(gnuplotpp::ScreenTerminal::wxt);
    session.cmd("set key outside");
    float offset = 0.f;
    
    auto xSize = x.size();
    y.resize(xSize);
    session.setRange(-1,1,gnuplotpp::Dimension::y);
    for(int j = 0 ; j < 1000 ; ++j)
    {
        for(auto i = 0 ; i < xSize; ++i)
            y[i] = std::sin(x[i]+offset);
        
        // override next plot
        session.overrideNextPlot();
        // plot the next sinus
        session.plot(x,y,"","with lines lc 1");
        // sleep a bit
        usleep(100000);
        // increment the offset
        offset += M_PI/16.f;
    }
    
    
    return 0;
}
