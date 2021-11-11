#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Command {

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
};

class CommandProcessor {

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

public:
    FileCommandProcessorAdapter(FileLineReader flr);
    ~FileCommandProcessorAdapter();
    virtual string readCommand() override;


};

#endif
