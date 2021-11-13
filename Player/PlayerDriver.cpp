#include "Player.h"

int main()
{
    string name = "John";
    Territory* t = new Territory("Germany", 1, 6);
    Territory* t2 = new Territory("Russia", 3, 6);
	vector<Territory*> toDefendTerritory;
	vector<Territory*> toAttackTerritory;

    Deck* deck = new Deck();
    Player* player = new Player();

    std::cout << *player <<std::endl;

    for (int i = 0; i < 5; i++) {
        deck->draw(player->getHand());
    }
    // Issue Orders from hand
    player->getHand()->cardAtIndex(0).play(*player, *deck, *player->getHand());
    player->getHand()->cardAtIndex(1).play(*player, *deck, *player->getHand());

	toDefendTerritory.push_back(t);
	toAttackTerritory.push_back(t2);

    player->toDefend(toDefendTerritory, *player);
    player->toAttack(toAttackTerritory, *player);


    std::cout << "Player 0's order list: \n" << *player->getOrderList() <<std::endl;
    
    Player* player1 = new Player();
    player1->toAttack(toAttackTerritory, *player1);
    std::cout << "Player 1's order list: \n" << *player1->getOrderList() <<std::endl;
    
    return 0;
}
