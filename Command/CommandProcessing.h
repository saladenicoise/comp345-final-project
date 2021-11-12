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
    Command(const Command& comObj);
    Command& operator=(const Command& command);
    Command(string command);
    ~Command();
    void setCommand(string command);
    string getCommand();
    void saveEffect(string effect);
    string getEffect();
    friend std::ostream& operator<<(std::ostream& out, const Command& comObj);
};

class CommandProcessor {

private:
    void saveCommand(string command);
public:
    vector<Command*> commands;
    CommandProcessor();
    CommandProcessor(CommandProcessor& comProcObj);
    CommandProcessor& operator=(const CommandProcessor& commandProcessor);
    ~CommandProcessor();
    void getCommand();
    virtual string readCommand();
    bool checkIfValidCommand(string command);
    bool validate(string command, vector<string> nextValidCommands);
    friend std::ostream& operator<<(std::ostream& out, const CommandProcessor& comObj);
};


class FileLineReader {
private:
    string filename;
    fstream myFile;
public:
    FileLineReader(string filename);
    FileLineReader(const FileLineReader& flr);
    FileLineReader& operator=(const FileLineReader& flr);
    ~FileLineReader();
    string readLineFromFile();
    friend std::ostream& operator<<(std::ostream& out, const FileLineReader& flr);
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* flr;
    virtual string readCommand() override;
public:
    FileCommandProcessorAdapter(FileLineReader flr);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcpa);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fcpa);
    ~FileCommandProcessorAdapter();
    friend std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& fcpa);
};






#endif
