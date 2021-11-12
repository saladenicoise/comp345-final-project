#include "CommandProcessing.h"

Command::Command() {} // Default constructor for Command class.

Command::Command(Command &comObj) { // Copy constructor for Command class.
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

CommandProcessor::CommandProcessor(CommandProcessor &comProcObj) {
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
    return commands.back()->getCommand();
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

bool CommandProcessor::checkIfValidCommand(string command) {
    if (command == "start" || command == "loadmap" || command == "validatemap" || command == "addplayer"
        || command == "gamestart" || command == "replay" || command == "quit" ) {

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

