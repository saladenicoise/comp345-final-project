#include "CommandProcessing.h"
#include <string>
using std::string;
using std::stoi;
#include <regex>
using std::regex;
using std::sregex_token_iterator;

Command::Command() {} // Default constructor for Command class.

Command::Command(const Command &comObj) { // Copy constructor for Command class.
    command = comObj.command;
    effect = comObj.effect;
}

Command::Command(string command) {  // Parameterized constructor for Command class.
    this->command = command;
    this->effect = "No Effect Yet!";
}

void Command::saveEffect(string effect) {
    this->effect = effect;
    notify(this);
}

string Command::getEffect() {
    return effect;
}

void Command::setCommand(string command) {
    this->command = command;
}

string Command::getCommand() {
    return command;
}

Command::~Command() {}


std::string Command::stringToLog() {
    return getEffect();
}

/* Defining methods for Command Processor Class. */

CommandProcessor::CommandProcessor() {

}

CommandProcessor::CommandProcessor(const CommandProcessor &comProcObj) {
    for(int i = 0; i < comProcObj.commands.size(); i++) {
        Command command = *comProcObj.commands.at(i);
        commands.push_back(new Command(command));
    }
}

CommandProcessor::~CommandProcessor() {
    for(int i = 0; i < commands.size(); i++) {
        delete commands.at(i);
        commands.at(i) = nullptr;
    }
}

string CommandProcessor::readCommand() {
    string commandString;
    Command command;

    cout << "Type in your command: ";

    getline(cin, commandString);

    return commandString;

}

std::string CommandProcessor::stringToLog() {
    return "Command Saved: " + commands.back()->getCommand();
}

void CommandProcessor::saveCommand(string commandString) {
    Command* command = new Command(commandString);
    commands.push_back(command);
    notify(this);
}

void CommandProcessor::getCommand() {
    string commandString = readCommand();

    saveCommand(commandString);

}

bool CommandProcessor::checkIfValidTourneyCommand(string command) {
    vector<string> mapFiles;
    vector<string> playerStrategies;
    int numOfGames = 0;
    int maxNumOfTurns = 0;
    regex delim("\\-");
    vector<string> out(sregex_token_iterator(command.begin(), command.end(), delim, -1), sregex_token_iterator());
    for(int i = 0; i < out.size(); i++) {
        if(out[i].rfind("M", 0) == 0) {//Map Files
            regex mapDelim(" ");
            vector<string> mapFiless(sregex_token_iterator(out[i].begin(), out[i].end(), mapDelim, -1), sregex_token_iterator());
            for(string mapF : mapFiless) {
                mapFiles.push_back(mapF);
            }
        }
        if(out[i].rfind("P", 0) == 0) {//Player Strategies
            regex playerDelim(" ");
            vector<string> playerStrategiess(sregex_token_iterator(out[i].begin(), out[i].end(), playerDelim, -1), sregex_token_iterator());
            //Check if strategies are valid
            for(string strategy : playerStrategiess) {
                if(strategy != "P") {
                    if (!(strategy == "Aggressive" || strategy == "Benevolent" || strategy == "Neutral" || strategy == "Cheater")) {//Not valid strategy
                        cout << strategy << " is not a valid strategy type only: Aggresive, Benevolent, Neutral, or Cheater are accepted." << endl;
                        return false;
                    }
                    playerStrategies.push_back(strategy);
                }
            }
        }
        if(out[i].rfind("G", 0) == 0) {//Number of Games
            numOfGames = stoi(out[i].substr(2, out[i].length()));
        }
        if(out[i].rfind("D", 0) == 0) {//Number of turns
            maxNumOfTurns = stoi(out[i].substr(2, out[i].length()));
        }
    }
    //At least: 1 map, 2 player strategies, 1 Game, 1 Round
    if((mapFiles.size() > 0 && mapFiles.size() < 6) && (playerStrategies.size() > 1 && playerStrategies.size() < 5)&& (numOfGames > 0 && numOfGames < 6) && (maxNumOfTurns > 9 && maxNumOfTurns < 51)) {
        cout << "Valid Tournament Command" << endl;
        return true;
    }else{
        cout << "Map Files Size(): " << mapFiles.size() << endl;
        cout << "Map Files Condition: " << (mapFiles.size() > 0 && mapFiles.size() < 6) << endl;
        cout << "Player Strats Size: " << playerStrategies.size() << endl;
        cout << "Player Strats Condition: " << (playerStrategies.size() > 1 && playerStrategies.size() < 5) << endl;
        cout << "Number of Games: " << numOfGames << endl;
        cout << "Number of Games Condition: " << (numOfGames > 0 && numOfGames < 6) << endl;
        cout << "Max Num of Turns: " << maxNumOfTurns << endl;
        cout << "Max Num of Turns Condition: " << (maxNumOfTurns > 9 && maxNumOfTurns < 51) << endl;
        return false;
    }
}

bool CommandProcessor::checkIfValidCommand(string command) {
    if (command.substr(0, 10) == "tournament" || command == "start" || command == "loadmap" || command == "validatemap" || command == "addplayer"
        || command == "gamestart" || command=="tourneystart" || command=="endtourney" || command == "replay" || command == "quit" || command == "tourney") {

        return true;
    }
    return false;
}

bool CommandProcessor::validate(string command, vector<string> nextValidCommands) {
    for(int i = 0; i < nextValidCommands.size(); i++) {
        if(command == nextValidCommands.at(i)) {
            return true;
        }
    }
    return false;
}

FileLineReader::FileLineReader(string filename) {
    this->filename = filename;
    myFile.open(filename, ios::in | ios::binary);
}

FileLineReader::FileLineReader(const FileLineReader& flr) {
    filename = flr.filename;
    myFile.open(filename);
}

FileLineReader::~FileLineReader(){}

string FileLineReader::readLineFromFile () {
    string command;
    if(myFile.is_open()) {
        if(myFile) {
            getline(myFile, command);
            cout << "This is the command: " << command << endl;
        } else {
            cout << "Closing File" << endl;
            myFile.close();
        }
    } else {
        command = "File is Closed.";
    }

    return command;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader flr) {
    this->flr = new FileLineReader(flr);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
    flr = nullptr;
}

string FileCommandProcessorAdapter::readCommand() {
    string command = flr->readLineFromFile();

    return command;
}
