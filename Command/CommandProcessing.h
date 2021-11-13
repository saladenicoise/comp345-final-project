#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H
#include <iostream>
#include <vector>
#include <fstream>
#include "../LoggingObserver/LoggingObserver.h"
using namespace std;

class Command : public ILoggable, public Subject {

private:
    string command; // variable to store the command
    string effect; // variable to store the effect
public:
    Command(); // default constructor
    Command(Command& comObj); // copy constructor
    Command(string command); // single parameter constructor
    ~Command(); // destructor
    void setCommand(string command); // mutator for command
    string getCommand(); // accessor for command
    void saveEffect(string effect); // mutator for effect
    string getEffect(); // accessor for effect
    std::string stringToLog(); // string to log function

};

class CommandProcessor : public ILoggable, public Subject {

private:
    virtual string readCommand(); // read command function
    void saveCommand(string command); // save command to command object
public:
    vector<Command*> commands; // vector of commands
    CommandProcessor(); // default constructor
    CommandProcessor(CommandProcessor& comProcObj); // copy constructor
    virtual ~CommandProcessor(); // destructor
    void getCommand(); // accessor for commandprocessor
    bool checkIfValidCommand(string command); // check if valid command
    bool validate(string command, vector<string> nextValidCommands); // validate method for next viable command
    std::string stringToLog(); // string to log function

};

class FileLineReader {
private:
    string filename; // filename var
    fstream myFile; // stream to open file
public:
    FileLineReader(string filename); // single parameterized constructor
    FileLineReader(const FileLineReader& flr); // copy constructor
    ~FileLineReader(); // destructor
    string readLineFromFile(); // function to read file
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* flr; // file reader object
    virtual string readCommand() override; // override readcommand function
public:
    FileCommandProcessorAdapter(FileLineReader flr); // single parameterized constructor
    ~FileCommandProcessorAdapter(); // destructor



};

#endif
