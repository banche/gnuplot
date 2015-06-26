macro(gnuplot_example NAME)
    add_executable(${NAME} ${NAME}.cpp)
    target_link_libraries(${NAME} gnuplot++ tools)
endmacro()

gnuplot_example(simpleLine)
gnuplot_example(curve)
gnuplot_example(movingSinus)