#include "GameEngine.h"

// Defining the default constructor for GameEngine class
GameEngine::GameEngine() {
    state = new std::string("pregame");
    nextValidCommands = new std::vector<std::string>;
    nextValidCommands->push_back("start");
    ordersToExecute = new std::vector<std::string>;
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


    return *this;
}


// Defining the getter function for state variable in GameEngine
std::string GameEngine::getState() const { return *this->state; }

// Defining the setter function for state variable in GameEngine
void GameEngine::setState(std::string state) {
    *this->state = state;
}

// Defining the support function for checking if the command entered by the player is valid
bool GameEngine::checkIfValid(std::string command) {

    if (command == "start" || command == "loadmap" || command == "validatemap" || command == "addplayer"
        || command == "assigncountries" || command == "issueorder" || command == "endissueorders"
        || command == "execorder" || command == "endexecorders" || command == "play" || command == "end"
        || command == "win") {

        return true;
    }
    return false;
}

// Defining the support function for checking if the command entered by the player leads to a valid transition
bool GameEngine::validTransition(std::string command) {
    for(int i = 0; i < nextValidCommands->size(); i++) {
        if(command == nextValidCommands->at(i)) {
            return true;
        }
    }
     return false;
}

// Defining the support function that handles the transition from state to state
void GameEngine::doTransition(std::string command) {

    std::string oldState;

    if(command == "start") {
        oldState = *state;
        setState("start");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("loadmap");
    } else if (command == "loadmap") {
        oldState = *state;
        setState("Map Loaded");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("loadmap");
        nextValidCommands->push_back("validatemap");
    } else if (command == "validatemap") {
        oldState = *state;
        setState("Map Validated");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("addplayer");
    } else if (command == "addplayer") {
        oldState = *state;
        setState("Players Added");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("addplayer");
        nextValidCommands->push_back("assigncountries");
    } else if (command == "assigncountries") {
        oldState = *state;
        setState("Assign Reinforcement");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("issueorder");
    } else if (command == "issueorder") {
        takeOrder();
        oldState = *state;
        setState("Issue Orders");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("issueorder");
        nextValidCommands->push_back("endissueorders");
    } else if (command == "endissueorders") {
        oldState = *state;
        setState("Execute Orders");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("execorder");
        nextValidCommands->push_back("endexecorders");
    } else if (command == "execorder") {
        if(ordersToExecute->size() == 0) {
            std::cout << "No more orders to execute." << std::endl;
            return;
        }
        std::cout << "Executing order \"" << ordersToExecute->back() << "\"" << std::endl;
        ordersToExecute->pop_back();
        nextValidCommands->clear();
        nextValidCommands->push_back("execorder");
        nextValidCommands->push_back("endexecorders");
        nextValidCommands->push_back("win");
    } else if (command == "endexecorders") {
        oldState = *state;
        setState("Assign Reinforcement");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("issueorder");
    } else if(command == "win") {
        oldState = *state;
        setState("Win");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        nextValidCommands->push_back("play");
        nextValidCommands->push_back("end");
    } else if(command == "play") {
        oldState = *state;
        setState("Win");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        ordersToExecute->clear();
        nextValidCommands->push_back("loadmap");
    } else if(command == "end") {
        oldState = *state;
        setState("End");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        ordersToExecute->clear();
        std::cout << "Thanks for playing Warzone." << std::endl;
    }

}

// Defining the support function responsible for displaying the current transition taking place
void GameEngine::displayTransition(std::string state, std::string *newState, std::string command) {
    std::cout << "Current state: " << state << ". Command: " << command
         << ". Transitioning to state: " << *newState << std::endl << std::endl;
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

    bool valid = checkIfValid(command);

    if(valid) {

        if(validTransition(command)) {
            doTransition(command);
        } else {
            std::cout << "Not a valid transition. Cannot " << command << " at state " << *state << std::endl;
        }

    } else {
        std::cout << "Command entered is not valid. State remains unchanged." << std::endl;
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

void GameEngine::startupPhase(){
    MapLoader load_map = MapLoader();
    deck = new Deck(30);
    // loadmap command here
    // TO DO: implement loop, the command prompt until correct file is in args
    try {
        this->map = load_map.loadMap("../Map/bigeurope.map");
        // validatemap command here
        if (map->validate())
            cout << "Generated Map is a connected graph!\n" << endl;
        else {
            cout << "Error! Generated Map is not connected graph!\n" << endl;
            throw 404;
        }
    } catch (int e) {
        cout << "\n\n" << "Invalid map specified: " << e << endl;
        exit(EXIT_FAILURE);
    }

    // addplayer command here
    // need min 2 players
    for (int i = 1; i <= 2; i++) {
        string playername = "Player " + std::to_string(i);

        // Player creation
        Player* player = new Player(playername);
        players.push_back(player);
    }

    // gamestart command here
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
                randTerritory->setPlayer(&*(*it));
                (*it)->t.push_back(randTerritory);
                copiedTerritories.erase(copiedTerritories.begin() + random);
                copiedTerritories.shrink_to_fit();
            }
        }
    }

    // Display players' territories, reinforcement pool
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

    // e) switch the game to the play phase

}
