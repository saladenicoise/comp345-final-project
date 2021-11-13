#include "Orders.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.h"
#include "../LoggingObserver/LoggingObserver.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    MapLoader loader;
    Map* map = loader.loadMap("fakemap.map");
    Territory* fakeTer = map->territories[0];
    cout << "Fake Ter: " << fakeTer->countryName;
    Territory* mapTer = map->territories[1];
    cout << "Map Ter: " << mapTer->countryName;
    Territory* withTer = map->territories[2];
    cout << "With Ter: " << withTer->countryName;
    Territory* helloTer = map->territories[3];
    cout << "Hello Ter: " << helloTer->countryName;
    Territory* dataTer = map->territories[4];
    cout << "Data Ter: " << dataTer->countryName;
    Territory* yayTer = map->territories[5];
    cout << "Yay Ter: " << yayTer->countryName;
    vector<Territory *> p1OwnerTer;
    p1OwnerTer.push_back(fakeTer); //Fake
    p1OwnerTer.push_back(mapTer); //Map
    p1OwnerTer.push_back(withTer); //With (Connected to Hello)
    vector<Territory *> p2OwnedTer;
    p2OwnedTer.push_back(helloTer); //Hello (Connected to Data)
    p2OwnedTer.push_back(dataTer); //Data
    p2OwnedTer.push_back(yayTer); //Yay

    //Cannot attack vectors
    vector<Player*> P1CantAttack;
    vector<Player*> P2CantAttack;

    //Create some Cards and Hands containing those cards
    Card *card1 = new Card(cardType::AIRLIFT);
    Card *card2 = new Card(cardType::BLOCKADE);
    Card *card3 = new Card(cardType::BOMB);
    Card *card4 = new Card(cardType::DIPLOMACY);
    Card *card5 = new Card(cardType::REINFORCEMENT);


    vector<Card *> vcards1;
    vcards1.push_back(card1);
    vcards1.push_back(card2);
    vcards1.push_back(card3);
    Hand *handp1 = new Hand(vcards1);

    vector<Card *> vcards2;
    vcards2.push_back(card4);
    vcards2.push_back(card5);
    Hand *handp2 = new Hand(vcards2);

    //Create empty orderslists
    OrderList* ol1 = new OrderList();
    OrderList* ol2 = new OrderList();

    //Create players
    Player* p1 = new Player("Player 1", 1, p1OwnerTer, handp1, ol1, P1CantAttack, 0);
    p1->setReinforcementPool(5);
    Player* p2 = new Player("Player 2", 2, p2OwnedTer, handp2, ol2, P2CantAttack, 0);
    p2->setReinforcementPool(8);
    Player* p3 = new Player("Neutral Player");

    //Test the validate() and execute() for deploy
    Deploy* deployOrder = new Deploy(p1, withTer, 5);
    cout << "\nDeploy is a valid Order: " << (deployOrder->validate() ? "True" : "False") << endl;
    cout << "Number of armies before executing deploy: " << withTer->armyCount << endl;
    deployOrder->execute();
    cout << "Number of armies after executing deploy: " << withTer->armyCount << endl;

    Deploy* deployOrder2 = new Deploy(p2, helloTer, 8);
    cout << "\nDeploy is a valid Order: " << (deployOrder2->validate() ? "True" : "False") << endl;
    cout << "Number of armies before executing deploy: " << helloTer->armyCount << endl;
    deployOrder2->execute();
    cout << "Number of armies after executing deploy: " << helloTer->armyCount << endl;

    //Test the validate() and execute() for advance
    Advance* advanceOrder = new Advance(p2, p1, helloTer, withTer, 8);
    cout << "\nAdvance is a valid Order: " << (advanceOrder->validate() ? "True" : "False") << endl;
    cout << "Number of armies before executing advance in Hello: " << helloTer->armyCount << endl;
    cout << "Number of armies before executing advance in With: " << withTer->armyCount << endl;
    cout << "Player 1 owned territories before attack : " << endl;
    for(Territory* t : p1->getTerritories()){
        cout << t->countryName << " ";
    }
    cout << "\nPlayer 2 owned territories before attack : " << endl;
    for(Territory* t : p2->getTerritories()){
        cout << t->countryName << " ";
    }
    cout << "\n";
    advanceOrder->execute();
    cout << "Number of armies after executing advance in Hello: " << helloTer->armyCount << endl;
    cout << "Number of armies after executing advance in With: " << withTer->armyCount << endl;

    cout << "Player 1 owned territories after attack : " << endl;
     for(Territory* t : p1->getTerritories()){
        cout << t->countryName << " ";
    }
    cout << "\nPlayer 2 owned territories before attack : " << endl;
    for(Territory* t : p2->getTerritories()){
        cout << t->countryName << " ";
    }

    //Test the validate() and execute() for airlift
    Airlift* airliftOrder = new Airlift(p2, withTer, helloTer, 2);
    cout << "\n\nAirlift is a valid Order: " << (airliftOrder->validate() ? "True" : "False") << endl;
    cout << "Number of armies before executing airlift in With: " << withTer->armyCount << endl;
    cout << "Number of armies before executing airlift in Hello: " << helloTer->armyCount << endl;
    airliftOrder->execute();
    cout << "Number of armies after executing airlift in With: " << withTer->armyCount << endl;
    cout << "Number of armies after executing airlift in Hello: " << helloTer->armyCount << endl;

    //Test the validate() and execute() for airlift
    Bomb* bombOrder = new Bomb(p1, helloTer);
    cout << "\nBomb is a valid Order: " << (bombOrder->validate() ? "True" : "False") << endl;
    cout << "Number of armies before executing bomb in Hello: " << helloTer->armyCount << endl;
    bombOrder->execute();
    cout << "Number of armies after executing bomb in Hello: " << helloTer->armyCount << endl;

    //Test the validate() and execute() for blockade
    Blockade* blockadeOrder = new Blockade(p2, helloTer, p3);
    cout << "\nBlockade is a valid Order: " << (blockadeOrder->validate()  ? "True" : "False")<< endl;
    cout << "Number of armies before executing blockade in Hello: " << helloTer->armyCount << endl;
    cout << "Player 2 owned territories before executing blockade: " << endl;
    for(int i =0; i<p2->getTerritories().size(); i++){
        cout << p2->getTerritories()[i]->countryName << endl;
    }
    blockadeOrder->execute();
    cout << "Number of armies after executing blockade in Hello: " << helloTer->armyCount << endl;
    cout << "Player 2 owned territories after executing blockade: " << endl;
    for(int i =0; i<p2->getTerritories().size(); i++){
        cout << p2->getTerritories()[i]->countryName << endl;
    }
    cout << "Territory Was Added to neutral player" << endl;
    cout << "Player 3 (Neutral Player) owned territories after executing blockade: " << endl;
    for(int i =0; i<p3->getTerritories().size(); i++){
        cout << p3->getTerritories()[i]->countryName << endl;
    }

    //Test the validate() and execute() for Negotiate
    Negotiate* negotiateOrder = new Negotiate(p1, p2);
    cout << "\nNegotiate is a valid Order: " << (negotiateOrder->validate() ? "True" : "False")<< endl;

    //Shows that the negotiate being executed prevents the attack from happening
    Advance* advanceOrder2 = new Advance(p2, p1, withTer, mapTer, 2);
    cout << "Advance is a valid Order before Negotiate: " << (advanceOrder2->validate() ? "True" : "False") << endl;
    negotiateOrder->execute();
    cout << "Advance is a valid Order after Negotiate: " << (advanceOrder2->validate() ? "True" : "False") << endl;

    cout << "\n" << endl;
    //Performed after execution of all orders
    if(p2->getGetCard() == 1) {
        cout << "Giving player 2 a new card" << endl;
        Hand *p2Hand = p2->getHand();
        cout << "Player 2 hand before: " << endl;
        for(int i = 0; i < p2Hand->getHandSize(); i++) {
            cout << p2Hand->cardAtIndex(i) << endl;
        }
        cout << "Player 2 hand after" << endl;
        p2Hand->add(cardType::BOMB); //Adding a BOMB card to show that it works
        p2->setGetCard(0);
        for(int i = 0; i < p2Hand->getHandSize(); i++) {
            cout << p2Hand->cardAtIndex(i) << endl;
        }
    }


    return 0;
}