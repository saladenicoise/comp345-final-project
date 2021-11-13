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

Command &Command::operator=(const Command &comObj) {
    this->command = comObj.command;
    this->effect = comObj.effect;

    return *this;
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

// stream insertion operator overloading function.
std::ostream& operator<< (std::ostream& out, const Command& comObj) {
    out << "Command: " << comObj.command << endl;
    out << "Effect: " << comObj.effect << endl;

    return out;
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

// Assignment operator overload function.
CommandProcessor &CommandProcessor::operator=(const CommandProcessor &comProcObj) {
    for(int i = 0; i < comProcObj.commands.size(); i++) {
        Command command = *comProcObj.commands.at(i);
        commands.push_back(new Command(command));
    }

    return *this;
}

// Insertion operator overload function.
std::ostream& operator<<(std::ostream& out, const CommandProcessor& comObj) {
    for(int i = 0; i < comObj.commands.size(); i++) {
        out << comObj.commands.at(i)->getEffect() << endl;
    }
    return out;
}

CommandProcessor::~CommandProcessor() {
    for(int i = 0; i < commands.size(); i++) {
        delete commands.at(i);
        commands.at(i) = nullptr;
    }
}

// getCommand function for reading and saving the command.
void CommandProcessor::getCommand() {
    string commandString = this->readCommand();

    saveCommand(commandString);
}

// readCommand function for getting command from the user.
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

FileLineReader::FileLineReader(const FileLineReader& flr) {
    filename = flr.filename;
    myFile.open(filename);
}

// Assignment operator overload function
FileLineReader &FileLineReader::operator=(const FileLineReader &flr) {
    filename = flr.filename;
    if(!myFile.is_open())
        myFile.open(filename);

    return *this;
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

// Insertion operator overload function
std::ostream& operator<<(std::ostream& out, const FileLineReader& flr) {
    out << "File being processed: " << flr.filename << endl;
    return out;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader flr) {
    this->flr = new FileLineReader(flr);
}

// Copy constructor for FileCommandProcessorAdapter class.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa) {
    this->flr = new FileLineReader(*fcpa.flr);
}

// Assignment operator overload function.
FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &fcpa) {
    this->flr = new FileLineReader(*fcpa.flr);
    return *this;
}

// Insertion operator overload function.
std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& fcpa) {
    out << "FileCommandProcessorAdapter for reading commands from files" << endl;

    return out;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
    flr = nullptr;
}

string FileCommandProcessorAdapter::readCommand() {
    string command = flr->readLineFromFile();

    return command;
}

