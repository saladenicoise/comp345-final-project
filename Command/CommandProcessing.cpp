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
    cout << "Destroyed CommandProcessor Object Instance" << endl;
}

string CommandProcessor::readCommand() {
    string commandString;
    Command command;

    cout << "Type in your command: ";

    getline(cin, commandString);

    return commandString;

}

void CommandProcessor::saveCommand(string commandString) {
    Command* command = new Command(commandString);
    commands.push_back(command);
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