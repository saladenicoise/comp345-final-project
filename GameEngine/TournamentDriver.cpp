#include "GameEngine.h"

//
// Created by Jules on 12/1/2021.
//
int main() {
    cout << "Tournament Mode" << endl;
    GameEngine* g = new GameEngine("console");
    while(g->getState() != "tourneystart") {
        cout << "Current State: " << g->getState() << endl;
        g->displayValidCommands();
        g->transition();
    }

    delete g;
    g = nullptr;

    return 0;
}

