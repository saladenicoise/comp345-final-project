//
// Created by Ryan on 2021-11-08.
//

#ifndef C345A2_LOGGINGOBSERVER_H
#define C345A2_LOGGINGOBSERVER_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class ILoggable{
public:
    virtual std::string stringToLog() = 0; // creates and returns a string to be output to the log file
};

class Observer{
public:
    Observer(); // default contructor 
    virtual ~Observer(){}; // destructor
    virtual void update(ILoggable*) = 0; // pure virtual function to update
};

class Subject{

public:
    std::vector<Observer*> observers; // vector of observer objects
   virtual void notify(ILoggable*); // notify for update
   void attach(Observer*);
   void detach(Observer*);

};


class LogObserver: public Observer {
public:
    std::ofstream gameLog; // stream for output file
    LogObserver(); // constructor
    LogObserver(LogObserver& logObserver); // constructor
    LogObserver& operator= (const LogObserver& logObserver); // assignment operator
    ~LogObserver(); // destructor
    void update(ILoggable*); 
    friend std::ostream& operator<< (std::ostream& os, const LogObserver& logObserver); // overload os stream operator
};


#endif //C345A2_LOGGINGOBSERVER_H
