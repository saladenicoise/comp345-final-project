#include <iostream>
#include <string>
#include "../GameEngine/GameEngine.h"

int main() {
    MapLoader mapLoader;
    Map* map = mapLoader.loadMap("fakemap.map");
    string name = "John";

    vector <Territory*> p1;
    p1.push_back(map->territories[0]);
    p1.push_back(map->territories[1]);
    p1.push_back(map->territories[2]);
    p1.push_back(map->territories[3]);
    vector <Territory*> p2;
    p2.push_back(map->territories[4]);
    p2.push_back(map->territories[5]);
    Player* player = new Player(name);
    Player* player2 = new Player("Tom");
    player->setpID(1);
    player2->setpID(2);
    player->setReinforcementPool(50);
    player2->setReinforcementPool(50);
    player->setTerritories(p1);
    player2->setTerritories(p2);
    vector <Player*> players = {player, player2};
    cout << "do I get here";
    player->toAttack(p1,*player);
    player->toDefend(p1,*player);
    player2->toAttack(p2,*player2);
    player2->toDefend(p2,*player2);
    
    Deck *deck = new Deck();
    for (int i = 0; i < 5; i++) 
    {
        deck->draw(player->getHand());
    }

    GameEngine g("console");
    // g.reinforcementPhase(map->territories, players);
    // g.issueOrderPhase(players,player2,deck);
    // g.executeOrderPhase(players);
     g.mainGameLoop(map->territories,players,deck);


    return 0;
}