# Helper for printing a variable
macro(print_variable VARIABLE)
    message(STATUS "  ${VARIABLE}: ${${VARIABLE}}")
endmacro()

#todo(wonder): print_variables
