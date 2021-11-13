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
    // Declaration of Default Constructor for Command Class.
    Command();
    // Copy Constructor for Command Class.
    Command(Command& comObj);
    // Declaration of parameterized Command Constructor.
    Command(string command);
    // Declaration of Command Destructor.
    ~Command();
    // Declaration of Setter function for command attribute.
    void setCommand(string command);
    // Declaration of Getter function for command attribute.
    string getCommand();
    // Declaration of Function used for saving the command's effect;
    void saveEffect(string effect);
    // Declaration of Getter function for effect attribute.
    string getEffect();
    // Declaration of Insertion operator overload function.
    friend std::ostream& operator<<(std::ostream& out, const Command& comObj);
    //Logging Function
    std::string stringToLog();
    // Declaration of Assignment Operator Overload function.
    Command& operator=(const Command& command);

};

class CommandProcessor : public ILoggable, public Subject {

private:
    // Declaration of Function for adding the commands entered by the user to the commands vector.
    void saveCommand(string command);
public:
    // Declaration of Commands vector used to store commands entered by the user.
    vector<Command*> commands;
    // Declaration of Default Constructor for Command Processor.
    CommandProcessor();
    // Declaration of Copy Constructor for Command Processor.
    CommandProcessor(CommandProcessor& comProcObj);
    //Destructor
    ~CommandProcessor();
    // Declaration of function for getting the command from the user and storing it in the command's vector.
    void getCommand();
    // Declaration of function to get commands from user.
    virtual string readCommand();
    // Declaration of function to check the validity of a command.
    bool checkIfValidCommand(string command);
    // Declaration of function to check the validity of a command in the current state of the game.
    bool validate(string command, vector<string> nextValidCommands);
    //Logging Function
    std::string stringToLog();
    // Declaration of Assignment operator overload function.
    CommandProcessor& operator=(const CommandProcessor& commandProcessor);
};

class FileLineReader {
private:
    string filename;
    fstream myFile;
public:
    // Declaration of Parameterized Constructor for the class FileLineReader.
    FileLineReader(string filename);
    // Declaration of Copy Constructor for the FileLineReader class.
    FileLineReader(const FileLineReader& flr);
    // Declaration of FileLineReader Destructor.
    ~FileLineReader();
    // Declaration of function responsible for reading a line from a file.
    string readLineFromFile();
    // Declaration of Assignment Operator Overload Function.
    FileLineReader& operator=(const FileLineReader& flr);
    friend std::ostream& operator<<(std::ostream& out, const FileLineReader& flr);
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileLineReader* flr;
    // Declaration of overridden function readCommand.
    virtual string readCommand() override;
public:
    // Declaration of Parameterized Constructor for FileCommandProcessorAdapter.
    FileCommandProcessorAdapter(FileLineReader flr);
    // Declaration of Copy Constructor for FileCommandProcessorAdapter.
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcpa);
    // Declaration of Destructor for FileCommandProcessorAdapter.
    ~FileCommandProcessorAdapter();
    // Declaration of Assignment Operator Overload function.
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fcpa);
    // Declaration of Insertion Operator Overload function.
    friend std::ostream& operator<<(std::ostream& out, const FileCommandProcessorAdapter& fcpa);
};

#endif
