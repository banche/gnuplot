SET(GNUPLOT_FOUND FALSE)

find_program(GNUPLOT_EXEC gnuplot)

if(NOT GNUPLOT_EXEC)
    message(FATAL_ERROR "[Gnuplot] gnuplot est absent")
endif()

