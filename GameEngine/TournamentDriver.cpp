#include "GameEngine.h"

//
// Created by Jules on 12/1/2021.
//
int main() {
    GameEngine* g = new GameEngine("console");
    delete g;
    g = nullptr;

    return 0;
}

