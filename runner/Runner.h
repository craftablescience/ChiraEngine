#pragma once

// cleans up the code
#define COMMAND_LINE_PARAM(tag, action) if (strcmp("-" #tag, param) == 0) {action}