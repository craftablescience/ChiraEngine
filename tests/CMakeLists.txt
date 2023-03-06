list(APPEND CHIRA_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/TestHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/engine/config/ConEntryTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/core/CommandLine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/event/EventsTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/resource/provider/FilesystemResourceProviderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/resource/Properties.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/script/AngelScriptHelpersTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/script/AngelScriptHolderTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/ui/debug/ConsolePanelTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/StringTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/utility/UUIDGeneratorTest.cpp)
