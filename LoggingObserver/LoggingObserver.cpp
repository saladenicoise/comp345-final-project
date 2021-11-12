//
// Created by Ryan on 2021-11-08.
//

#include "LoggingObserver.h"
Observer::Observer() {

}
LogObserver::LogObserver(){
    gameLog = std::ofstream("gamelog.txt");

};

LogObserver::~LogObserver() {
    gameLog.close();
}

void LogObserver::update(ILoggable *i) {
    gameLog << i->stringToLog() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const LogObserver& territory){
    return os << "This is the games log observer";
}

LogObserver& LogObserver::operator=(const LogObserver&logObserver){
    this->gameLog = std::ofstream("gameLog.txt");
    return *this;
}

LogObserver::LogObserver(LogObserver &logObserver) {
    this->gameLog = std::ofstream("gameLog.txt");
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