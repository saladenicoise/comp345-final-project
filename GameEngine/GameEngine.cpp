#include "GameEngine.h"
#include <unordered_map>
#include <regex>
using std::regex;
using std::sregex_token_iterator;
// Defining the default constructor for GameEngine class
GameEngine::GameEngine(string mode) {
    state = new std::string("pregame");
    nextValidCommands = new std::vector<std::string>;
    nextValidCommands->push_back("start");
    nextValidCommands->push_back("tournament");
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
    bool valid = false;
    std::string oldState;
    std::string transitionString;
    std::string tournamentCommand;
    if (command == "tournament") {//Does it start with tournament
        oldState = *state;
        setState("tournament");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.back()->saveEffect(transitionString);
        nextValidCommands->clear();
        while (!valid) {
            cout << "Enter Tournament Command: ";
            getline(cin, tournamentCommand);
            if(commandProcessor->checkIfValidTourneyCommand(tournamentCommand)) {
                valid = true;
                nextValidCommands->push_back("tourneystart");
            } else {
                cout << "Invalid tournament command, try again" << endl;
            }
        }
    }

    // Declare variables
    load_map = new MapLoader();
    deck = new Deck(30);

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

    }else if (command == "tourneystart"){
        oldState = *state;
        setState("tourneystart");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.back()->saveEffect(transitionString);
        std::cout << "\nTOURNAMENT NOW STARTING!" << std::endl;
        std::cout << "LETS GET READY TO RUMBLE!!!!!!\n" << std::endl;
        nextValidCommands->clear();
        tournamentMode(tournamentCommand);
    }else if (command == "gamestart") { // Start the game

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
                    (*it)->defendList.push_back(randTerritory);
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

        mainGameLoop(map->territories,players,deck, map->territories.size(), this);
    }
    
}

/**
 * Parses the command and calls the main tournament loop
 * @param command The tournament command
 */
void GameEngine::tournamentMode(std::string command) {
    vector<string> mapFiles;
    vector<string> playerStrategies;
    int numOfGames = 0;
    int maxNumOfTurns = 0;
    regex delim("\\-");
    vector<string> out(sregex_token_iterator(command.begin(), command.end(), delim, -1), sregex_token_iterator());
    for(int i = 0; i < out.size(); i++) {
        if(out[i].rfind("M", 0) == 0) {//Map Files
            regex mapDelim(" ");
            vector<string> mapFiles(sregex_token_iterator(out[i].begin(), out[i].end(), mapDelim, -1), sregex_token_iterator());
        }
        if(out[i].rfind("P", 0) == 0) {//Player Strategies
            regex playerDelim(" ");
            vector<string> playerStrategies(sregex_token_iterator(out[i].begin(), out[i].end(), playerDelim, -1), sregex_token_iterator());
        }
        if(out[i].rfind("G", 0) == 0) {//Number of Games
            numOfGames = stoi(out[i].substr(2, out[i].length()));
        }
        if(out[i].rfind("D", 0) == 0) {//Number of turns
            maxNumOfTurns = stoi(out[i].substr(2, out[i].length()));
        }
    }
    tournamentLoop(mapFiles, playerStrategies, numOfGames, maxNumOfTurns);
}

