list(APPEND GAME_DLL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/testGame.h
    ${CMAKE_CURRENT_LIST_DIR}/testGame.cpp)
add_library(game MODULE ${GAME_DLL_SOURCES})
set_target_properties(game
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/testgame/bin")