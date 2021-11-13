#include "GameEngine.h"
#include <unordered_map>
// Defining the default constructor for GameEngine class
GameEngine::GameEngine(string mode) {
    state = new std::string("pregame");
    nextValidCommands = new std::vector<std::string>;
    nextValidCommands->push_back("start");
    ordersToExecute = new std::vector<std::string>;
    commandProcessor = new CommandProcessor();
    if(mode == "console") {
       commandProcessor = new CommandProcessor();
    } else {
        FileLineReader *flr = new FileLineReader(mode);
        commandProcessor = new FileCommandProcessorAdapter(*flr);
    }
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

CommandProcessor GameEngine::getCommandProcessor() {
    return *commandProcessor;
}

std::string GameEngine::getCommandsFileName() {
    return commandsFilename;
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

    // Start the game
    if(command == "start") {
        oldState = *state;
        setState("start");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.back()->saveEffect(transitionString);
        nextValidCommands->clear();
        nextValidCommands->push_back("loadmap");
    } else if (command == "loadmap") {
        if(fileExist(mapFilename)) { // Check if map exists
            this->map = load_map->loadMap(mapFilename); // Load map
            cout << "\n" << "Map Loaded" << endl;
            oldState = *state;
            setState("maploaded");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
            nextValidCommands->clear();
            nextValidCommands->push_back("loadmap");
            nextValidCommands->push_back("validatemap");
        } else {
            cout << "\n\n" << "Invalid map specified! Please Try Again." << endl;
            oldState = *state;
            setState("start");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
            nextValidCommands->clear();
            nextValidCommands->push_back("loadmap");
        }
    } else if (command == "validatemap") {
        if (map->validate()) {
            cout << "Generated Map is a connected graph!\n" << endl;
            oldState = *state;
            setState("mapvalidated");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
            nextValidCommands->clear();
            nextValidCommands->push_back("addplayer");
        } else {
            cout << "Error! Generated Map is not connected graph!\n" << endl;
            setState("maploaded");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
            nextValidCommands->clear();
            nextValidCommands->push_back("loadmap");
            nextValidCommands->push_back("validatemap");
        }

    } else if (command == "addplayer") {
        string playername = playerName;
        if(playerName.empty()) {
            cout << "\n" << "No player name specified! Please Try Again." << endl;
            oldState = *state;
            setState("mapvalidated");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
            nextValidCommands->clear();
            nextValidCommands->push_back("addplayer");
        } else {
            noOfPlayers++; // Keep track of the number of players
            playerName.clear();
            // Player creation
            Player* player = new Player(playername);
            player->setpID(noOfPlayers);
            players.push_back(player);

            cout << "Current Number of players: " << noOfPlayers << endl; // Display number of players
            oldState = *state;
            setState("playersadded");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
            nextValidCommands->clear();

            // Check if the correct number of players are playing
            if(noOfPlayers < 6)
                nextValidCommands->push_back("addplayer"); // Stop from adding more than 6 players
            if(noOfPlayers >= 2)
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
        setState("Assign Reinforcement");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.back()->saveEffect(transitionString);
        std::cout << "\nNow Entering the Play Phase!\n" << std::endl;
        nextValidCommands->clear();
    }
    
}

// Defining the support function responsible for displaying the current transition taking place
std::string GameEngine::displayTransition(std::string state, std::string *newState, std::string command) {
    string transitionString = "Current state: " + state + ". Command: " + command
                                + ". Transition to state: " + *newState;
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
void GameEngine::transition() {

    commandProcessor->getCommand();
   
    string command = commandProcessor->commands.back()->getCommand();
    string delimiter = " ";
    
    if(command.find(' ') != std::string::npos) {
        string token = command.substr(command.find(delimiter)).erase(0,delimiter.length());;
        command = command.substr(0,command.find(delimiter));
        if(getState() == "start" || getState() == "maploaded") {
            mapFilename = token;
        }
        else if(getState() == "mapvalidated" || getState() == "playersadded") {
            playerName = token;
        }
        
    }
    bool valid = commandProcessor->checkIfValidCommand(command);

    if(valid) {

        if(validTransition(command)) {
            doTransition(command);
            notify(this);
        } else {
            commandProcessor->commands.back()->saveEffect("Not a valid transition. Cannot " + command + " at state " + *state);
            std::cout << commandProcessor->commands.back()->getEffect() << std::endl;
            
        }

    } else {
        commandProcessor->commands.back()->saveEffect("Command entered is not valid. State remains unchanged.");
        std::cout << commandProcessor->commands.back()->getEffect() << std::endl;
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

bool GameEngine::fileExist (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

std::string GameEngine::stringToLog() {
    return "Current State is now: " + this->getState();
}

void GameEngine::reinforcementPhase(vector<Territory*> map, vector<Player*> players) {
    cout << "Starting the Reinforcement Phase" << endl;
    vector <Territory*> Owned;
    int temp = 0;
    double count = 0;

    for (int i = 0; i < players.size(); i++) {
        temp = players[i]->getReinforcementPool();
        Owned = players[i]->getTerritories();

        // count the terriorties number
        for (int j = 0; j < players[i]->getTerritories().size(); j++) {
            count++;
        }

        // count number of territores per contintent in the world map
        std::unordered_map<unsigned, size_t> countMap1;
        for (auto v : map)
            ++countMap1[v->continentNumber];

        // count number of territores per contintent that are owned by the players
        std::unordered_map<unsigned, size_t> countOwned;
        for (auto v : Owned)
            ++countOwned[v->continentNumber];

        // check if they have the entire continent
        // bonus is the number of territories in the contintent/3, more territories in continent = more bonus
        for (auto const &k : countMap1) {
            for (auto const &l : countOwned) {
                if(k.first == l.first) {
                    if(k.second == l.second) {
                        temp += (int)floor(l.second/3);
                        cout << "Player with pID: " << players[i]->getPID() << " has all the territories in contintent: " << l.first << endl;
                        cout << "A bonus of "<< (int)floor(l.second/3) << " armies been awarded!\n" << endl;
                    }
                }
            }
        }
        // reinforcements based on number of territories
        temp += (int)floor(count/3);

        // minimal number of reinforcements
        temp += 3;
        players[i]->setReinforcementPool(temp);

        // reset variables
        temp = 0;
        count = 0;

        cout << "Player with pID: " << players[i]->getPID() << " has " << players[i]->getReinforcementPool() << " armies" << endl;
    }
}

void GameEngine::issueOrderPhase(vector<Player*> player,Player* targetPlayer, Deck* deck)
{
    for (int i=0; i<player.size(); i++)
    {
        player[i]->issuingOrder(player[i],targetPlayer, deck);
    }

}
void GameEngine::executeOrderPhase(vector<Player*> player)
{    
    cout<<"\nStarting Execute Order Phase..."<<endl;
    Order *currentOrder;
    Order *narrative;
    OrderList narrativeOrder;
    for (int i=0; i<player.size(); i++)
    {
        //std::cout << "Player "<<player[i]->getPID() <<"'s order list: \n" << *player[i]->getOrderList() <<std::endl;
        for (int j=0; j<player[i]->getOrderList()->getSize(); i++)
        {    
            std::string str (player[i]->getOrderList()->getIndex(j)->getOrderType() + " ");
            currentOrder = player[i]->getOrderList()->getIndex(j);
            currentOrder->execute();
            string isValid = currentOrder->validate() ? "true" : "false";
            narrative = new Order(str.append(isValid));
            narrativeOrder.addOrder(narrative);
        }
    }
    
    cout<<"Narrative:\n"<<narrativeOrder<<endl;
}




void GameEngine::mainGameLoop(vector<Territory*> map,vector<Player*> players, Deck* deck,int mapSize)
{
    srand (time(NULL));
    while (true)
    {
        int targetPlayer = rand() % players.size()-1 + 0;
        reinforcementPhase(map,players);
        issueOrderPhase(players,players[targetPlayer],deck);
        executeOrderPhase(players);
        int i = 0;
        i++;
        if (players[i]->defendList.size()==0) //if a player does not control any territories then the player is removed
        {
            delete players[i];
            cout<<"size of players: "<<players.size()<<endl;
        }
        if (players[i]->getTerritories().size() == mapSize)//if player controls all the territories then game ends and winner is announced
        {
            cout<<"Game Over"<<endl;
            cout<<"Player " << players[i]->getPID() << " won" << endl;
            break;
        }
        
    }
}
