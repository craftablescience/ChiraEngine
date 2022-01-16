#pragma once

#if !defined(_WIN32) && (!defined(__clang__) && (defined(__GNUC__) || defined(__GNUG__)))
#define CHIRA_CDECL __attribute__((__cdecl__))
#else
#define CHIRA_CDECL __cdecl
#endif
