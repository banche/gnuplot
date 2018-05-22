#include "session.hpp"
#include <string>
#include <stdexcept>

#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

namespace gnuplotpp {

  static const std::string& fileTypeToTerminal(FileType fileType)
{
    static const std::string s_table[3] = {
        "pngcairo",
        "pdfcairo",
        "postscript color"
    };
    
    return s_table[static_cast<int>(fileType)];
}

static const std::string& screenTerminalToString(ScreenTerminal term)
{
    static const std::string s_termTable[2] = {
        "qt",
        "wxt"
    };

    return s_termTable[static_cast<int>(term)];
}

Session::Session(ScreenTerminal term)
    : m_gnuCmd(0)
    , m_enableScreen(false)
    , m_isValid(false)
    , m_plot(false)
    , m_term(term)
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
        showOnScreen(m_term);
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




Session& Session::showOnScreen(ScreenTerminal term)
{
    if(not m_enableScreen)
    {
        throw std::runtime_error("screen mode is not enable");
        return *this;
    }
    cmd("set output");
    cmd("set terminal " + screenTerminalToString(term));
    if(m_plot)
        cmd("replot");
    
    return *this;
}



Session& Session::saveToFile(FileType fileType, const std::string& filename, float xSize, float ySize)
{
    if( ySize == 0)
        ySize = xSize;
    if(xSize == 0)
    {
        xSize = 10;
        ySize = 10;
    }
    
    
    std::string cmdStr = "set terminal " + fileTypeToTerminal(fileType) + " size " + std::to_string(xSize);
    cmdStr += "," + std::to_string(ySize);
    
    std::string outputStr = "set output \"" + filename + "\"";
    
    return cmd(cmdStr).cmd(outputStr);
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

Session& Session::operator<<(const std::string& command)
{
    return cmd(command);
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

Session& Session::plot(
    const std::string& filename, 
    int xColumn, 
    int yColumn, 
    const std::string& title, 
    const std::string& style)
{
    // check if the file exist
    if(not fileExist(filename))
        return *this;
    
    std::ostringstream cmdStream;
    if(m_plot)
        cmdStream << "replot";
    else
        cmdStream << "plot";
    
    cmdStream << "\"" << filename << "\" using " << xColumn << ":" << yColumn;
    if(title.empty())
        cmdStream << " notitle";
    else 
        cmdStream << " title \"" << title << "\"";
    
    if(style.empty())
        cmdStream << " with linespoint";
    else
        cmdStream << " " << style;
    
    return cmd(cmdStream.str());
}

Session& Session::setGrid()
{
    return cmd("set grid");
}

Session& Session::unsetGrid()
{
    return cmd("unset grid");
}

static const std::string& dim2String(Dimension dim)
{
    static const std::string dimStr[3] = 
    {
        "x","y","z"
    };
    return dimStr[static_cast<int>(dim)];
}

Session& Session::setRange(float from, float to, Dimension dim)
{

    std::ostringstream cmdStream;
    cmdStream << "set " << dim2String(dim) << "range[" << from << ":" << to << "]";
    return cmd(cmdStream.str());    
}


Session& Session::clear()
{
    m_plot = false;
    cmd("clear");
    return *this;
}

Session& Session::overrideNextPlot()
{
    m_plot = false;
    return *this;
}




Session& Session::setAutoScale(Dimension dim)
{
    return cmd("set autoscale " + dim2String(dim));
}

Session& Session::setLogScale(Dimension dim, int base)
{
    return cmd("set logscale " + dim2String(dim) + " " + std::to_string(base));
}

Session& Session::unsetLogScale(Dimension dim)
{
    return cmd("unset logscale " + dim2String(dim));
}

Session& Session::setLabel(Dimension dim, const std::string& title)
{
    return cmd("set " + dim2String(dim) + "label \"" + title + "\"");
}

Session& Session::setLegendPosition(const std::string& position)
{
    return cmd("set key " + position);
}

Session& Session::unsetLegend()
{
    return cmd("set nokey"); 
}


Session& Session::setTitle(const std::string& title)
{
    return cmd("set title \"" + title + "\"");
}

Session& Session::plotWithError(
    const std::string& filename, 
    const std::string& title, 
    const std::string& style, 
    int xColumn, int yColumn, int dyErrorColumn)
{
    std::string cmdStr;
    if(m_plot)
        cmdStr = "replot ";
    else 
        cmdStr = "plot ";
    
    cmdStr += "\"" + filename + "\" using " + std::to_string(xColumn) + ":" + std::to_string(yColumn);
    cmdStr += ":" + std::to_string(dyErrorColumn) + " with errorbars ";
    
    if(not title.empty())
        cmdStr += "notitle ";
    else 
        cmdStr += "title \"" + title + "\" ";
    
    if(not style.empty())
        cmdStr += style;
    
    return cmd(cmdStr);
}


}
