#include "testGame.h"

void game_preInit() {
    printf("calling printf from testGame dll in preInit!");
}

void game_init() {
    printf("calling printf from testGame dll in init!");
}