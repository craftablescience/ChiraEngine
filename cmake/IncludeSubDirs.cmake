# Helper to include subdirectories with cmake files of the same name
macro(include_subdirs)
    foreach(INC_SUBDIR IN ITEMS ${ARGN})
        include("${CMAKE_CURRENT_LIST_DIR}/${INC_SUBDIR}/_${INC_SUBDIR}.cmake")
    endforeach()
endmacro()
