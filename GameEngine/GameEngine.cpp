#include "GameEngine.h"

// Defining the default constructor for GameEngine class
GameEngine::GameEngine() {
    state = new std::string("pregame");
    nextValidCommands = new std::vector<std::string>;
    nextValidCommands->push_back("start");
    ordersToExecute = new std::vector<std::string>;
    commandProcessor = new CommandProcessor();
}

// Defining the copy constructor for the GameEngine class
GameEngine::GameEngine(const GameEngine &gameEngine) {

    state = new std::string(*gameEngine.state);
    nextValidCommands = new std::vector<std::string>;
    for(int i = 0; i < gameEngine.nextValidCommands->size(); i++) {
        nextValidCommands->push_back(gameEngine.nextValidCommands->at(i));
    }
    ordersToExecute = new std::vector<std::string>;
    for(int i = 0; i < gameEngine.ordersToExecute->size(); i++) {
        ordersToExecute->push_back(gameEngine.ordersToExecute->at(i));
    }

    commandProcessor = new CommandProcessor(*gameEngine.commandProcessor);

}

// Defining the overloading function for the assignment operator
GameEngine& GameEngine::operator=(const GameEngine &gameEngine) {

    delete state;
    delete nextValidCommands;
    delete ordersToExecute;

    state = new std::string(*gameEngine.state);
    nextValidCommands = new std::vector<std::string>;
    for(int i = 0; i < gameEngine.nextValidCommands->size(); i++) {
        nextValidCommands->push_back(gameEngine.nextValidCommands->at(i));
    }
    ordersToExecute = new std::vector<std::string>;
    for(int i = 0; i < gameEngine.ordersToExecute->size(); i++) {
        ordersToExecute->push_back(gameEngine.ordersToExecute->at(i));
    }

    commandProcessor = new CommandProcessor(*gameEngine.commandProcessor);

    return *this;
}


// Defining the getter function for state variable in GameEngine
std::string GameEngine::getState() const { return *this->state; }

// Defining the setter function for state variable in GameEngine
void GameEngine::setState(std::string state) {
    *this->state = state;
}

// Defining the support function for checking if the command entered by the player leads to a valid transition
bool GameEngine::validTransition(std::string command) {
    return commandProcessor->validate(command, *nextValidCommands);
}

// Defining the support function that handles the transition from state to state
void GameEngine::doTransition(std::string command) {
    std::string oldState;

    startupPhase(command);
    
    /**************************** 
     * 
     * 
     * 
     * 
     * Game-Triggered Transition Here 
     * 
     * 
     * 
     * 
     ****************************/


    if(command == "replay") {
        oldState = *state;
        setState("Win");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        ordersToExecute->clear();
        nextValidCommands->push_back("loadmap");
    } else if(command == "quit") {
        oldState = *state;
        setState("End");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        ordersToExecute->clear();
        std::cout << "Thanks for playing Warzone." << std::endl;
    }

}

