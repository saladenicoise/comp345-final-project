#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H
#include <iostream>
#include <vector>
#include <fstream>
#include "../LoggingObserver/LoggingObserver.h"
using namespace std;

class Command : public ILoggable, public Subject {

private:
    string command;
    string effect;
public:
    Command();
    Command(Command& comObj);
    Command(string command);
    ~Command();
    void setCommand(string command);
    string getCommand();
    void saveEffect(string effect);
    string getEffect();
    std::string stringToLog();

};

class CommandProcessor : public ILoggable, public Subject {

private:
    virtual string readCommand();
    void saveCommand(string command);
public:
    vector<Command*> commands;
    CommandProcessor();
    CommandProcessor(CommandProcessor& comProcObj);
    ~CommandProcessor();
    void getCommand();
    bool checkIfValidCommand(string command);
    bool validate(string command, vector<string> nextValidCommands);
    std::string stringToLog();

};

class FileLineReader {
private:
    string filename;
    fstream myFile;
public:
    FileLineReader(string filename);
    ~FileLineReader();
    string readLineFromFile();
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* flr;
    virtual string readCommand() override;
public:
    FileCommandProcessorAdapter(FileLineReader flr);
    ~FileCommandProcessorAdapter();
    


};

#endif