void GameEngine::tournamentLoop(vector<string> mapFiles, vector<string> playerStrategies, int numOfGames, int maxNumOfRounds) {
    string report[mapFiles.size()+1][numOfGames+1];
    //Prep report:
    report[0][0] = "";
    int curMapNum = 1;
    for(string mapF : mapFiles) { //Loop through every map file given
        report[curMapNum][0] = mapF;
        MapLoader mapLoader;
        Map* map = mapLoader.loadMap(mapF+".map");
        vector<Player *> players;
        int pId = 1;
        for(string playerStrat : playerStrategies) { //Create each player with the appropriate strategy
            Player* player = new Player(playerStrat);
            player->setpID(pId);
            if(playerStrat == "Aggressive") { //Not implemented yet
                //player->setStrategy(new AggresivePlayerStrategy());
                //player->setStrategyString("Aggressive")
            }
            if(playerStrat == "Benevolent") {
                player->setStrategy(new BenevolentPlayerStrategy());
                player->setStrategyString("Benevolent");
            }
            if(playerStrat == "Neutral") {
                player->setStrategy(new NeutralPlayerStrategy());
                player->setStrategyString("Neutral");
            }
            if(playerStrat == "Cheater") {
                player->setStrategy(new CheaterPlayerStrategy());
                player->setStrategyString("Cheater");
            }
            players.push_back(player);
        }
        // Make copy of the territories
        vector<Territory*> copiedTerritories(map->territories.size());
        for(int i = 0; i < map->territories.size(); i++){
            copiedTerritories[i] = map->territories[i];
        }

        //For every game we give them new territories
        for(int curGame = 1; curGame < numOfGames+1; curGame++) {
            report[0][curGame] = "Game " + curGame;
            // a) fairly distribute all the territories to the players
            while(copiedTerritories.size() != 0){
                for(auto it = players.begin(); it != players.end(); ++it){
                    if(copiedTerritories.size() != 0) {
                        int random = rand() % copiedTerritories.size();
                        Territory* randTerritory = copiedTerritories[random];
                        randTerritory->setOwnerId((*it)->getPID());
                        (*it)->t.push_back(randTerritory);
                        (*it)->defendList.push_back(randTerritory);
                        copiedTerritories.erase(copiedTerritories.begin() + random);
                        copiedTerritories.shrink_to_fit();
                    }
                }
            }

            // Display players' territories and reinforcement pool
            for(auto it = players.begin(); it != players.end(); ++it){
                // c) give 50 initial armies to the players, which are placed in their respective reinforcement pool
                (*it)->setReinforcementPool(50);

                // d) let each player draw 2 initial cards from the deck using the deck’s draw() method
                for (int i = 0; i < 2; i++) {
                    deck->draw((*it)->getHand());
                }
            }
            // b) determine randomly the order of play of the players in the game
            unsigned seed = std::chrono::system_clock::now()
                    .time_since_epoch()
                    .count();
            std::shuffle(std::begin(players), std::end(players), std::default_random_engine());

            int round = 1;
            srand(time(NULL));
            bool winner = false;
            while(round < maxNumOfRounds+1) {
                cout << "------ Tournament Game " << curGame << " | Round: " << round << " ------" << endl;
                reinforcementPhase(map->territories, players);
                issueOrderPhase(players, this, deck);
                executeOrderPhase(players);
                for(int f = 0; f < players.size(); f++) {
                    if(players[f]->defendList.size() == 0) {
                        cout << "Player " << players[f]->getPID() << " has been eliminated" << endl;
                        delete players[f];
                        players[f] = nullptr; //Avoid dangling pointers
                    }
                    if(players[f]->getTerritories().size() == map->territories.size()) {
                        cout << "Game " << curGame << " has been won by " << players[f] << endl;
                        report[curMapNum][curGame] = players[f]->getPlayerStrategyString();
                        winner = true;
                        break;
                    }
                }
            }
            if(winner == false) {//No one won and we ran out of rounds thus draw
                report[curMapNum][curGame] = "Draw";
            }
        }
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

void GameEngine::issueOrderPhase(vector<Player*> player,GameEngine *game, Deck* deck)
{
    for (int i=0; i<player.size(); i++)
    {
        player[i]->issuingOrder(player[i],game, deck);
    }

}
void GameEngine::executeOrderPhase(vector<Player*> player)
{    
    cout<<"\nStarting Execute Order Phase..."<<endl;
    Order *currentOrder;
    OrderList* orderlist;
    for (int i=0; i<player.size(); i++)
    {
        std::cout << "Player "<<player[i]->getPID() <<"'s order list: \n" << *player[i]->getOrderList() <<std::endl;
        for (int j=0; j<player[i]->getOrderList()->getSize(); j++)
        {    
            currentOrder = player[i]->getOrderList()->getIndex(j);
            currentOrder->execute();
        }
        player[i]->olst->deleteAllOrder();     //deletes order list of each player for next turn 
    }
}

vector<Player*> GameEngine::getPlayersList(){ // get players
	return players;
}

void GameEngine::mainGameLoop(vector<Territory*> map,vector<Player*> players, Deck* deck,int mapSize, GameEngine *game)
{
    int count = 1;
    srand (time(NULL));
    for(auto p : players){
        p->setStrategy(new HumanPlayerStrategy());
        p->toAttack(p->getAttackList(),*p);
        p->toDefend(p->getDefendList(),*p);
    }
    while (true)
    {
        cout<<"-----------------------------------------------------"<<endl;
        cout<<"Turn "<<count<<endl;
        reinforcementPhase(map,players);
        issueOrderPhase(players,game,deck);
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
        count++;
    }
}
