//
// Created by Ryan on 2021-11-12.
//

#include <iostream>
#include <vector>
#include "../Map/Map.h"
#include "../Orders/Orders.h"
#include "../Command/CommandProcessing.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"
int main() {
    LogObserver* logObserver = new LogObserver;
    OrderList* orderList = new OrderList;
    Player* p = new Player("Ryan");

    Territory* t = new Territory("Fake Territory", 1, 5);
    vector<Territory*> pTerritorys;
    pTerritorys.push_back(t);
    p->setTerritories(pTerritorys);
    t->armyCount = 10;
    Blockade* blockade = new Blockade(p, t);
    CommandProcessor* commandProcessor = new CommandProcessor;
    FileLineReader* flr = new FileLineReader("commands.txt");
    FileCommandProcessorAdapter* fileCommandProcessorAdapter = new FileCommandProcessorAdapter(*flr);
    Command* command = new Command;

    GameEngine* gameEngine = new GameEngine("console");
    gameEngine->attach(logObserver);
    cout << "type start to transition" << endl;
    gameEngine->transition(); //Type start into console, since its the beginning of a gameEngine.
    commandProcessor->attach(logObserver);
    fileCommandProcessorAdapter->attach(logObserver);
    command->attach(logObserver);
    blockade->attach(logObserver);
    orderList->attach(logObserver);
    fileCommandProcessorAdapter->getCommand();
    fileCommandProcessorAdapter->getCommand();
    command->saveEffect("This is a sample effect being saved by command->saveEffect");
    cout << "type a command of your choice, it doesn't have to be a real command but it will be added to the log file." << endl;
    commandProcessor->getCommand(); //Calls Save Command (You will need to type a command to save into the console. Doesn't check if it's valid in this case.)
    orderList->addOrder(blockade);
    blockade->execute(); //Is a type of order.

}