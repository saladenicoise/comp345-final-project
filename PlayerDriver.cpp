#include "Player.h"

int main()
{
    string name = "John";
    Territory* t = new Territory();
    t->setTerritory("Europe");
    t->setTerritory("Russia");
    Cards* c = new Cards();
    c->setCard("bomb");
    c->setCard("airlift");
    vector<Order*> o;

    Player* player = new Player(name,1,t,c,o);

    std::cout << "Initializing Player" << player->getPID() << ": "  << player->getPlayerName()<< "\n";
    player->toAttack();
    player->toDefend();
    player->issueOrder("bomb");
    player->issueOrder("deploy");
    player->printOrder();
    
    return 0;
}