#ifndef _GNUPLOT_PP_SESSION_HPP_
#define _GNUPLOT_PP_SESSION_HPP_

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace gnuplotpp {

/**
 * @class Session
 * @brief gnuplot session
 * 
 */
class Session {

    public:
        Session();
        ~Session();
        
        // disable copy constructor/operator
        Session(const Session& other) = delete;
        Session& operator=(const Session& other) = delete;

        Session& showOnScreen();
        
        Session& cmd(const std::string& cmdstr);
    
        template<class VectorT>
        Session& plot(const VectorT& x, const std::string& title = "", const std::string& style = "");
        
        Session& plot(const std::string& filename, int column = 1, const std::string& title = "", const std::string& style = "");

    private:
        void init();
        
        const std::string& createTemporaryFile(std::ofstream& stream);
        void deleteTemporaryFiles();
        
        static bool fileExist(const std::string& filename);

    private:
    
        FILE        *m_gnuCmd;
        bool        m_enableScreen;
        bool        m_isValid;
        bool        m_plot;
        
        std::vector<std::string> m_tmpFiles;

};


template<class VectorT>
Session& Session::plot(const VectorT& x, const std::string& title, const std::string& style)
{
    if( x.size() == 0) return *this;
    
    std::ofstream stream;
    const std::string& filename = createTemporaryFile(stream);
    if(filename.empty()) return *this;
    
    for(auto value : x)
    {
        stream << value << std::endl;
    }
    stream.flush();
    stream.close();
    
    return plot(filename,1,title,style);
}

    
}


#endif // end _GNUPLOT_PP_SESSION_HPP_
