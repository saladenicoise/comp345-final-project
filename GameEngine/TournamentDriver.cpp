#include "GameEngine.h"
#include "LoggingObserver.h"

//
// Created by Jules on 12/1/2021.
//
int main() {
    bool valid = false;
    string mode;
    string delimiter = " ";
    string filename;
    while(!valid) {
        cout << "Type \"console\" to take commands from the console or \"file filename.txt \" to take commands from a file: ";
        getline(cin, mode);
        if(mode.find(' ') != string::npos) {
            filename = mode.substr(mode.find(delimiter)).erase(0,delimiter.length());
            mode = mode.substr(0, mode.find(delimiter));
            if(mode == "console") {
                valid = true;
            } else if (mode == "file"){
                ifstream f(filename.c_str());
                if(f.good()) {
                    valid = true;
                } else {
                    cout << "File not located in specified directory or file doesn't exist. Try again." << endl;
                }
            } else {
                cout << "Invalid mode selected. Try again." << endl;
            }
        } else {
            if(mode == "console")
                valid = true;
            else
                cout << "Need to specify file name!" << endl;
        }

    }
    LogObserver* logObserver = new LogObserver;
    GameEngine* g;
    if(mode == "console") {
        g = new GameEngine("console");
    } else {
        g = new GameEngine(filename);
    }
    g->attach(logObserver);

    while(g->getState() != "tourneystart") {
        cout << "Current State: " << g->getState() << endl;
        g->displayValidCommands();
        g->transition();
    }

    delete g;
    g = nullptr;

    return 0;
}