void GameEngine::startupPhase(std::string command) {
    // Declare variables
    load_map = new MapLoader();
    deck = new Deck(30);
    std::string oldState;
    std::string transitionString;
    int lastCommandIndex = commandProcessor->commands.size()-1;
    // cp = new CommandProcessor();

    // Start the game
    if(command == "start") {
        oldState = *state;
        setState("start");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.at(lastCommandIndex)->saveEffect(transitionString);
        nextValidCommands->clear();
        nextValidCommands->push_back("loadmap");
    } else if (command == "loadmap") {
        while(mapFlag) {
            // cp->getCommand();
            // getcommand the filename
            if("../Map/bigeurope.map") { // Check if map exists
                this->map = load_map->loadMap("../Map/bigeurope.map"); // Load map
                cout << "\n" << "Map Loaded" << endl;
                mapFlag = false; // Exit loop
            } else {
                cout << "\n\n" << "Invalid map specified! Please Try Again " << endl;
                continue; // Loop until correct file is given
            }
        }
        oldState = *state;
        setState("maploaded");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.at(lastCommandIndex)->saveEffect(transitionString);
        nextValidCommands->clear();
        nextValidCommands->push_back("loadmap");
        nextValidCommands->push_back("validatemap");
    } else if (command == "validatemap") {
        // cp->getCommand();
        if (map->validate()) {
            cout << "Generated Map is a connected graph!\n" << endl;
        } else {
            cout << "Error! Generated Map is not connected graph!\n" << endl;
        }
        oldState = *state;
        setState("mapvalidated");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.at(lastCommandIndex)->saveEffect(transitionString);
        nextValidCommands->clear();
        nextValidCommands->push_back("addplayer");
    } else if (command == "addplayer") {
        // cp->getCommand();
        noOfPlayers++; // Keep track of the number of players
        // getcommand their name
        string playername = "Player " + std::to_string(noOfPlayers); // Set playerID
                
         // Player creation
        Player* player = new Player(playername);
        player->setpID(noOfPlayers);
        players.push_back(player);

        cout << "Current Number of players: " << noOfPlayers << endl; // Display number of players
        oldState = *state;
        setState("playersadded");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.at(lastCommandIndex)->saveEffect(transitionString);
        nextValidCommands->clear();

        // Check if the correct number of players are playing
        if(noOfPlayers < 6)
            nextValidCommands->push_back("addplayer"); // Stop from adding more than 6 players
        if(noOfPlayers >= 2) {
            nextValidCommands->push_back("gamestart"); // Only start playing with min 2 players
        }
    } else if (command == "gamestart") { // Start the game

        // Make copy of the territories
        vector<Territory*> copiedTerritories(map->territories.size());
        for(int i = 0; i < map->territories.size(); i++){
            copiedTerritories[i] = map->territories[i];
        }

        // a) fairly distribute all the territories to the players
        while(copiedTerritories.size() != 0){
            for(auto it = players.begin(); it != players.end(); ++it){
                if(copiedTerritories.size() != 0) {
                    int random = rand() % copiedTerritories.size();
                    Territory* randTerritory = copiedTerritories[random];
                    randTerritory->setOwnerId((*it)->getPID());
                    (*it)->t.push_back(randTerritory);
                    copiedTerritories.erase(copiedTerritories.begin() + random);
                    copiedTerritories.shrink_to_fit();
                }
            }
        }

        // Display players' territories and reinforcement pool
        for(auto it = players.begin(); it != players.end(); ++it){
            cout << (*it)->getPlayerName() << ":" << endl;
            cout << " " << endl;

        // Showing the territories for each player and reinforcement pool
        cout << "Territories assigned: " << endl;
        for(int i = 0; i < (*it)->t.size(); i++){
             cout<< "\t" << *(*it)->t[i] << endl;
        }

        // c) give 50 initial armies to the players, which are placed in their respective reinforcement pool
        (*it)->setReinforcementPool(50);
        cout << "Armies in reinforcement pool: " << (*it)->getReinforcementPool() << endl;

        // d) let each player draw 2 initial cards from the deck using the deck’s draw() method
        for (int i = 0; i < 2; i++) {
            deck->draw((*it)->getHand());
        }
        cout << "------------------\n" << endl;
        
    }            
        // b) determine randomly the order of play of the players in the game
        unsigned seed = std::chrono::system_clock::now()
                            .time_since_epoch()
                            .count();
        std::shuffle(std::begin(players), std::end(players), std::default_random_engine());

        cout << "Randomized Players Order" << endl;
        for (auto p : players) {
            cout << "pID: " << p->getPID() << endl;
        }

        // e) switch the game to the play phase
        oldState = *state;
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("assignreinforcement");
    }
    
}

// Defining the support function responsible for displaying the current transition taking place
std::string GameEngine::displayTransition(std::string state, std::string *newState, std::string command) {
    string transitionString = "Current state: " + state + ". Command: " + command
                                + ". Transition to state: " + newState;
    std::cout << transitionString << std::endl << std::endl;

    return transitionString;

}

// Defining the support function responsible for taking in the order the player wishes to make
void GameEngine::takeOrder() {
    std::string order;

    std::cout << "Please type in your order: ";

    std::cin.ignore();
    std::getline(std::cin, order);

    ordersToExecute->insert(ordersToExecute->begin(), order);

}

// Defining the function that encapsulates the inner working of all the transition functions
void GameEngine::transition(std::string command) {

    commandProcessor->getCommand();

    int lastCommandIndex = commandProcessor->commands.size()-1;
    string command = commandProcessor->commands.at(lastCommandIndex)->getCommand();
    bool valid = commandProcessor->checkIfValidCommand(command);


    if(valid) {

        if(validTransition(command)) {
            doTransition(command);
        } else {
            commandProcessor->commands.at(lastCommandIndex)->saveEffect("Not a valid transition. Cannot " + command + " at state " + state);
            std::cout << commandProcessor->commands.at(lastCommandIndex)->getEffect() << std::endl;
        }

    } else {
        commandProcessor->commands.at(lastCommandIndex)->saveEffect("Command entered is not valid. State remains unchanged.");
        std::cout << commandProcessor->commands.at(lastCommandIndex)->getEffect() << std::endl;
    }

}

// Defining the function that displays the next valid commands the player can make
void GameEngine::displayValidCommands() const{

    std::cout << "Next Valid Commands: " << std::endl;
    int counter = 0;

    for(int i = 0; i < nextValidCommands->size(); i++) {
        std::cout << "\t" << ++counter << ". " << nextValidCommands->at(i) << std::endl;
    }

    std::cout << std::endl;
}

// Defining the friend function responsible for overloading the insertion operator
std::ostream& operator<< (std::ostream& out, const GameEngine& gameEngine) {
    out << "Current state: " << gameEngine.getState() << std::endl;

    gameEngine.displayValidCommands();

    return out;
}

// Defining the destructor for GameEngine class
GameEngine::~GameEngine() {
    delete state;
    state = nullptr;
    delete nextValidCommands;
    nextValidCommands = nullptr;
    delete ordersToExecute;
    ordersToExecute = nullptr;
}