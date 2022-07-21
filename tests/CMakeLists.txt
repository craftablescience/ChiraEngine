list(APPEND CHIRA_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/TestHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/engine/event/EventsTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/math/AxisTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/resource/provider/FilesystemResourceProviderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/script/AngelScriptHolderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/ConCommandTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/ConVarTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/StringTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/UUIDGeneratorTest.cpp)
