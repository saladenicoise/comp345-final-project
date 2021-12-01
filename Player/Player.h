//This is the header file Player.h. 
//It contains all the class and operator declarations.
//Also provides dummy classes for Territory class, Card class and Order class.
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <iostream>
#include <ostream> 
#include <algorithm>

#include "../Orders/Orders.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../PlayerStrategies/PlayerStrategies.h"

class Territory;
class Hand;
class OrderList;
class Order;
class Deck;
class PlayerStrategy;
class GameEngine;

class Player //this is the Player class
{
    
    public:
        Player(); //default constructor
        Player(string playerName);
        Player(string playerName, int isNeutral);
        Player(string playerName, int pid, vector<Territory*> t, Hand* h, OrderList* olst, vector<Player*> noAttack, int isNeutral); //constructor  
        Player(const Player& p); //copy constructor
        ~Player(); //deconstructor 
        string getPlayerName();
        int getPID(); 
        void issueOrder(const string& order);
        void printOrder();
        Player& operator=(const Player& p); //overloaded assignment operator
        friend std::ostream& operator<<(std::ostream &os,const Player& p); //stream insertion operator
        Hand* getHand();
        OrderList* getOrderList();

        vector<Player*> getNotAttackablePlayers();
        void setNotAttackablePlayers(vector<Player*> newUnattackablePlayer);
        void setTerritories(vector<Territory*> newTerritories);
        int getReinforcementPool();
        void setReinforcementPool(int pool);
        void setpID(int pID);
        int getNeutral();
        void setGetCard(int getCard);
        int getGetCard();
        vector<Territory*> getNeighbour(vector<Territory*>);
        void issuingOrder(Player* p, GameEngine *game, Deck* deck);
        void issueOrderObject(const Order); //creates an order object and adds it to the list of orders. 
        int getOrderSize(int size); //returns order list object
        void selectOrder(Player *p, vector<Player*> targetPlayer, Deck* deck);
        vector<Territory*> getTerritories();
        
        vector<Territory*> toDefend(vector<Territory*>, Player& p);
        vector<Territory*> toAttack(vector<Territory*>, Player& p);

        vector<Territory*> getDefendList();
        void setDefendList(vector<Territory*> newDefendList);
        vector<Territory*> getAttackList();
        void setAttackList(vector<Territory*> newAttackList);

        void setStrategy(PlayerStrategy *newStrategy);
        PlayerStrategy* getPlayerStrategy();


    private: // data members
        string playerName;
        int pid;
        vector<Territory*> t;
        Hand* h; 
        OrderList* olst;
        int reinforcementPool;
        vector<Player*> cannotAttack;
        int isNeutral;
        int getCard;
        int tempReinforcementPool;
        PlayerStrategy *strategy;
        vector<Territory*> attackList;
        vector<Territory*> defendList;

        friend class GameEngine;
};

#endif //PLAYER_H
