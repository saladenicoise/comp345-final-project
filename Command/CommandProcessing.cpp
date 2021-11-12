#include "CommandProcessing.h"

Command::Command() {} // Default constructor for Command class.

Command::Command(const Command &comObj) { // Copy constructor for Command class.
    command = comObj.command;
    effect = comObj.effect;
}

Command &Command::operator=(const Command &comObj) {
    this->command = comObj.command;
    this->effect = comObj.effect;

    return *this;
}



Command::Command(string command) {  // Parameterized constructor for Command class.
    this->command = command;
    this->effect = "No Effect Yet!";
}

// saveEffect method for Command class
void Command::saveEffect(string effect) {
    this->effect = effect;
}

//getEffect for Command class
string Command::getEffect() {
    return effect;
}

// setter method for command attribute
void Command::setCommand(string command) {
    this->command = command;
}

// getter method for command attribute
string Command::getCommand() {
    return command;
}

// stream insertion operator overloading function.
std::ostream& operator<< (std::ostream& out, const Command& comObj) {
    out << "Command: " << comObj.command << endl;
    out << "Effect: " << comObj.effect << endl;

    return out;
}

// Destructor for Command class
Command::~Command() {}



/* Defining methods for Command Processor Class. */

// Default constructor for CommandProcessor class.
CommandProcessor::CommandProcessor() {}

// Copy constructor for CommandProcessor class.
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

// Destructor for CommandProcessor class.
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

// saveCommand function for saving the commands in the commands vector.
void CommandProcessor::saveCommand(string commandString) {
    Command* command = new Command(commandString);
    commands.push_back(command);
}

// function for checking if a command is valid
bool CommandProcessor::checkIfValidCommand(string command) {
    if (command == "start" || command == "loadmap" || command == "validatemap" || command == "addplayer"
        || command == "gamestart" || command == "replay" || command == "quit" ) {

        return true;
    }
    return false;
}

//function for checking if the command is valid in the current state.
bool CommandProcessor::validate(string command, vector<string> nextValidCommands) {
    for(int i = 0; i < nextValidCommands.size(); i++) {
        if(command == nextValidCommands.at(i)) {
            return true;
        }
    }
    return false;
}

// FileLineReader parameterized constructor.
FileLineReader::FileLineReader(string filename) {
    this->filename = filename;
}

// FileLineReader copy constructor
FileLineReader::FileLineReader(const FileLineReader &flr) {
    filename = flr.filename;
    myFile.open(filename, ios::in | ios::binary);
}

// Assignment operator overload function
FileLineReader &FileLineReader::operator=(const FileLineReader &flr) {
    filename = flr.filename;
    if(!myFile.is_open())
        myFile.open(filename);

    return *this;
}

// Destructor for FileLineReader class.
FileLineReader::~FileLineReader(){}

// Function for reading a line from a file.
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

// Parameterized constructor for FileCommandProcessorAdapter class.
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

// Destructor for FileCommandProcessorAdapter class.
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
    flr = nullptr;
}

// readCommand override for reading a command from a file.
string FileCommandProcessorAdapter::readCommand() {
    string command = flr->readLineFromFile();

    return command;
}