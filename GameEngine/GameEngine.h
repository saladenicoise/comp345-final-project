#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
// For random shuffle
#include <algorithm>
#include <random>
#include <chrono>

#include "../Cards/Cards.h"
#include "../Orders/Orders.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Command/CommandProcessing.h"
#include "../LoggingObserver/LoggingObserver.h"

class Map;
class Deck;
class Player;
class CommandProcessor;

// Declaring class of GameEngine
class GameEngine : public ILoggable, public Subject {

private:

    // declaring a pointer to string attribute for storing the current state of the game
    std::string* state;

    // declaring a pointer to a string vector
    std::vector<std::string> * nextValidCommands;

    // declaring a pointer to an orders vector.
    std::vector<std::string> * ordersToExecute;

    // declaring a pointer to a command processor object
    CommandProcessor *commandProcessor;

    // declaring a support function that uses the command processor object's validate method.
    bool validTransition(std::string command);

    // declaring a support function for handling the steps associated with the transition
    void doTransition(std::string command);

    // declaration of a support function used for displaying the current transition taking place
    std::string displayTransition(std::string state, std::string* newState, std::string command);

    // declaration of a support function responsible for taking the order the player wishes to make
    void takeOrder();

    // Startup phase method to begin the game
    void startupPhase(std::string command);

    // Variable declaration
    MapLoader *load_map;
    Map *map;
    vector<Player*> players;
    Deck * deck;
    int noOfPlayers = 0;
    string mapFilename;
    std::string commandsFilename;
    string playerName;
    
    
public:

    // default constructor declaration.
    GameEngine(string mode);

    // copy constructor declaration
    GameEngine(const GameEngine &gameEngine);

    //overloading assignment operator function declaration
    GameEngine& operator=(const GameEngine& gameEngine);

    // getter function for attribute state declaration
    std::string getState() const;

    // getter function for commandProcessor
    CommandProcessor getCommandProcessor();

    // getter function for commandsFilename
    std::string getCommandsFileName();

    // setter function for attribute state declaration
    void setState(std::string state);

    // declaration for function used for *correctly* transitioning between states
    void transition();

    // declaration for function used for displaying the next valid commands the player can make
    void displayValidCommands() const;

    // declaration for friend function used for overloading the insertion operator
    friend std::ostream& operator<<(std::ostream& out, const GameEngine& gameEngine);

    // declaration of destructor for class GameEngine
    ~GameEngine();

    bool fileExist (const std::string& name);

    std::string stringToLog();

    void reinforcementPhase(vector<Territory*> map, vector<Player*> players);
    void issueOrderPhase(vector<Player*> player, Player* targetPlayer, Deck* deck);
    void executeOrderPhase(vector<Player*> player);
    void mainGameLoop(vector<Territory*> map,vector<Player*> players, Deck* deck,int mapSize);

};

#endif
