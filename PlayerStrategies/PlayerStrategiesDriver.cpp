#include <iostream>
#include <string>
#include "../GameEngine/GameEngine.h"

int main() {
    MapLoader mapLoader; //load map
    Map* map = mapLoader.loadMap("fakemap.map");
    string name = "John";

    vector <Territory*> p1; // set territores in vectors
    p1.push_back(map->territories[0]);
    p1.push_back(map->territories[2]);
  
    vector <Territory*> p2;
    
    p2.push_back(map->territories[4]);
    p2.push_back(map->territories[5]);

    vector <Territory*> p3;
    p3.push_back(map->territories[1]);
    p3.push_back(map->territories[3]);

    Player* player = new Player(name); // initiate players
    Player* player2 = new Player("Tom");
    Player* player3 = new Player("Tim");
    
    player->setpID(1); // set ids
    player2->setpID(2);
    player3->setpID(3);
    
    player->setReinforcementPool(50); // set armies
    player2->setReinforcementPool(50);
    player3->setReinforcementPool(50);
    
    player->setTerritories(p1); // set territories for each player
    player2->setTerritories(p2);
    player3->setTerritories(p3);
    
    vector <Player*> players = {player, player2, player3}; // total players
    
    player->setStrategy(new CheaterPlayerStrategy()); // strats
    player2->setStrategy(new BenevolentPlayerStrategy());
    player3->setStrategy(new NeutralPlayerStrategy());

    player->toDefend(p1,*player); // initiate attack and defend lists
    player->toAttack(p1,*player);
    
    player2->toDefend(p2,*player2);
    player2->toAttack(p2,*player2);
    
    player3->toDefend(p3,*player3);
    player3->toAttack(p3,*player3);
     
    Deck* deck= new Deck(30); // initiate deck

    for (int i = 0; i < 2; i++) { // each player draws 2 cards
        deck->draw(player->getHand());
        deck->draw(player2->getHand());
        deck->draw(player3->getHand());
    }
    GameEngine g("console"); // start game as console
    g.setPlayersList(players);
    g.mainGameLoop(map->territories,players,deck, map->territories.size(), &g);



    return 0;
}