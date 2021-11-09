#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>
// For random shuffle
#include <algorithm>
#include <random>
#include <chrono>

#include "../Cards/Cards.h"
#include "../Orders/Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Command/CommandProcessing.h"

class Map;
class Deck;
class Player;
class CommandProcessor;

// Declaring class of GameEngine
class GameEngine {

private:

    // declaring a pointer to string attribute for storing the current state of the game
    std::string* state;

    // declaring a pointer to a string vector
    std::vector<std::string> * nextValidCommands;

    // declaring a pointer to an orders vector.
    std::vector<std::string> * ordersToExecute;

    // declaring a pointer to a command processor object
    CommandProcessor *commandProcessor;

    // declaring a support function for handling the steps associated with the transition
    void doTransition(std::string command);

    // declaration of a support function used for displaying the current transition taking place
    void displayTransition(std::string state, std::string* newState, std::string command);

    // declaration of a support function responsible for taking the order the player wishes to make
    void takeOrder();

    // Startup phase method to begin the game
    void startupPhase(std::string command);

    MapLoader *load_map;
    Map *map;
    vector<Player*> players;
    Deck * deck;
    // CommandProcessor* cp;
    int noOfPlayers = 0;
    bool mapFlag = true;

public:

    // default constructor declaration.
    GameEngine();

    // copy constructor declaration
    GameEngine(const GameEngine &gameEngine);

    //overloading assignment operator function declaration
    GameEngine& operator=(const GameEngine& gameEngine);

    // getter function for attribute state declaration
    std::string getState() const;

    // getter function for commandProcessor
    CommandProcessor getCommandProcessor();

    // setter function for attribute state declaration
    void setState(std::string state);

    // declaration for function used for *correctly* transitioning between states
    void transition(std::string command);

    // declaration for function used for displaying the next valid commands the player can make
    void displayValidCommands() const;

    // declaration for friend function used for overloading the insertion operator
    friend std::ostream& operator<<(std::ostream& out, const GameEngine& gameEngine);

    // declaration of destructor for class GameEngine
    ~GameEngine();

};

#endif