#include "session.hpp"
#include <string>
#include <stdexcept>



namespace gnuplotpp {

Session::Session()
    : m_gnuCmd(0)
    , m_enableScreen(false)
    , m_isValid(false)
{
}

Session::~Session()
{
    if(pclose(m_gnuCmd) == -1)
    {
        throw std::runtime_error("Error while trying to close gnuplot");
    }
    m_gnuCmd = nullptr;
}

void Session::init()
{
    if(getenv("DISPLAY") != nullptr)
    {
        m_enableScreen = true;
    }

    //TODO make it configurable through constructor
    std::string exec = "/usr/bin/gnuplot";
    m_gnuCmd = popen(exec.c_str(),"w");

    if(!m_gnuCmd)
    {
        m_isValid = false;
        throw std::runtime_error("Could not open pipe to gnuplot");
    }

    m_isValid = true;
    if(m_enableScreen)
        showOnScreen();
}

Session& Session::showOnScreen()
{
    if(not m_enableScreen)
    {
        throw std::runtime_error("screen mode is not enable");
        return *this;
    }
    //TODO set terminal and plot/replot if needed
    return *this;
}


}