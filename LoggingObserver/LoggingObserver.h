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
    virtual std::string stringToLog() = 0;
};

class Observer{
public:
    Observer();
    virtual ~Observer(){};
    virtual void update(ILoggable*) = 0;
};

class Subject{

public:
    std::vector<Observer*> observers;
   virtual void notify(ILoggable*);
   void attach(Observer*);
   void detach(Observer*);

};


class LogObserver: Observer {
public:
    std::ofstream gameLog;
    LogObserver();
    ~LogObserver();
    void update(ILoggable*);
};


#endif //C345A2_LOGGINGOBSERVER_H
