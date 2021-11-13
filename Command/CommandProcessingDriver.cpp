#include "GameEngine.h"

int main() {

    bool valid = false;
    std::string mode = "";
    std::string delimiter = " ";
    std::string filename = "";
    while(!valid) {
        std::cout << "Type \"console\" to take commands from the console or \"file filename.txt \" to take commands from a file: ";
        getline(cin, mode);
        if(mode.find(' ') != std::string::npos) {
            filename = mode.substr(mode.find(delimiter)).erase(0,delimiter.length());
            mode = mode.substr(0, mode.find(delimiter));
            if(mode == "console") {
                valid = true;
            } else if (mode == "file"){
                ifstream f(filename.c_str());
                if(f.good()) {
                    valid = true;
                } else {
                    std::cout << "File not located in specified directory or file doesn't exist. Try again." << std::endl;
                }
            } else {
                std::cout << "Invalid mode selected. Try again." << std::endl;
            }
        } else {
            if(mode == "console")
                valid = true;
            else
                cout << "Need to specify file name!" << endl;
        }

    }

    GameEngine* g;

    std::cout << "Welcome to Warzone!" << std::endl << std::endl;

    if(mode == "console") {
        g = new GameEngine("console");
        std::cout << "Type start to begin the game..." << std::endl << std::endl;
    } else {
        g = new GameEngine(filename);
    }

    while(g->getState() != "Assign Reinforcement") {

        if(g->getState() != "pregame")
            g->displayValidCommands();
        g->transition();
    }

    delete g;
    g = nullptr;

    return 0;
}