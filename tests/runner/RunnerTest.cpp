#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <core/Platform.h>

#ifdef CHIRA_PLATFORM_WINDOWS
    #include <windows.h>
#endif

using namespace chira;

TEST(Runner, launchEngineDLL) {
    LOG_BEGIN();
#ifdef CHIRA_PLATFORM_WINDOWS
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    bool processResult = CreateProcess(NULL,
        "ChiraRunner.exe -game testgame -testmode",
        NULL,
        NULL,
        false,
        0,
        NULL,
        NULL,
        &si,
        &pi);

    EXPECT_TRUE(processResult);

    WaitForSingleObject(pi.hProcess, INFINITE);

    int lpExitCode;
    // check close result
    GetExitCodeProcess(pi.hProcess, &lpExitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    EXPECT_EQ(lpExitCode, 0);
#endif
    LOG_END();
}
