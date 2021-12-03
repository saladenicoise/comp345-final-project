#include "GameEngine.h"
#include <unordered_map>
#include <regex>
using std::regex;
using std::sregex_token_iterator;
// Defining the default constructor for GameEngine class

std::string tournamentCommand;
GameEngine::GameEngine(string mode) {
    state = new std::string("pregame");
    nextValidCommands = new std::vector<std::string>;
    nextValidCommands->push_back("start");
    nextValidCommands->push_back("tourney");
    ordersToExecute = new std::vector<std::string>;
    commandProcessor = new CommandProcessor();
    if(mode == "console") {
       commandProcessor = new CommandProcessor();
    } else {
        FileLineReader *flr = new FileLineReader(mode);
        skipTourneyCommandInput = true;
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


    if(command == "endtourney") {
        oldState = *state;
        setState("TourneyEnd");
        displayTransition(oldState, state, command);
        nextValidCommands->clear();
        ordersToExecute->clear();
        nextValidCommands->push_back("quit");
    }
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
        exit(0);
    }

}

void GameEngine::startupPhase(std::string command) {
    bool valid = false;
    std::string oldState;
    std::string transitionString;
    if(command == "tourney") {
        oldState = *state;
        setState("tourney");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.back()->saveEffect(transitionString);
        nextValidCommands->clear();
        nextValidCommands->push_back("tournament");
    }else if(command.substr(0, 10) == "tournament" && !skipTourneyCommandInput) {//User Input Required
            oldState = *state;
            setState("tournament");
            transitionString = displayTransition(oldState, state, command);
            commandProcessor->commands.back()->saveEffect(transitionString);
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
    }else if(command == "tournament" && !tournamentCommand.empty()) {//Is tournament command empty? if no, that means it was validated when read from the file
        oldState = *state;
        setState("tournament");
        transitionString = displayTransition(oldState, state, command);
        commandProcessor->commands.back()->saveEffect(transitionString);
        nextValidCommands->clear();
        nextValidCommands->push_back("tourneystart");
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
    cout << "In Tournament Mode" << endl;
    vector<string> mapFiles;
    vector<string> playerStrategies;
    MapLoader mapLoader;
    int numOfGames = 0;
    int maxNumOfTurns = 0;
    regex delim("\\-");
    vector<string> out(sregex_token_iterator(command.begin(), command.end(), delim, -1), sregex_token_iterator());
    for(int i = 0; i < out.size(); i++) {
        if(out[i].rfind("M", 0) == 0) {//Map Files
            regex mapDelim(" ");
            vector<string> mapFiless(sregex_token_iterator(out[i].begin(), out[i].end(), mapDelim, -1), sregex_token_iterator());
            for(string mapF : mapFiless) {
                if(mapF != "M" && mapLoader.loadMap(mapF)->validate()) {
                    if(mapF.size() > biggestMapNameSize) {
                        biggestMapNameSize = mapF.size();
                    }
                    mapFiles.push_back(mapF);
                    mapList += mapF + " ";
                }
            }
        }
        if(out[i].rfind("P", 0) == 0) {//Player Strategies
            regex playerDelim(" ");
            vector<string> playerStrategiess(sregex_token_iterator(out[i].begin(), out[i].end(), playerDelim, -1), sregex_token_iterator());
            //Check if strategies are valid
            for(string strategy : playerStrategiess) {
                if(strategy != "P") {
                    if(strategy.size() > biggestStrategyNameSize) {
                        biggestStrategyNameSize = strategy.size();
                    }
                    playerStrategies.push_back(strategy);
                    playerList += strategy + " ";
                }
            }

        }
        if(out[i].rfind("G", 0) == 0) {//Number of Games
            numOfGames = stoi(out[i].substr(2, out[i].length()));
            numOfGamesToPrint = numOfGames;
        }
        if(out[i].rfind("D", 0) == 0) {//Number of turns
            maxNumOfTurns = stoi(out[i].substr(2, out[i].length()));
            maxRoundsToPrint = maxNumOfTurns;
        }
    }
    cout << "Tournament Loop being called" << endl;
    tournamentLoop(mapFiles, playerStrategies, numOfGames, maxNumOfTurns);
}

void GameEngine::tournamentLoop(vector<string> mapFiles, vector<string> playerStrategies, int numOfGames, int maxNumOfRounds) {
    string oldState;
    string transitionString;
    oldState = *state;
    setState("intourney");
    transitionString = displayTransition(oldState, state, "tourneystart");
    commandProcessor->commands.back()->saveEffect(transitionString);
    nextValidCommands->clear();
    nextValidCommands->push_back("endtourney");
    //Prep report:
    for(int space=0; space < biggestMapNameSize; space++) {
        report[0][0] += " ";
    }
    int curMapNum = 1;
    for(string mapF : mapFiles) { //Loop through every map file given
        cout << "Now looping through: " << mapF << endl;
        report[curMapNum][0] = mapF;
        MapLoader mapLoader;
        Map* mapL = mapLoader.loadMap(mapF);
        int pId = 1;
        for(string playerStrat : playerStrategies) { //Create each player with the appropriate strategy
            cout << "Creating a player with: " << playerStrat << endl;
            Player* player = new Player(playerStrat);
            player->setpID(pId);
            pId++;
            if(playerStrat == "Aggressive") {
                player->setStrategy(new AggressivePlayerStrategy());
                player->setStrategyString("Aggressive");
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
            this->players.push_back(player);
        }
        // Make copy of the territories
        vector<Territory*> copiedTerritories(mapL->territories.size());
        for(int i = 0; i < mapL->territories.size(); i++){
            copiedTerritories[i] = mapL->territories[i];
        }

        //For every game we give them new territories
        for(int curGame = 1; curGame < numOfGames+1; curGame++) {
            report[0][curGame] = "Game " + to_string(curGame);
            cout << "Currently running game " << curGame << endl;
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

            //Assign their attack/defend lists
            for(auto p : players){
                p->toDefend(p->getDefendList(),*p);
                p->toAttack(p->getAttackList(),*p);
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
            std::shuffle(std::begin(players), std::end(players), std::default_random_engine(seed));

            int round = 1;
            bool winner = false;
            cout << "Now undergoing rounds" << endl;
            while(round < maxNumOfRounds+1 && !winner) {
                cout << "------ Tournament Game " << curGame << " | Round: " << round << " ------" << endl;
                reinforcementPhase(mapL->territories, players);
                issueOrderPhase(players, this, deck);
                executeOrderPhase(players);
                
                int randP = std::rand() % (1 + 1 - 0) + 0;

                // if last two players are incapable of doing anything switch thier strategy
                if(players.size() == 2){
                    players.shrink_to_fit();
                    if((players[0]->getPlayerStrategy()->getStrategyName() == "Neutral" && players[1]->getPlayerStrategy()->getStrategyName() == "Neutral")
                       ||(players[0]->getPlayerStrategy()->getStrategyName() == "Benevolent" && players[1]->getPlayerStrategy()->getStrategyName() == "Neutral")
                       ||(players[0]->getPlayerStrategy()->getStrategyName() == "Benevolent" && players[1]->getPlayerStrategy()->getStrategyName() == "Benevolent")) {
                        cout << "Switching " << players[randP]->getPID() <<" to cheater" << endl;
                        players[randP]->setStrategy(new CheaterPlayerStrategy()); // switch random person
                    }
                }
                //Check if someone won
                for(int f = 0; f < players.size(); f++) {
                    if(players[f]->defendList.empty()) {
                        cout << "Player " << players[f]->getPID() << " has been eliminated" << endl;
                        delete players[f];
                        players[f] = nullptr; //Avoid dangling pointers
                    }
                    if(players[f]->getTerritories().size() == mapL->territories.size()) {
                        cout << "Game " << curGame << " has been won by Player " << players[f]->getPID() << " who was the " << players[f]->getPlayerStrategyString() <<endl;
                        report[curMapNum][curGame] = players[f]->getPlayerStrategyString();
                        winner = true;
                        break;
                    }
                }
                round++;
            }
            if(winner == false) {//No one won and we ran out of rounds thus draw
                cout << "Game Ended in a Draw!" << endl;
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
    bool tournament = false;
    bool valid = false;
    commandProcessor->getCommand();
    string command = commandProcessor->commands.back()->getCommand();
    string delimiter = " ";

    if(command.substr(0, 10) == "tournament" && command.length() > 10) {
        tournament = true;
        if(commandProcessor->checkIfValidTourneyCommand(command)) {
            tournamentCommand = command;
            valid = true;
        }else{
            std::cout << "Tournament Command Invalid, now exiting" << endl;
            exit(0);
        }
    }
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
    if(!tournament)
        valid = commandProcessor->checkIfValidCommand(command);

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
    if(this->getState() == "TourneyEnd") {
        string toRet = "Tournament Mode: \nM: " + mapList + "\nP: " + playerList + "\nG: " + to_string(numOfGamesToPrint) + "\nD: " + to_string(maxRoundsToPrint) + "\nResults:\n";
        for(int i=0; i < 6; i++) {//Rows
            if(report[i][0].empty()) {//Is the current row empty at the first entry? yes, we are done printing our report
                break;
            }
            for(int j=0; j < 6; j++) {//Cols
                if(report[i][j].empty()) {
                    toRet += "\n";
                    break;
                }else{
                    if(i == 0 && j > 0) {//First row, Second+ Column
                        toRet += report[i][j];
                        int diff = biggestStrategyNameSize-report[i][j].length()-2;
                        for(int v = 0; v < diff; v++) {
                            toRet += " ";
                        }
                        toRet += " | ";
                    }else{
                        toRet += report[i][j] + " | ";
                    }
                }
            }
        }
        return toRet;
    }
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
        player[i]->issuingOrder(player[i], game, deck);
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
            cout << "Attempting to execute: " << currentOrder->getOrderType() << endl;
            currentOrder->execute();
        }
        player[i]->olst->deleteAllOrder();     //deletes order list of each player for next turn
    }
}

vector<Player*> GameEngine::getPlayersList(){ // get players
	return players;
}

void GameEngine::setPlayersList(vector<Player*> newPlayersList){
	players = newPlayersList;
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
        int randP = std::rand() % (1 + 1 - 0) + 0;

        // if last two players are incapable of doing anything switch thier strategy
        if(players.size() == 2){
            players.shrink_to_fit();
            if((players[0]->getPlayerStrategy()->getStrategyName() == "Neutral" && players[1]->getPlayerStrategy()->getStrategyName() == "Neutral")
            ||(players[0]->getPlayerStrategy()->getStrategyName() == "Benevolent" && players[1]->getPlayerStrategy()->getStrategyName() == "Neutral")
            ||(players[0]->getPlayerStrategy()->getStrategyName() == "Benevolent" && players[1]->getPlayerStrategy()->getStrategyName() == "Benevolent")) {
                cout << "Switching " << players[randP]->getPID() <<" to cheater" << endl;
                players[randP]->setStrategy(new CheaterPlayerStrategy()); // switch random person
            }
        }
	for(int j = 0;j<players.size();j++){
            std::string change;
            std::string playerStrat;
            cout << "Switch player " <<  players[j]->getPID() << " to another strategy?, y or n" << endl;
            cin>>change;
            if(change == "y"){
                cout << "Which strategy?" << endl;
                cin>>playerStrat;
                if(playerStrat == "Aggressive") {
                    players[j]->setStrategy(new AggressivePlayerStrategy());
                    players[j]->setStrategyString("Aggressive");
                    cout << "Switching " << players[j]->getPID() <<" to aggressive" << endl;
                }
                if(playerStrat == "Benevolent") {
                    players[j]->setStrategy(new BenevolentPlayerStrategy());
                    players[j]->setStrategyString("Benevolent");
                    cout << "Switching " << players[j]->getPID() <<" to benevolent" << endl;
                }
                if(playerStrat == "Neutral") {
                    players[j]->setStrategy(new NeutralPlayerStrategy());
                    players[j]->setStrategyString("Neutral");
                    cout << "Switching " << players[j]->getPID() <<" to neutral" << endl;
                }
                if(playerStrat == "Cheater") {
                    players[j]->setStrategy(new CheaterPlayerStrategy());
                    players[j]->setStrategyString("Cheater");
                    cout << "Switching " << players[j]->getPID() <<" to cheater" << endl;
                }
                if(playerStrat == "Human") {
                    players[j]->setStrategy(new HumanPlayerStrategy());
                    players[j]->setStrategyString("Human");
                    cout << "Switching " << players[j]->getPID() <<" to human" << endl;
                }
            }
        }
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
	i++;
        count++;
    }
}
