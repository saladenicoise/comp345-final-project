#include "GameEngine.h"

int main() {

    GameEngine g;
    std::string command;

    std::cout << "Welcome to Warzone!" << std::endl << std::endl;

    std::cout << "Type start to begin the game..." << std::endl << std::endl;

    while(g.getState() != "End") {

        if(g.getState() != "pregame")
            g.displayValidCommands();

        g.transition();
    }

    return 0;
}