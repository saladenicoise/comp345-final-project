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
    //NEED TO ADD GAMEENGINE THIS THIS DRIVER WHEN ITS DONE.
    GameEngine* gameEngine = new GameEngine("console");
    gameEngine->attach(logObserver);
    gameEngine->transition();
    commandProcessor->attach(logObserver);
    fileCommandProcessorAdapter->attach(logObserver);
    command->attach(logObserver);
    blockade->attach(logObserver);
    orderList->attach(logObserver);
    fileCommandProcessorAdapter->getCommand();
    fileCommandProcessorAdapter->getCommand();
    command->saveEffect("This is a sample effect being saved by command->saveEffect");
    commandProcessor->getCommand(); //Calls Save Command
    orderList->addOrder(blockade);
    blockade->execute(); //Is a type of order.

}