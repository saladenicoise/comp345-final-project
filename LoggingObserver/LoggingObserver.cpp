//
// Created by Ryan on 2021-11-08.
//

#include "LoggingObserver.h"

LogObserver::LogObserver(){
    gameLog = std::ofstream("gamelog.txt");

};

LogObserver::~LogObserver() {
    gameLog.close();
}

void LogObserver::update(ILoggable *i) {
    gameLog << i->stringToLog() << std::endl;
}

void Subject::attach(Observer *i) {
    observers.push_back(i);
}

void Subject::detach(Observer *i) {

    for(int j = 0; j < observers.size(); i++){
        if(observers[j] == i) {
            observers.erase(observers.begin() + j);
        }
    }
}

void Subject::notify(ILoggable *i) { //Observer notify method for Order
    for(Observer* o : observers){
        o->update(i);
    }
}