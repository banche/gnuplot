#include "session.hpp"
#include <string>
#include <stdexcept>

#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

namespace gnuplotpp {

Session::Session()
    : m_gnuCmd(0)
    , m_enableScreen(false)
    , m_isValid(false)
    , m_plot(false)
{
    init();
}

Session::~Session()
{
    if(pclose(m_gnuCmd) == -1)
    {
        throw std::runtime_error("Error while trying to close gnuplot");
    }
    m_gnuCmd = nullptr;
    
    deleteTemporaryFiles();
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


const std::string& Session::createTemporaryFile(std::ofstream& stream)
{
    static const std::string empty;
    
    char name[] = "/tmp/gnuplotXXXXXX";
    if(mkstemp(name) == -1)
    {
        return empty;
    }
    
    stream.open(name);
    if(stream.bad())
        return empty;
    
    m_tmpFiles.push_back(name);
    return m_tmpFiles.back();   
}

void Session::deleteTemporaryFiles()
{
    for(const auto& name : m_tmpFiles)
    {
        std::remove(name.c_str());
    }
    m_tmpFiles.clear();
}

bool Session::fileExist(const std::string& filename)
{
    std::ifstream infile(filename.c_str());
    return infile.good();
}




Session& Session::showOnScreen()
{
    if(not m_enableScreen)
    {
        throw std::runtime_error("screen mode is not enable");
        return *this;
    }
    cmd("set output");
    cmd("set terminal qt");
    //TODO plot/replot if needed
    return *this;
}

Session& Session::cmd(const std::string& cmdstr)
{
    assert(m_isValid);
    // send the command in the pipe
    fputs((cmdstr + "\n").c_str(),m_gnuCmd );
    fflush(m_gnuCmd);
    
    if( ( not m_plot) && (cmdstr.find("plot") != std::string::npos))
    {
        m_plot = true;
    }
    
    return *this;
}

Session& Session::plot(const std::string& filename, int column, const std::string& title, const std::string& style)
{
    // check if file exist
    if(not fileExist(filename))
        return *this;
    
    std::ostringstream cmdStream;
    // commands
    if(m_plot)
        cmdStream << "replot";
    else 
        cmdStream << "plot";
    // file using given column
    cmdStream << "\"" << filename << "\" using " << column;
    // handle title
    if(title.empty()) cmdStream << " notitle ";
    else cmdStream << " title \"" << title << "\"";
    // handle style
    if(style.empty())
        cmdStream << " with linespoint";
    else
        cmdStream << " " << style;
    
    // do the real plot
    return cmd(cmdStream.str());
}




}