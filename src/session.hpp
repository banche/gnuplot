#ifndef _GNUPLOT_PP_SESSION_HPP_
#define _GNUPLOT_PP_SESSION_HPP_

#include <stdio.h>

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
    

    private:
        void init();


    private:
    
        FILE        *m_gnuCmd;
        bool        m_enableScreen;
        bool        m_isValid;

};
    
}


#endif // end _GNUPLOT_PP_SESSION_HPP_
