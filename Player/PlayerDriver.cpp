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
  
    vector <Territory*> p2;
    
    p2.push_back(map->territories[4]);
    p2.push_back(map->territories[5]);

    vector <Territory*> p3;
    p3.push_back(map->territories[2]);
    p3.push_back(map->territories[3]);

    Player* player = new Player(name);
    Player* player2 = new Player("Tom");
    Player* player3 = new Player("Tim");
    player->setpID(1);
    player2->setpID(2);
    player3->setpID(3);
    player->setReinforcementPool(50);
    player2->setReinforcementPool(50);
    player3->setReinforcementPool(50);
    player->setTerritories(p1);
    player2->setTerritories(p2);
    player3->setTerritories(p3);
    vector <Player*> players = {player, player2, player3};
    
    player->setStrategy(new HumanPlayerStrategy());
    player2->setStrategy(new BenevolentPlayerStrategy());
    player3->setStrategy(new HumanPlayerStrategy());

    player->toDefend(p1,*player);
    player->toAttack(p1,*player);
    
    player2->toDefend(p2,*player2);
    player2->toAttack(p2,*player2);
    
    player3->toDefend(p3,*player3);
    player3->toAttack(p3,*player3);
     
    Deck* deck= new Deck(30);

    for (int i = 0; i < 2; i++) {
        deck->draw(player->getHand());
        deck->draw(player2->getHand());
        deck->draw(player3->getHand());
    }
    GameEngine g("console");
    g.setPlayersList(players);
    g.mainGameLoop(map->territories,players,deck, map->territories.size(), &g);



    return 0;
}
