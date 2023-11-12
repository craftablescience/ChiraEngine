# Helper for printing a variable
macro(print_variable VARIABLE)
    message(STATUS "  ${VARIABLE}: ${${VARIABLE}}")
endmacro()

# Helper to print several variables in a row
macro(print_variables)
    foreach(VARIABLE IN ITEMS ${ARGN})
        print_variable(${VARIABLE})
    endforeach()
endmacro()
