#ifndef _GNUPLOT_PP_SESSION_HPP_
#define _GNUPLOT_PP_SESSION_HPP_

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


namespace gnuplotpp {
    
/**
 * @enum Dimension
 */
enum class Dimension : unsigned char
{
    x = 0,
    y,
    z
};

/**
 * @enum FileType
 * @brief Available format when saving the plot to a file
 */
enum class FileType : unsigned char 
{
    png = 0,
    pdf,
    ps
};

enum class ScreenTerminal : unsigned char
{
    qt = 0,
    wxt
};

/**
 * @class Session
 * @brief gnuplot session
 * 
 * TODO documentation
 * TODO implement some 3D graph + examples
 */
class Session {

    public:
        
        ~Session();
        
        /**
         * constructor
         * Initialize a gnuplot session
         */
        Session(ScreenTerminal terminal = ScreenTerminal::qt);
        
        /// disable copy constructor/operator
        Session(const Session& other) = delete;
        Session& operator=(const Session& other) = delete;

        /**
         * Show the plot on the screen
         */ 
        Session& showOnScreen(ScreenTerminal termimal = ScreenTerminal::qt);
        
        /**
         * save the current plot into a file using the 
         * @a fileType format.
         * 
         * Resolution should be set using the @a xSize and @a ySize parameters
         */
        Session& saveToFile(
            FileType fileType, 
            const std::string& filename, 
            float xSize = 0.f, 
            float ySize = 0.f);
        
        /**
         * Execute the gnuplot command @a cmd
         */ 
        Session& cmd(const std::string& cmdstr);
        
        /**
         * @overload cmd
         * @brief execute the gnuplot command @a command
         */ 
        Session& operator<<(const std::string& command);
    
        /**
         * clear the session. It will reset all current plots
         */ 
        Session& clear();
        
        /**
         * Option to override next plot. It means all current parameters will be kept except
         * the plot.
         */ 
        Session& overrideNextPlot();
        
        /**
         * Plot the single data series @a x with the @a title and using @a style
         * 
         * TODO x must be stl like.
         * TODO check if it's not easier to use iterators instead
         */
        template<class VectorT>
        Session& plot(
            const VectorT& x, 
            const std::string& title = "", 
            const std::string& style = "");
        
        /**
         * Plot the single data series identified as the @a column in the file @a filename
         * with the @a title and using @a style
         * 
         */
        Session& plot(
            const std::string& filename, 
            int column = 1, 
            const std::string& title = "", 
            const std::string& style = "");
        
        
        template<class XVectorT, class YVectorT>
        Session& plot(
            const XVectorT& x, 
            const YVectorT& y, 
            const std::string& title = "", 
            const std::string& style = "");
        
        template<class XVectorT, class YVectorT, class EVectorT>
        Session& plotWithError(
            const XVectorT x,
            const YVectorT y,
            const EVectorT dyErrors,
            const std::string& title ="",
            const std::string& style = "");
          
        Session& plotWithError(
            const std::string& filename,
            const std::string& title = "",
            const std::string& style = "",
            int xColumn = 1,
            int yColumn = 2,
            int dyErrorColumn= 3);
        
        Session& plot(
            const std::string& filename, 
            int xColumn, 
            int yColumn, 
            const std::string& title = "",
            const std::string& style = "");
        
        //TODO add histogram
        
        /**
         * Set the default grid
         */ 
        Session& setGrid();
        
        /**
         * Unset the grid
         */ 
        Session& unsetGrid();
        
        /**
         * Define the plot range for the dimension @a dim as [@a from : @a to]
         */ 
        Session& setRange(float from, float to, Dimension dim = Dimension::x);
        
        /**
         * Let gnuplot automatically define the range for the dimension @a dim
         */ 
        Session& setAutoScale(Dimension dim = Dimension::x);
        
        /**
         * Set a logarithm scale for the axis @a dim, using a subdivision of @a base
         */ 
        Session& setLogScale(Dimension dim = Dimension::y, int base = 10);
        
        /**
         * Unset the log scale for the axis @a dim
         */ 
        Session& unsetLogScale(Dimension dim = Dimension::y);
        
        /**
         * Set the gprah' title
         */ 
        Session& setTitle(const std::string& title);
        
        /**
         * Add a label @a title to the axis @a dim
         */ 
        Session& setLabel(Dimension dim, const std::string& title);
        
        /**
         * Specify where the legend shoud be
         * TODO: give an enum class instead of a string
         */ 
        Session& setLegendPosition(const std::string& position);
        
        /**
         * Disable the legend information for the graph
         */
        Session& unsetLegend();
        

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
        ScreenTerminal m_term;
        
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

template<class XVectorT, class YVectorT>
Session& Session::plot(const XVectorT& x, const YVectorT& y, const std::string& title, const std::string& style)
{
    if(x.size() == 0 || y.size () == 0) return *this;
    
    int maxIdx = std::min(x.size(),y.size());
    std::ofstream stream;
    const std::string& filename = createTemporaryFile(stream);
    if(filename.empty()) return *this;
    
    for(int i = 0; i < maxIdx; ++i)
    {
        stream << x[i] << "\t" << y[i] << std::endl;
    }
    stream.flush();
    stream.close();
    
    return plot(filename,1,2,title,style);
}

template<class XVectorT, class YVectorT, class EVectorT>
Session& Session::plotWithError(const XVectorT x, const YVectorT y, const EVectorT dyErrors, const std::string& title, const std::string& style)
{
    if(x.size() == 0 || y.size() == 0 || dyErrors.size() == 0)
        return *this;
    
    int range = std::min(x.size(),std::min(y.size(),dyErrors.size()));
    
    std::ofstream stream;
    const std::string& filename = createTemporaryFile(stream);
    if(filename.empty()) 
        return *this;
    
    for(int i = 0 ; i < range ; ++i)
    {
        stream << x[i] << "\t" << y[i] << "\t" << dyErrors[i] << std::endl;
    }
    stream.flush();
    stream.close();
    
    return plotWithError(filename,title,style);
}



    
}


#endif // end _GNUPLOT_PP_SESSION_HPP_
